#include "SwissHCCAnalysis.h"

#include <QDebug>
#include <QThread>
#include <QLabel>
#include <QPainter>

#include <math.h>

#include "QOpenCVHelpers.h"

SwissHCCAnalysis::SwissHCCAnalysis(FrameGrabber *frameGrabber, ECS02 *ecs02, QObject *parent)
    : AnalysisProgram(frameGrabber, ecs02, parent), m_imageAnalysisState(None),
      m_edgeFound(false)
{ }

void SwissHCCAnalysis::settingsSave(QSettings *settings)
{
    settings->setValue("SwissHCC/crossPoint_x",m_crossPoint.x());
    settings->setValue("SwissHCC/crossPoint_y",m_crossPoint.y());

    settings->setValue("SwissHCC/probesOffset_x",m_probesOffsetX);
    settings->setValue("SwissHCC/probesOffset_y",m_probesOffsetY);
}

void SwissHCCAnalysis::settingsLoad(QSettings *settings)
{
    m_crossPoint.setX(settings->value("SwissHCC/crossPoint_x",0.).toFloat());
    m_crossPoint.setY(settings->value("SwissHCC/crossPoint_y",0.).toFloat());

    m_probesOffsetX=settings->value("SwissHCC/probesOffset_x",0.).toFloat();
    m_probesOffsetY=settings->value("SwissHCC/probesOffset_y",0.).toFloat();
}


void SwissHCCAnalysis::setValidSlots(const QList<QPoint>& validSlots)
{ m_validSlots=validSlots; }

std::vector<cv::Vec2f> SwissHCCAnalysis::findLines(const QImage& img) const
{
    QImage newimg=img;
    cv::Mat cvimg(img.height(), img.width(), CV_8UC1 , newimg.bits(), img.bytesPerLine());

    // Find edges
    cv::Mat imgpass;
    cv::threshold(cvimg, imgpass, 70, 255, cv::THRESH_BINARY);

    cv::Mat imgedges;
    cv::Canny(imgpass,imgedges, 100, 200);

    std::vector<cv::Vec2f> lines;
    cv::HoughLines(imgedges,lines,1,CV_PI/180.,50);

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

    QImage imgArea=img.copy(160,150,290,170);

    QImage imgnew=img.convertToFormat(QImage::Format_RGB32);
    QPainter painter(&imgnew);
    painter.setBrush(Qt::NoBrush);
    painter.setPen(Qt::red);

    std::vector<cv::Vec2f> candidates=findGrooves(imgArea);
    float r=0,c=0,s=0,theta=0;
    for(auto& line : candidates)
    {
        line[0]+=160*cos(line[1]);
        line[0]+=150*sin(line[1]);
        r+=line[0];
        c+=cos(line[1]);
        s+=sin(line[1]);
    }
    theta=atan2(s,c);
    r/=candidates.size();
    if((-CV_PI/4<theta && theta<CV_PI/4) || (3*CV_PI/4<theta && theta<5*CV_PI/4))
        painter.drawLine(r/cos(theta),0, -sin(theta)/cos(theta)*img.height()+r/cos(theta),img.height());
    else
        painter.drawLine(0,r/sin(theta), img.width() ,-cos(theta)/sin(theta)*img.width() +r/sin(theta));

    painter.end();

    if(inWorkThread)
    {
        m_edgeFound=candidates.size()>0;
        m_edgeAngle=theta;
    }
    else
        emit updateImage(imgnew);
}

void SwissHCCAnalysis::analyzeFindGrooveCross(const QImage& img)
{
    bool inWorkThread=QThread::currentThread()==thread();

    QImage imgArea=img.copy(160,150,290,170);

    QImage imgnew=img.convertToFormat(QImage::Format_RGB32);
    QPainter painter(&imgnew);
    painter.setBrush(Qt::NoBrush);
    painter.setPen(Qt::red);

    std::vector<cv::Vec2f> candidates=findGrooves(img);
    float r=0,c=0,s=0; // groove being processed
    float vr=0,vc=0,vs=0; // vertical groove
    float hr=0,hc=0,hs=0; // horizontal groove
    uint vn=0,hn=0;
    for(auto& line : candidates)
    {
        //line[0]+=160*cos(line[1]);
        //line[0]+=150*sin(line[1]);

        r=line[0];
        c=cos(line[1]);
        s=sin(line[1]);
        if(c>s)
        {
            hr+=r;
            hc+=c;
            hs+=s;
            hn++;
        }
        else
        {
            vr+=r;
            vc+=c;
            vs+=s;
            vn++;
        }
    }
    float vtheta=atan2(vs,vc);
    float htheta=atan2(hs,hc);
    vr/=vn;
    hr/=hn;

    // Calculate the average angle and draw all candidates
    float avgAngle=htheta;
    if(vn>0) painter.drawLine(cvline2qlinef(cv::Vec2f(vr,vtheta),img.width(),img.height()));
    if(hn>0) painter.drawLine(cvline2qlinef(cv::Vec2f(hr,htheta),img.width(),img.height()));

    // Find intersection point of two candidates
    bool crossFound=((vn>0) && (hn>0));
    QPointF crossPoint;
    if(crossFound)
    {
        QLineF vline=cvline2qlinef(cv::Vec2f(vr,vtheta),img.width(),img.height());
        QLineF hline=cvline2qlinef(cv::Vec2f(hr,htheta),img.width(),img.height());
        crossFound=(vline.intersect(hline,&crossPoint)==QLineF::BoundedIntersection);

        painter.setBrush(QBrush(Qt::red));
        painter.drawEllipse(crossPoint.x()-5,crossPoint.y()-5,10,10);
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
    //return;

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

        getECS02()->moveIncrement(int(cos(m_edgeAngle)/getECS02()->getIncrementX()),-int(sin(m_edgeAngle)/getECS02()->getIncrementY()));
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

void SwissHCCAnalysis::runCrossSave()
{
    //
    // Get final position
    QImage img=getFrameGrabber()->getImage(true);
    analyzeFindGrooveCross(img);

    getECS02()->updateInfo();
    getECS02()->waitForIdle();

    m_crossPoint=m_crossPoint*0.0076+QPointF(getECS02()->getY(),getECS02()->getX());

    runFindChips();
}


void SwissHCCAnalysis::runFindChips()
{
    m_imageAnalysisState=None;

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
