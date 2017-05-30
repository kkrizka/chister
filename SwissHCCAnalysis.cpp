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

std::vector<cv::Vec2f> SwissHCCAnalysis::findGrooves(const QImage& img) const
{
    QImage newimg=img;
    cv::Mat cvimg(img.height(), img.width(), CV_8UC1 , newimg.bits(), img.bytesPerLine());

    // Find edges
    cv::Mat imgpass;
    cv::threshold(cvimg, imgpass, 100, 255, cv::THRESH_BINARY);

    cv::Mat imgedges;
    cv::Canny(imgpass,imgedges, 100, 200);

    std::vector<cv::Vec2f> lines;
    cv::HoughLines(imgedges,lines,1,CV_PI/180,100);

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
            if(l1[1]!=l2[1]) continue; // Not parallel

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
    case FindGroove:
        analyzeFindGroove(img);
        break;
    case FindGrooveCross:
        analyzeFindGrooveCross(img);
        break;
    default:
        AnalysisProgram::analyze(img);
    }

    m_waitForAnalyze.wakeAll();
}

void SwissHCCAnalysis::analyzeFindGroove(const QImage& img)
{
    std::vector<cv::Vec2f> candidates=findGrooves(img);

    QImage imgnew=img.convertToFormat(QImage::Format_RGB32);
    QPainter painter(&imgnew);
    painter.setBrush(Qt::NoBrush);
    painter.setPen(Qt::red);

    for(const auto& line : candidates)
    {
        if((-CV_PI/4<line[1] && line[1]<CV_PI/4) || (3*CV_PI/4<line[1] && line[1]<5*CV_PI/4))
            painter.drawLine(line[0]/cos(line[1]),0, -sin(line[1])/cos(line[1])*img.height()+line[0]/cos(line[1]),img.height());
        else
            painter.drawLine(0,line[0]/sin(line[1]), img.width() ,-cos(line[1])/sin(line[1])*img.width() +line[0]/sin(line[1]));
    }
    painter.end();

    emit updateImage(imgnew);

    if(m_analysisResultsMutex.tryLock())
    {
        m_edgeFound=candidates.size()>0;
        m_analysisResultsMutex.unlock();
    }
}

void SwissHCCAnalysis::analyzeFindGrooveCross(const QImage& img)
{
    std::vector<cv::Vec2f> candidates=findGrooves(img);

    QImage imgnew=img.convertToFormat(QImage::Format_RGB32);
    QPainter painter(&imgnew);
    painter.setBrush(Qt::NoBrush);
    painter.setPen(Qt::red);

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
    QPoint crossPoint;
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
            crossPoint=QPoint(
                        (r2*sin(a1)-r1*sin(a2))/(cos(a2)*sin(a1)-cos(a1)*sin(a2)),
                        (r2/cos(a2)-r1/cos(a1))/(tan(a2)-tan(a1))
                        );
            painter.setBrush(QBrush(Qt::red));
            painter.drawEllipse(crossPoint.x()-5,crossPoint.y()-5,10,10);
        }
    }

    painter.end();

    // Update info
    if(m_analysisResultsMutex.tryLock())
    {
        m_crossFound=crossFound;
        m_crossPoint=QPoint();
        m_analysisResultsMutex.unlock();
    }

    emit updateImage(imgnew);
    emit foundCross(avgAngle);
}

void SwissHCCAnalysis::run()
{
    m_imageAnalysisState=None;

    getECS02()->moveLoad();
    getECS02()->waitForIdle();

    emit stepMoveToLoadDone();
}

void SwissHCCAnalysis::runCalibration(const QList<QPoint>& validSlots)
{
    QMutex waitMutex; waitMutex.lock();
    m_validSlots=validSlots;

    emit message(tr("POSITION CALIBRATION"));

    getECS02()->moveAbsolute(0, 20);
    getECS02()->waitForIdle();

    emit message(tr("POSITION ALIGNMENT"));

    m_imageAnalysisState=FindGroove;
    bool foundEdge=false;
    for(uint i=0;i<10;i++)
    {
        QThread::msleep(200);
        m_waitForAnalyze.wait(&waitMutex);

        m_analysisResultsMutex.lock();
        foundEdge=m_edgeFound;
        m_analysisResultsMutex.unlock();
        if(foundEdge) break;

        getECS02()->moveIncrement(0,-int(1./getECS02()->getIncrementY()));
        getECS02()->waitForIdle();
    }

    if(!foundEdge)
    {
        waitMutex.unlock();
        emit message("EDGE NOT FOUND");
        emit finished();
        return;
    }

    emit message("EDGE FOUND. FINDING CROSS");

    //
    // Find the cross!
    m_imageAnalysisState=FindGrooveCross;
    bool foundCross=false;
    for(uint i=0;i<30;i++)
    {
        QThread::msleep(200);
        m_waitForAnalyze.wait(&waitMutex);

        m_analysisResultsMutex.lock();
        foundCross=m_crossFound;
        m_analysisResultsMutex.unlock();
        if(foundCross) break;

        getECS02()->moveIncrement(int(1./getECS02()->getIncrementX()),0);
        getECS02()->waitForIdle();
    }

    if(!foundCross)
    {
        waitMutex.unlock();
        emit message("CROSS NOT FOUND");
        emit finished();
        return;
    }

    waitMutex.unlock();
    emit message("CROSS FOUND. ROTATE!");
    emit stepCrossFound();
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

void SwissHCCAnalysis::runFindChip()
{ }
