#include "ImageScanAnalysis.h"

#include <QDebug>
#include <QPainter>

#include <opencv2/opencv.hpp>
#include <iostream>

ImageScanAnalysis::ImageScanAnalysis(FrameGrabber *frameGrabber, ECS02 *ecs02, QObject *parent)
  : AnalysisProgram(frameGrabber, ecs02, parent)
{ }

void ImageScanAnalysis::run()
{ }

void ImageScanAnalysis::runCalibrate()
{
  emit startCalibrate();

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
      getStage()->updateInfo();
      getStage()->moveIncrement(0, 10);
      getStage()->waitForIdle();
      py=getStage()->getY();
      QImage img=getFrameGrabber()->getImage(true);
      QThread::msleep(100);

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

	  emit stepCalibrate(dmm/dpix);

	  scale+=dmm/dpix;
	  n++;
	}
    }

  m_scale=fabs(scale/n);
  qInfo() << "Calculated scale as " << m_scale;
  emit doneCalibrate(m_scale);

  runScan();
}

void ImageScanAnalysis::runScan()
{
  emit startScan();

  // Figure out the number of steps necessary
  uint ysteps=51./m_scale/240; // need 51./m_scale pixels to cover 2 in, 240 pixels per frame
  uint xsteps=51./m_scale/400; // need 51./m_scale pixels to cover 2 in, 400 pixels per frame

  //ysteps/=4;
  //xsteps/=2;

  QImage result(xsteps*400,ysteps*240,QImage::Format_Grayscale8);
  for(uint ix=0;ix<xsteps;ix++)
    {
      for(uint iy=0;iy<ysteps;iy++)
	{
	  getStage()->moveAbsolute(-26.+iy*240*m_scale ,-19+ix*400*m_scale);
	  getStage()->waitForIdle();

	  QImage frame=getFrameGrabber()->getImage(true).copy(120,120,400,240);
	  QPainter painter(&result);
	  painter.drawImage(ix*400,iy*240, frame);
	  painter.end();
	  frame.save("frame_x"+QString::number(ix)+"_y"+QString::number(iy)+".png");
	  emit stepScan(result);
	}
    }

  result.save("scan.png");

  //
  // Find the chips
  cv::Mat cvimg(result.height(), result.width(), CV_8UC1 , result.bits(), result.bytesPerLine());
  
  cv::Mat cvimg_threshold;
  cv::threshold(cvimg,cvimg_threshold,50,255,cv::THRESH_BINARY);

  std::vector<std::vector<cv::Point> > contours;
  std::vector<cv::Vec4i> hierarchy;

  /// Find contours
  cv::findContours( cvimg_threshold, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );


  cv::RNG rng(12345);
  cv::Mat img_contours;
  cv::cvtColor(cvimg, img_contours, cv::COLOR_GRAY2BGR);
  uint chipIdx=0;
  m_chipPos.clear();
  for(const auto& contour : contours)
    {
      chipIdx++;
      double area=cv::contourArea(contour);
      if(area<10000) continue;
      QPoint min(result.width(),result.height()),max(0,0);
      for(const auto& p : contour)
	{
	  if(p.x<min.x()) min.setX(p.x);
	  if(p.y<min.y()) min.setY(p.y);
	  if(max.x()<p.x) max.setX(p.x);
	  if(max.y()<p.y) max.setY(p.y);
	}
      QPoint avg=(min+max)/2;
      m_chipPos.append(avg);

      //
      cv::Scalar color = cv::Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
      cv::drawContours( img_contours, contours, chipIdx-1, color, 2, 8, hierarchy, 0, cv::Point() );
      cv::circle( img_contours, cv::Point(avg.x(),avg.y()), 10, color, 5);
    }
  qInfo() << "Found" << m_chipPos.size() << "chips!";
  cv::imwrite("contours.png", img_contours);

  emit doneScan(m_chipPos.size());

  runPictures();
}

void ImageScanAnalysis::runPictures()
{
  emit startPictures();

  uint chipidx=0;
  for(const auto& chipPos : m_chipPos)
    {
      qInfo() << "Find chip" << chipidx << chipPos;
      getStage()->moveAbsolute(-26.+(chipPos.y()-120)*m_scale ,-19.+(chipPos.x()-120)*m_scale);
      getStage()->waitForIdle();
      QThread::msleep(100);
      QImage chip=getFrameGrabber()->getImage(true);
      chip.save("chip_"+QString::number(chipidx)+".png");
      chipidx++;
    }

  emit donePictures();
  done();
}

void ImageScanAnalysis::done()
{ emit finished(); }
