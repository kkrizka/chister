#include "SwissHCCAnalysis.h"

#include <QDebug>
#include <QThread>
#include <QLabel>
#include <QPainter>

#include <math.h>

SwissHCCAnalysis::SwissHCCAnalysis(FrameGrabber *frameGrabber, ECS02 *ecs02, QObject *parent)
    : AnalysisProgram(frameGrabber, ecs02, parent), m_imageAnalysisState(None),
      m_edgeFound(false)
{ }

void SwissHCCAnalysis::setValidSlots(const QList<QPoint>& validSlots)
{ m_validSlots=validSlots; }

std::vector<cv::Vec2f> SwissHCCAnalysis::findLines(const QImage& img) const
{
    QImage newimg=img;
    cv::Mat cvimg(img.height(), img.width(), CV_8UC1 , newimg.bits(), img.bytesPerLine());

    // Find edges
    cv::Mat imgpass;
    cv::threshold(cvimg, imgpass, 100, 255, cv::THRESH_BINARY);

    cv::Mat imgedges;
    cv::Canny(imgpass,imgedges, 100, 200);

    std::vector<cv::Vec2f> lines;
    cv::HoughLines(imgedges,lines,1,CV_PI/180.,100);

    for(auto &line : lines)
    {
        if(line[0]<0)
        {
            line[0]=-line[0];
            line[1]+=(float)CV_PI;
        }
    }

    return lines;
}

std::vector<cv::Vec2f> SwissHCCAnalysis::findGrooves(const QImage& img) const
{
    std::vector<cv::Vec2f> lines=findLines(img);

    // Find candidates (parallel and 1 mm apart)
    std::vector<cv::Vec2f> candidates;
    if(lines.size()==0) return candidates;

    // Search
    for(uint l1idx=0;l1idx<lines.size()-1;l1idx++)
    {
        const cv::Vec2f &l1=lines[l1idx];
        for(uint l2idx=l1idx+1;l2idx<lines.size();l2idx++)
        {
            const cv::Vec2f &l2=lines[l2idx];
            if(fabs(sin(l2[1]-l1[1]))>2*CV_PI/180.) continue; // Not parallel

            float dist=fabs(l2[0]-l1[0])*0.0076;
            if(fabs(dist-1)>0.05) continue; // Not 1mm apart

            candidates.push_back(cv::Vec2f((l1[0]+l2[0])/2.,l1[1]));
        }
    }

    return candidates;
}

void SwissHCCAnalysis::analyze(const QImage& img)
{
    switch(m_imageAnalysisState)
    {
    case FindProbes:
        analyzeFindProbes(img);
        break;
    case FindGroove:
        analyzeFindGroove(img);
        break;
    case FindGrooveCross:
        analyzeFindGrooveCross(img);
        break;
    case AlignChip:
        analyzeAlignChip(img);
        break;
    default:
        AnalysisProgram::analyze(img);
    }
}

void SwissHCCAnalysis::analyzeFindProbes(const QImage& img)
{
    bool inWorkThread=QThread::currentThread()==thread();

    QImage hccprobestemplate(":/hccprobestemplate.png");
    cv::Mat imgtemplate(hccprobestemplate.height(), hccprobestemplate.width(), CV_8UC1 , hccprobestemplate.bits(), hccprobestemplate.bytesPerLine());

    QImage newimg=img;
    cv::Mat cvimg(img.height(), img.width(), CV_8UC1 , newimg.bits(), img.bytesPerLine());
    cv::Mat imgpass;
    cv::threshold(cvimg, imgpass, 30, 255, cv::THRESH_BINARY_INV);

    cv::Mat result;
    cv::matchTemplate(imgpass,imgtemplate,result,cv::TM_CCOEFF_NORMED);

    double min, max;
    cv::Point min_loc, max_loc;
    cv::minMaxLoc(result, &min, &max, &min_loc, &max_loc);
    //qInfo() << "Found maximum of" << max << "at" << max_loc.x << "" << max_loc.y;

    if(!inWorkThread)
    {
        QImage imgnew=img.convertToFormat(QImage::Format_RGB32);
        QPainter painter(&imgnew);
        painter.setBrush(Qt::NoBrush);
        if(0<max && max < 0.3)
            painter.setPen(Qt::red);
        else if(0.3<max && max < 0.9)
            painter.setPen(Qt::yellow);
        else
            painter.setPen(Qt::green);

        painter.drawRect(max_loc.x,max_loc.y,hccprobestemplate.width(),hccprobestemplate.height());

        painter.end();

        emit updateImage(imgnew);
    }
    else
    {
        m_probesOffsetScore=max;
        m_probesOffsetX=(max_loc.x+hccprobestemplate.width() )*0.0076;
        m_probesOffsetY=(max_loc.y+hccprobestemplate.height())*0.0076;
    }
}

void SwissHCCAnalysis::analyzeFindGroove(const QImage& img)
{
    bool inWorkThread=QThread::currentThread()==thread();

    QImage imgnew=img.convertToFormat(QImage::Format_RGB32);
    QPainter painter(&imgnew);
    painter.setBrush(Qt::NoBrush);
    painter.setPen(Qt::red);

    std::vector<cv::Vec2f> candidates=findGrooves(img);
    for(const auto& line : candidates)
    {
        if((-CV_PI/4<line[1] && line[1]<CV_PI/4) || (3*CV_PI/4<line[1] && line[1]<5*CV_PI/4))
            painter.drawLine(line[0]/cos(line[1]),0, -sin(line[1])/cos(line[1])*img.height()+line[0]/cos(line[1]),img.height());
        else
            painter.drawLine(0,line[0]/sin(line[1]), img.width() ,-cos(line[1])/sin(line[1])*img.width() +line[0]/sin(line[1]));
    }
    painter.end();

    if(inWorkThread)
    {
        m_edgeFound=candidates.size()>0;
    }
    else
        emit updateImage(imgnew);
}

void SwissHCCAnalysis::analyzeFindGrooveCross(const QImage& img)
{
    bool inWorkThread=QThread::currentThread()==thread();

    QImage imgnew=img.convertToFormat(QImage::Format_RGB32);
    QPainter painter(&imgnew);
    painter.setBrush(Qt::NoBrush);
    painter.setPen(Qt::red);

    std::vector<cv::Vec2f> candidates=findGrooves(img);
    // Calculate the average angle and draw all candidates
    float avgAngle=0.;
    for(const auto& line : candidates)
    {
        // Angle
        double angle=line[1];
        if(CV_PI   /4.<angle && angle<CV_PI*3./4.) angle-=CV_PI   /2.;
        if(CV_PI*3./4.<angle && angle<CV_PI*5./4.) angle-=CV_PI;
        if(CV_PI*5./4.<angle && angle<CV_PI*7./4.) angle-=CV_PI*3./2.;
        if(CV_PI*7./4.<angle && angle<CV_PI*9./4.) angle-=CV_PI*2;
        avgAngle=angle/candidates.size();

        // Draw
        if((-CV_PI/4<line[1] && line[1]<CV_PI/4) || (3*CV_PI/4<line[1] && line[1]<5*CV_PI/4))
            painter.drawLine(line[0]/cos(line[1]),0, -sin(line[1])/cos(line[1])*img.height()+line[0]/cos(line[1]),img.height());
        else
            painter.drawLine(0,line[0]/sin(line[1]), img.width() ,-cos(line[1])/sin(line[1])*img.width() +line[0]/sin(line[1]));
    }

    // Find intersection point of two candidates
    bool crossFound=false;
    QPointF crossPoint;
    if(candidates.size()>1)
    {
        // Find perpendicular lines
        cv::Vec2f l1, l2;
        for(uint l1idx=0;l1idx<candidates.size()-1;l1idx++)
        {
            l1=candidates[l1idx];
            for(uint l2idx=l1idx+1;l2idx<candidates.size();l2idx++)
            {
                l2=candidates[l2idx];

                if(fabs(cos(l2[1]-l1[1]))<1.*CV_PI/180.)
                {
                    crossFound=true;
                    break;
                }
            }
            if(crossFound) break;
        }

        if(crossFound)
        {
            float r1=l1[0];
            float a1=l1[1];
            float r2=l2[0];
            float a2=l2[1];
            crossPoint=QPointF(
                        (r2*sin(a1)-r1*sin(a2))/(cos(a2)*sin(a1)-cos(a1)*sin(a2)),
                        (r2/cos(a2)-r1/cos(a1))/(tan(a2)-tan(a1))
                        );
            painter.setBrush(QBrush(Qt::red));
            painter.drawEllipse(crossPoint.x()-5,crossPoint.y()-5,10,10);
        }
    }

    painter.end();

    // Update info
    if(inWorkThread)
    {
        m_crossFound=crossFound;
        m_crossPoint=crossPoint;
    }
    else
    {
        emit updateImage(imgnew);
        emit foundCross(avgAngle);
    }
}

void SwissHCCAnalysis::analyzeAlignChip(const QImage& img)
{
    bool inWorkThread=QThread::currentThread()==thread();

    QImage hcctemplate(":/hcctemplate.png");
    cv::Mat imgtemplate(hcctemplate.height(), hcctemplate.width(), CV_8UC1 , hcctemplate.bits(), hcctemplate.bytesPerLine());

    QImage newimg=img;
    cv::Mat cvimg(img.height(), img.width(), CV_8UC1 , newimg.bits(), img.bytesPerLine());

    cv::Mat result;
    cv::matchTemplate(cvimg,imgtemplate,result,cv::TM_CCOEFF_NORMED);

    double min, max;
    cv::Point min_loc, max_loc;
    cv::minMaxLoc(result, &min, &max, &min_loc, &max_loc);
    //qInfo() << "Found maximum of" << max << "at" << max_loc.x << "" << max_loc.y;

    if(!inWorkThread)
    {
        QImage imgnew=img.convertToFormat(QImage::Format_RGB32);
        QPainter painter(&imgnew);
        painter.setBrush(Qt::NoBrush);
        if(0<max && max < 0.3)
            painter.setPen(Qt::red);
        else if(0.3<max && max < 0.9)
            painter.setPen(Qt::yellow);
        else
            painter.setPen(Qt::green);

        painter.drawRect(max_loc.x,max_loc.y,hcctemplate.width(),hcctemplate.height());

        painter.end();

        emit updateImage(imgnew);
    }
    else
    {
        m_chipOffsetScore=max;
        m_chipOffsetX=(max_loc.x+hcctemplate.width() )*0.0076;
        m_chipOffsetY=(max_loc.y+hcctemplate.height())*0.0076;
    }
}

void SwissHCCAnalysis::run()
{
    m_imageAnalysisState=None;

    getECS02()->moveLoad();
    getECS02()->waitForIdle();

    emit stepMoveToLoadDone();
}

void SwissHCCAnalysis::runFindProbes()
{
    emit message(tr("FINDING PROBES"));

    getECS02()->moveAbsolute(0, 20);
    getECS02()->waitForIdle();

    m_imageAnalysisState=FindProbes;
    QThread::msleep(200);
    const QImage &img=getFrameGrabber()->getImage(true);
    analyzeFindProbes(img);

    if(m_probesOffsetScore<0.5)
    {
        emit message(tr("CANNOT FIND PROBES!"));
        emit finished();
        return;
    }
    emit message(tr("PROBES FOUND"));
    m_imageAnalysisState=None;

    emit stopFindProbesDone();

    runCalibration();
}

void SwissHCCAnalysis::runCalibration()
{
    emit message(tr("POSITION CALIBRATION"));

    getECS02()->moveAbsolute(0, 20);
    getECS02()->waitForIdle();

    emit message(tr("POSITION ALIGNMENT"));

    m_imageAnalysisState=FindGroove;
    for(uint i=0;i<10;i++)
    {
        QThread::msleep(200);
        QImage img=getFrameGrabber()->getImage(true);
        analyzeFindGroove(img);

        if(m_edgeFound) break;

        getECS02()->moveIncrement(0,-int(1./getECS02()->getIncrementY()));
        getECS02()->waitForIdle();
    }

    if(!m_edgeFound)
    {
        emit message("EDGE NOT FOUND");
        emit finished();
        return;
    }

    emit message("EDGE FOUND. FINDING CROSS");

    //
    // Find the cross!
    m_imageAnalysisState=FindGrooveCross;
    for(uint i=0;i<30;i++)
    {
        QThread::msleep(200);
        QImage img=getFrameGrabber()->getImage(true);
        analyzeFindGrooveCross(img);
        if(m_crossFound) break;

        getECS02()->moveIncrement(int(1./getECS02()->getIncrementX()),0);
        getECS02()->waitForIdle();
    }

    if(!m_crossFound)
    {
        emit message("CROSS NOT FOUND");
        emit finished();
        return;
    }

    emit message("CROSS FOUND. ROTATE!");
    emit stepFindCrossDone();
}

void SwissHCCAnalysis::runCrossTest()
{
    //
    // Get current position
    QImage img=getFrameGrabber()->getImage(true);
    std::vector<cv::Vec2f> candidates=findGrooves(img);

    getECS02()->updateInfo();
    getECS02()->waitForIdle();
    float oldY=getECS02()->getY();
    float oldLY=0.;
    for(const auto& candidate : candidates)
    {
        if(fabs(cos(candidate[1]))<0.5)
        {
            oldLY=candidate[0]/sin(candidate[1]);
            break;
        }
    }

    //
    // Move far away (200 mm)
    getECS02()->moveIncrement(0,-int(200./getECS02()->getIncrementY()));
    getECS02()->waitForIdle();

    //
    // Get new position
    img=getFrameGrabber()->getImage(true);
    candidates=findGrooves(img);

    getECS02()->updateInfo();
    getECS02()->waitForIdle();
    float newY=getECS02()->getY();
    float newLY=0.;
    for(const auto& candidate : candidates)
    {
        if(fabs(cos(candidate[1]))<0.5)
        {
            newLY=candidate[0]/sin(candidate[1]);
            break;
        }
    }
    float angle=atan2(oldLY-newLY,(oldY-newY)/0.0076);

    //
    // Move back
    getECS02()->moveIncrement(0,+int(200./getECS02()->getIncrementY()));
    getECS02()->waitForIdle();

    emit testCrossAngle(angle);
}

void SwissHCCAnalysis::runFindChips()
{
    m_imageAnalysisState=None;

    //
    // Get final position
    QImage img=getFrameGrabber()->getImage(true);
    analyzeFindGrooveCross(img);

    getECS02()->updateInfo();
    getECS02()->waitForIdle();

    m_crossPoint=m_crossPoint*0.0076+QPointF(getECS02()->getY(),getECS02()->getX());

    emit startFindChip();

    //
    // Move to chip 1
    runFindChip(QPoint(0,0));
}

void SwissHCCAnalysis::runFindChip(const QPoint& slot)
{
    m_imageAnalysisState=None;
    QPointF chipPos=m_crossPoint-QPointF(5,6.6)-QPointF(10*slot.y(),8*(4-slot.x()));
    getECS02()->moveAbsolute(chipPos.y(),chipPos.x());
    m_activeSlot=slot;
    runAlignChip();
}

void SwissHCCAnalysis::runAlignChip()
{
    m_imageAnalysisState=AlignChip;

    QImage img=getFrameGrabber()->getImage(true);
    analyzeAlignChip(img);

    if(m_chipOffsetScore>0.3)
    {
        getECS02()->updateInfo();
        getECS02()->waitForIdle();

        QPointF newPos=QPointF(getECS02()->getY(),getECS02()->getX())+QPointF(m_chipOffsetX,m_chipOffsetY)-QPointF(img.width(),img.height())*0.0076;
        getECS02()->moveAbsolute(newPos.y(),newPos.x());
    }
    else
    {
        emit chipAlignFailed();
    }
}

void SwissHCCAnalysis::runChipTest()
{
    QImage img=getFrameGrabber()->getImage();
    img.save(QString("chip_%1_%2.png").arg(m_activeSlot.x()).arg(m_activeSlot.y()));
}
