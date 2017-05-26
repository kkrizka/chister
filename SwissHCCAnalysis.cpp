#include "SwissHCCAnalysis.h"

#include <QDebug>
#include <QThread>
#include <QLabel>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>

#include <math.h>

SwissHCCAnalysis::SwissHCCAnalysis(FrameGrabber *frameGrabber, ECS02 *ecs02, QObject *parent)
    : AnalysisProgram(frameGrabber, ecs02, parent)
{ }

void SwissHCCAnalysis::run()
{

    getECS02()->moveLoad();
    getECS02()->waitForIdle();

    emit stepMoveToLoadDone();
}

void SwissHCCAnalysis::runCalibration(const QList<QPoint>& validSlots)
{
    m_validSlots=validSlots;

    emit message(tr("POSITION CALIBRATION"));

    getECS02()->moveAbsolute(0, 20);
    getECS02()->waitForIdle();

    emit message(tr("POSITION ALIGNMENT"));

    bool foundEdge=false;
    float vedgeAngle=0.;
    for(uint i=0;i<10;i++)
    {
        QThread::msleep(200);
        QImage img=getFrameGrabber()->getImage(false);

        cv::Mat cvimg(img.height(), img.width(), CV_8UC1 , img.bits(), img.bytesPerLine());

        // Apply a treshhold
        cv::Mat imgpass;
        cv::threshold(cvimg, imgpass, 100, 255, cv::THRESH_BINARY);

        cv::Mat imgedges;
        cv::Canny(imgpass,imgedges, 100, 200);

        std::vector<cv::Vec2f> lines;
        cv::HoughLines(imgedges,lines,1,CV_PI/180,100);

        if(lines.size()>2)
        { // Check possible lines
            for(uint l1idx=0;l1idx<lines.size()-1;l1idx++)
            {
                const cv::Vec2f &l1=lines[l1idx];
                for(uint l2idx=l1idx+1;l2idx<lines.size();l2idx++)
                {
                    const cv::Vec2f &l2=lines[l2idx];
                    if(l1[1]!=l2[1]) continue;

                    float dist=fabs(l2[0]-l1[0])*0.0076;
                    if(fabs(dist-1)<0.05)
                    {
                        vedgeAngle=l1[1];
                        foundEdge=true;
                        break;
                    }
                }
                if(foundEdge) break;
            }
            if(foundEdge) break;
        }

        /*std::stringstream ss;
        ss << "cv_" << i << ".png";
        cv::Mat colorimg;
        cv::cvtColor( imgpass, colorimg, CV_GRAY2BGR );
        for(auto line : lines)
        {
            float rho = line[0];
            float theta = line[1];
            double a = cos(theta), b = sin(theta);
            double x0 = a*rho, y0 = b*rho;
            cv::Point pt1(cvRound(x0 + 1000*(-b)),
                          cvRound(y0 + 1000*(a)));
            cv::Point pt2(cvRound(x0 - 1000*(-b)),
                          cvRound(y0 - 1000*(a)));
            cv::line( colorimg, pt1, pt2, cv::Scalar(0,0,255), 3, 8 );
        }
        cv::imwrite(ss.str(), colorimg);*/

        getECS02()->moveIncrement(0,-int(1./getECS02()->getIncrementY()));
        getECS02()->waitForIdle();
    }

    if(!foundEdge)
    {
        emit message("EDGE NOT FOUND");
        emit finished();
        return;
    }

    emit message("EDGE FOUND. FINDING CROSS");

    //
    // Find the cross!
    bool foundCross=false;
    for(uint i=0;i<30;i++)
    {
        QThread::msleep(200);
        QImage img=getFrameGrabber()->getImage(false);

        cv::Mat cvimg(img.height(), img.width(), CV_8UC1 , img.bits(), img.bytesPerLine());

        // Apply a treshhold
        cv::Mat imgpass;
        cv::threshold(cvimg, imgpass, 100, 255, cv::THRESH_BINARY);
        cv::Mat imgedges;
        cv::Canny(imgpass,imgedges, 100, 200);
        std::vector<cv::Vec2f> lines;
        cv::HoughLines(imgedges,lines,1,CV_PI/180,100);

        std::vector<cv::Vec2f> hlines;
        for(const auto& line : lines)
            if(fabs(cos(vedgeAngle-line[1]))<CV_PI/180) hlines.push_back(line);

        for(uint l1idx=0;hlines.size()>0 && l1idx<hlines.size()-1;l1idx++)
        {
            const cv::Vec2f &l1=hlines[l1idx];
            for(uint l2idx=l1idx+1;l2idx<hlines.size();l2idx++)
            {
                const cv::Vec2f &l2=hlines[l2idx];
                if(l1[1]!=l2[1]) continue;

                float dist=fabs(l2[0]-l1[0])*0.0076;
                if(fabs(dist-1)<0.05)
                {
                    foundCross=true;
                    break;
                }
            }
            if(foundCross) break;
        }
        if(foundCross) break;

        /*std::stringstream ss;
        ss << "cv_" << i << ".png";
        cv::Mat colorimg;
        cv::cvtColor( imgpass, colorimg, CV_GRAY2BGR );
        for(auto line : lines)
        {
            float rho = line[0];
            float theta = line[1];
            double a = cos(theta), b = sin(theta);
            double x0 = a*rho, y0 = b*rho;
            cv::Point pt1(cvRound(x0 + 1000*(-b)),
                          cvRound(y0 + 1000*(a)));
            cv::Point pt2(cvRound(x0 - 1000*(-b)),
                          cvRound(y0 - 1000*(a)));
            //cv::line( colorimg, pt1, pt2, cv::Scalar(0,0,255), 3, 8 );
        }
        cv::imwrite(ss.str(), cvimg);*/

        if(!foundCross)
        {
            getECS02()->moveIncrement(int(1./getECS02()->getIncrementX()),0);
            getECS02()->waitForIdle();
        }
    }

    if(!foundCross)
    {
        emit message("CROSS NOT FOUND");
        emit finished();
        return;
    }

    emit message("CROSS FOUND. ROTATE!");

    emit stepCrossFound();
}

void SwissHCCAnalysis::analyzeCrossAngle(const QImage& img)
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

    //
    // Find edges of interest

    // Find candidates (parallel and 1 mm apart)
    qInfo() << "START";
    std::vector<double> candidateAngles;
    for(uint l1idx=0;l1idx<lines.size()-1;l1idx++)
    {
        const cv::Vec2f &l1=lines[l1idx];
        for(uint l2idx=l1idx+1;l2idx<lines.size();l2idx++)
        {
            const cv::Vec2f &l2=lines[l2idx];
            if(l1[1]!=l2[1]) continue; // Not parallel

            float dist=fabs(l2[0]-l1[0])*0.0076;
            if(fabs(dist-1)>0.05) continue; // Not 1mm apart

            double angle=l1[1];

            qInfo() << "Candidate found at" <<  angle*180./CV_PI;
            if(CV_PI   /4.<angle && angle<CV_PI*3./4.) angle-=CV_PI   /2.;
            if(CV_PI*3./4.<angle && angle<CV_PI*5./4.) angle-=CV_PI;
            if(CV_PI*5./4.<angle && angle<CV_PI*7./4.) angle-=CV_PI*3./2.;
            if(CV_PI*7./4.<angle && angle<CV_PI*9./4.) angle-=CV_PI*2;
            candidateAngles.push_back(angle);
            qInfo() << " actually" <<  angle*180./CV_PI;
        }
    }

    float avgAngle=0;
    for(auto angle : candidateAngles) avgAngle+=angle/candidateAngles.size();

    emit foundCross(0,0,avgAngle);
}

void SwissHCCAnalysis::runCrossTest()
{
    getECS02()->moveIncrement(0,-int(200./getECS02()->getIncrementY()));
    getECS02()->waitForIdle();
    getECS02()->moveIncrement(0,+int(200./getECS02()->getIncrementY()));
    getECS02()->waitForIdle();
}

void SwissHCCAnalysis::runFindChip()
{ }
