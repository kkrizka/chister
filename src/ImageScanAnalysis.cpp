#include "ImageScanAnalysis.h"

#include <QDebug>
#include <QThread>

#include <opencv2/opencv.hpp>
#include <iostream>

ImageScanAnalysis::ImageScanAnalysis(FrameGrabber *frameGrabber, ECS02 *ecs02, QObject *parent)
  : AnalysisProgram(frameGrabber, ecs02, parent)
{ }

void ImageScanAnalysis::run()
{ }

void ImageScanAnalysis::runCalibrate()
{
  cv::Mat ref_cvimg;

  cv::ORB orb;

  std::vector<cv::KeyPoint> ref_kp;
  cv::Mat ref_des;
  double ref_py;

  std::vector<cv::KeyPoint> kp;
  cv::Mat des;
  double py;

  double scale=0;
  uint n=0;

  cv::BFMatcher matcher;
  std::vector<cv::DMatch> matches;

  for(uint y=0; y<10;y++)
    {
      qInfo() << "y" << y;
      getStage()->updateInfo();
      getStage()->moveIncrement(0, 5);
      getStage()->waitForIdle();
      py=getStage()->getY();
      QImage img=getFrameGrabber()->getImage(true);

      // Find the keypoints and descriptors with ORB
      cv::Mat cvimg(img.height(), img.width(), CV_8UC1 , img.bits(), img.bytesPerLine());
      orb.detect(cvimg,kp);
      orb.compute(cvimg,kp,des);

      if(y==0)
	{
	  ref_cvimg=cvimg.clone();
	  ref_kp=kp;
	  ref_des=des.clone();
	  ref_py=py;
	}
      else
	{
	  matcher.match(des,ref_des,matches);

	  cv::DMatch best_match;
	  for(auto match : matches)
	    if(match.distance<best_match.distance || best_match.distance==0)
	      best_match=match;

	  // std::vector<cv::DMatch> best_matches;
	  // best_matches.push_back(best_match);
	  // cv::Mat img_matches;
	  // cv::drawMatches(cvimg, kp, ref_cvimg, ref_kp, best_matches, img_matches);
	  // cv::imwrite("matches.png", img_matches);

	  double dmm = py - ref_py;
	  double dpix=kp[best_match.queryIdx].pt.x-ref_kp[best_match.trainIdx].pt.x;

	  scale+=dmm/dpix;
	  n++;
	}

    }

  scale/=n;
  qInfo() << "Calculated scale as " << scale;

  //getFrameGrabber()->getImage(true).save(QString("test.png"));
}

void ImageScanAnalysis::done()
{ emit finished(); }
