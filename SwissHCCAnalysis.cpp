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
{
    QImage hcctemplate(":/hcctemplate.png");
    m_templateHCC=cv::Mat(hcctemplate.height(), hcctemplate.width(), CV_8UC1 , hcctemplate.bits(), hcctemplate.bytesPerLine()).clone();

    QImage hccprobestemplate(":/hccprobestemplate.png");
    m_templateProbes=cv::Mat(hccprobestemplate.height(), hccprobestemplate.width(), CV_8UC1 , hccprobestemplate.bits(), hccprobestemplate.bytesPerLine()).clone();
}

void SwissHCCAnalysis::setLogDirectory(const QString& logDirectory)
{
    m_logDirectory=logDirectory;

    if(!m_logDirectory.isEmpty())
    {
        m_logFH.setFileName(m_logDirectory+"/output.log");
        m_logFH.open(QIODevice::WriteOnly | QIODevice::Text);
        m_log.setDevice(&m_logFH);
    }
}

void SwissHCCAnalysis::setValidSlots(const QList<QPoint>& validSlots)
{ m_validSlots=validSlots; }

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

void SwissHCCAnalysis::logStatus(const QString& message)
{
    m_log << message << "\n";
    emit status(message);
}

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
    cv::HoughLines(imgedges,lines,1,CV_PI/180.,70);

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

    QImage newimg=img;
    cv::Mat cvimg(img.height(), img.width(), CV_8UC1 , newimg.bits(), img.bytesPerLine());
    cv::Mat imgpass;
    cv::threshold(cvimg, imgpass, 25, 255, cv::THRESH_BINARY_INV);

    cv::Mat result;
    cv::matchTemplate(imgpass,m_templateProbes,result,cv::TM_CCOEFF_NORMED);

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

        painter.drawRect(max_loc.x,max_loc.y,m_templateProbes.cols,m_templateProbes.rows);

        painter.end();

        emit updateImage(imgnew);
    }
    else
    {
        m_probesOffsetScore=max;
        m_probesOffsetX=(max_loc.x)*0.0076;
        m_probesOffsetY=(max_loc.y)*0.0076;
    }
}

void SwissHCCAnalysis::analyzeFindGroove(const QImage& img)
{
    bool inWorkThread=QThread::currentThread()==thread();

    QImage imgnew=img.convertToFormat(QImage::Format_RGB32);
    QPainter painter(&imgnew);
    painter.setBrush(Qt::NoBrush);
    painter.setPen(Qt::red);

    painter.drawRect(160,150,290,170);
    QImage imgArea=img.copy(160,150,290,170);

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
    bool crossFound=((vn>0) && (hn>0) && (fabs(cos(vtheta-htheta))<1.*CV_PI/180.));
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

    QImage newimg=img;
    cv::Mat cvimg(img.height(), img.width(), CV_8UC1 , newimg.bits(), img.bytesPerLine());

    cv::Mat result;
    cv::matchTemplate(cvimg,m_templateHCC,result,cv::TM_CCOEFF_NORMED);

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

        painter.drawRect(max_loc.x,max_loc.y,m_templateHCC.cols,m_templateHCC.rows);

        painter.end();

        emit chipFound(max);
        emit updateImage(imgnew);
    }
    else
    {
        m_chipOffsetScore=max;
        m_chipOffsetX=(max_loc.x+m_templateHCC.cols)*0.0076;
        m_chipOffsetY=(max_loc.y+m_templateHCC.rows)*0.0076;
    }
}

void SwissHCCAnalysis::run()
{ }

void SwissHCCAnalysis::runLoadChips()
{
    logStatus("LOADING CHIPS");
    m_imageAnalysisState=None;

    getECS02()->moveLoad();
    getECS02()->waitForIdle();

    emit donePrepareLoadChips();
}

void SwissHCCAnalysis::runFindProbes()
{
    emit message(tr("FINDING PROBES"));
    logStatus("FINDING PROBES");

    getECS02()->moveAbsolute(0, 20);
    getECS02()->waitForIdle();

    m_imageAnalysisState=FindProbes;
    QThread::msleep(200);
    const QImage &img=getFrameGrabber()->getImage(true);
    analyzeFindProbes(img);

    if(m_probesOffsetScore<0.5)
    {
        logStatus(QString("Cannot find probes. Best score is %1.").arg(m_probesOffsetScore));
        emit message(tr("CANNOT FIND PROBES!"));
        emit finished();
        return;
    }
    logStatus(QString("Found probes with score %1 at %2,%3").arg(m_probesOffsetScore).arg(m_probesOffsetX).arg(m_probesOffsetY));
    emit message(tr("PROBES FOUND"));
    m_imageAnalysisState=None;

    emit doneFindProbes();
}

void SwissHCCAnalysis::runCalibratePosition()
{
    emit message(tr("POSITION CALIBRATION"));
    logStatus("FINDING POSITION CALIBRATION GROOVE");

    getECS02()->moveAbsolute(0, 20);
    getECS02()->waitForIdle();

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
        logStatus("No edge found.");
        emit finished();
        return;
    }

    emit message("EDGE FOUND. FINDING CROSS");
    logStatus("Edge found.");

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
        logStatus("No cross found.");
        emit finished();
        return;
    }

    emit message("CROSS FOUND. ROTATE!");
    logStatus(QString("Initial cross found at %1,%2.").arg(m_crossPoint.x()).arg(m_crossPoint.y()));
    emit doneFindCross();
}

void SwissHCCAnalysis::runCrossTest()
{
    logStatus("RUNNING CROSS TEST");
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
    logStatus(QString("Measured angle as %1").arg(angle));
}

void SwissHCCAnalysis::runCrossSave()
{
    //
    // Get final position
    QImage img=getFrameGrabber()->getImage(true);
    analyzeFindGrooveCross(img);

    getECS02()->updateInfo();
    getECS02()->waitForIdle();

    logStatus(QString("Final cross position is %1,%2.").arg(m_crossPoint.x()).arg(m_crossPoint.y()));
    m_crossPoint=m_crossPoint*0.0076+QPointF(getECS02()->getY(),getECS02()->getX());
    logStatus(QString("Absolute cross position is %1,%2.").arg(m_crossPoint.x()).arg(m_crossPoint.y()));

    runFindChips();
}


void SwissHCCAnalysis::runFindChips()
{
    logStatus("---- START CHIP TESTING ----");
    m_imageAnalysisState=None;

    emit startFindChip();

    //
    // Move to chip 1
    if(m_validSlots.size()>0)
        runFindChip(m_validSlots.at(0));
    else
        runFindChip(QPoint(0,0));
}

void SwissHCCAnalysis::runFindChip(const QPoint& slot)
{
    logStatus(QString("-- CHIP TEST %1,%2 --").arg(slot.x()).arg(slot.y()));
    emit findingChip();
    m_imageAnalysisState=None;
    QPointF chipPos=m_crossPoint-QPointF(5,6.6)-QPointF(10*slot.y(),8*(4-slot.x()));
    getECS02()->moveAbsolute(chipPos.y(),chipPos.x());
    getECS02()->waitForIdle();
    m_activeSlot=slot;
    runAlignChip();
}

void SwissHCCAnalysis::runAlignChip()
{
    m_imageAnalysisState=AlignChip;

    QImage img=getFrameGrabber()->getImage(true);
    analyzeAlignChip(img);

    if(m_chipOffsetScore>0.7)
    {
        getECS02()->updateInfo();
        getECS02()->waitForIdle();

        static QPointF offsetStaticPad(73,30);
        static QPointF offsetStaticProbes(130,215);
        QPointF newPos=QPointF(getECS02()->getY(),getECS02()->getX())+QPointF(m_chipOffsetX,m_chipOffsetY)-offsetStaticPad*0.0076-QPointF(m_probesOffsetX,m_probesOffsetY)-offsetStaticProbes*0.0076;
        getECS02()->moveAbsolute(newPos.y(),newPos.x());
        logStatus(QString("Chip found with score %1 at position %2,%3.").arg(m_chipOffsetScore).arg(m_chipOffsetX).arg(m_chipOffsetY));
        emit chipAlignSuccess();
    }
    else if(m_chipOffsetScore<0.1)
    {
        logStatus(QString("No chip found. Score is %1.").arg(m_chipOffsetScore));
        emit chipAlignFailed();
    }
}

void SwissHCCAnalysis::runChipTest()
{
    logStatus("Running chip test.");
    if(!m_logDirectory.isEmpty())
    {
        QImage img=getFrameGrabber()->getImage();
        img.save(QString("%1/chip_%2_%3.png").arg(m_logDirectory).arg(m_activeSlot.x()).arg(m_activeSlot.y()));
    }
}
