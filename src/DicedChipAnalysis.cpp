#include "DicedChipAnalysis.h"

#include <QDebug>
#include <QThread>
#include <QLabel>
#include <QPainter>
#include <QProcess>

#include <math.h>

#include "QOpenCVHelpers.h"

DicedChipAnalysis::DicedChipAnalysis(FrameGrabber *frameGrabber, ECS02 *ecs02, QObject *parent)
    : AnalysisProgram(frameGrabber, ecs02, parent), m_imageAnalysisState(None)
{ }

void DicedChipAnalysis::setChipTemplate(const DicedChipTemplate& chipTemplate)
{ m_chipTemplate=chipTemplate; }

void DicedChipAnalysis::setLogDirectory(const QString& logDirectory)
{
  m_logDirectory=logDirectory;

  if(!m_logDirectory.isEmpty())
    {
      m_logFH.setFileName(m_logDirectory+"/output.log");
      m_logFH.open(QIODevice::WriteOnly | QIODevice::Text);
      m_log.setDevice(&m_logFH);
    }
}

void DicedChipAnalysis::setValidSlots(const QList<DicedChipSlot*>& validSlots)
{
  m_validSlots=validSlots;
}

void DicedChipAnalysis::settingsSave(QSettings *settings)
{
  settings->setValue("DicedChip/crossPoint_x",m_crossPoint.x());
  settings->setValue("DicedChip/crossPoint_y",m_crossPoint.y());

  settings->setValue("DicedChip/probesOffset_x",m_probesOffsetX);
  settings->setValue("DicedChip/probesOffset_y",m_probesOffsetY);

  settings->setValue("DicedChip/workingArea_x"     ,m_workingAreaX     );
  settings->setValue("DicedChip/workingArea_y"     ,m_workingAreaY     );
  settings->setValue("DicedChip/workingArea_width" ,m_workingAreaWidth );
  settings->setValue("DicedChip/workingArea_height",m_workingAreaHeight);
}

void DicedChipAnalysis::settingsLoad(QSettings *settings)
{
  m_crossPoint.setX(settings->value("DicedChip/crossPoint_x",0.).toFloat());
  m_crossPoint.setY(settings->value("DicedChip/crossPoint_y",0.).toFloat());

  m_probesOffsetX=settings->value("DicedChip/probesOffset_x",0.).toFloat();
  m_probesOffsetY=settings->value("DicedChip/probesOffset_y",0.).toFloat();

  m_workingAreaX     =settings->value("DicedChip/workingArea_x"     ,0)  .toUInt();
  m_workingAreaY     =settings->value("DicedChip/workingArea_y"     ,0)  .toUInt();
  m_workingAreaWidth =settings->value("DicedChip/workingArea_width" ,640).toUInt();
  m_workingAreaHeight=settings->value("DicedChip/workingArea_height",480).toUInt();
}

void DicedChipAnalysis::logStatus(const QString& message)
{
  if(m_log.device()!=0) m_log << message << "\n";
  emit status(message);
}

std::vector<cv::Vec2f> DicedChipAnalysis::findLines(const QImage& img) const
{
  QImage newimg=img;
  cv::Mat cvimg(img.height(), img.width(), CV_8UC1 , newimg.bits(), img.bytesPerLine());

  // Find edges
  cv::Mat imgpass;
  cv::threshold(cvimg, imgpass, 90, 255, cv::THRESH_BINARY_INV);

  cv::Mat imgedges;
  cv::Canny(imgpass, imgedges, 50, 150);

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

std::vector<cv::Vec2f> DicedChipAnalysis::findGrooves(const QImage& img) const
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
	  if(fabs(dist-0.5)>0.05) continue; // Not 1mm apart

	  candidates.push_back(cv::Vec2f((l1[0]+l2[0])/2.,l1[1]));
        }
    }

  return candidates;
}

double DicedChipAnalysis::calculateEmptyInArea(const cv::Mat& img, const cv::Rect& rect) const
{
  cv::Mat subimg=img(rect);
  int nonzero=countNonZero(subimg);
  int size=subimg.total();
  return ((double)nonzero)/((double)size);
}

void DicedChipAnalysis::analyze(const QImage& img)
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

void DicedChipAnalysis::analyzeFindProbes(const QImage& img)
{
  bool inWorkThread=QThread::currentThread()==thread();

  QImage newimg=img;
  cv::Mat cvimg(img.height(), img.width(), CV_8UC1 , newimg.bits(), img.bytesPerLine());
  cv::Mat imgpass;
  cv::threshold(cvimg, imgpass, 50, 255, cv::THRESH_BINARY_INV);

  //
  // Probe position finding using template match
  cv::Mat result;
  cv::matchTemplate(imgpass,m_chipTemplate.cvProbesImage(),result,cv::TM_CCOEFF_NORMED);

  double min, max;
  cv::Point min_loc, max_loc;
  cv::minMaxLoc(result, &min, &max, &min_loc, &max_loc);
  //qInfo() << "Found maximum of" << max << "at" << max_loc.x << "" << max_loc.y;

  //
  // Empty area finding using look for biggest rectangle with shiny

  cv::Rect workingArea(cvimg.size().width/2-10,cvimg.size().height/2-10,20,20);

  bool growpx=true,growmx=true,growpy=true,growmy=true;
  double rat=0;
  while(growpx || growmx || growpy || growmy)
    {
      // Grow in +x direction
      if(growpx) { workingArea.width++; }
      rat=calculateEmptyInArea(imgpass, workingArea);
      if(rat>0.01) 
	{ 
	  workingArea.width--;
	  growpx=false;
	}

      // Grow in -x direction
      if(growmx) { workingArea.x--; workingArea.width++; }
      rat=calculateEmptyInArea(imgpass, workingArea);
      if(rat>0.01) 
	{
	  workingArea.x++;
	  workingArea.width--;
	  growmx=false;
	}

      // Grow in +y direction
      if(growpy) { workingArea.height++; }
      rat=calculateEmptyInArea(imgpass, workingArea);
      if(rat>0.01) 
	{ 
	  workingArea.height--;
	  growpy=false;
	}

      // Grow in -y direction
      if(growmy) { workingArea.y--; workingArea.height++; }
      rat=calculateEmptyInArea(imgpass, workingArea);
      if(rat>0.01) 
	{
	  workingArea.y++;
	  workingArea.height--;
	  growmy=false;
	}
    }

  // 20 pixel buffer on each side
  workingArea.x+=20;
  workingArea.y+=20;
  workingArea.width-=40;
  workingArea.height-=40;

  //
  // Draw/save results
  if(!inWorkThread)
    {
      QImage imgnew=img.convertToFormat(QImage::Format_RGB32);
      QPainter painter(&imgnew);

      // Draw a box for the matched template
      painter.setBrush(Qt::NoBrush);
      if(0<max && max < 0.3)
	painter.setPen(Qt::red);
      else if(0.3<max && max < 0.9)
	painter.setPen(Qt::yellow);
      else
	painter.setPen(Qt::green);

      painter.drawRect(max_loc.x,max_loc.y,m_chipTemplate.cvProbesImage().cols,m_chipTemplate.cvProbesImage().rows);

      // Draw a point for the reference point
      painter.setBrush(QBrush(Qt::red));
      painter.drawEllipse(QPoint(max_loc.x-2,max_loc.y-2)+m_chipTemplate.probesOffset(),4,4);


      // Draw the working area
      painter.setBrush(Qt::NoBrush);
      painter.setPen(Qt::blue);

      painter.drawRect(workingArea.x,workingArea.y,workingArea.width,workingArea.height);

      painter.end();

      emit updateImage(imgnew);
    }
  else
    {
      m_probesOffsetScore=max;
      m_probesOffsetX=max_loc.x*0.0076;
      m_probesOffsetY=max_loc.y*0.0076;

      m_workingAreaX     =workingArea.x;
      m_workingAreaY     =workingArea.y;
      m_workingAreaWidth =workingArea.width;
      m_workingAreaHeight=workingArea.height;
    }
}

void DicedChipAnalysis::analyzeFindGroove(const QImage& img)
{
  bool inWorkThread=QThread::currentThread()==thread();

  QImage imgnew=img.convertToFormat(QImage::Format_RGB32);
  QPainter painter(&imgnew);
  painter.setBrush(Qt::NoBrush);
  painter.setPen(Qt::red);

  painter.drawRect(m_workingAreaX,m_workingAreaY,m_workingAreaWidth,m_workingAreaHeight);
  QImage imgArea=img.copy(m_workingAreaX,m_workingAreaY,m_workingAreaWidth,m_workingAreaHeight);

  std::vector<cv::Vec2f> candidates=findGrooves(imgArea);
  float r=0,c=0,s=0,theta=0;
  for(auto& line : candidates)
    {
      line[0]+=m_workingAreaX*cos(line[1]);
      line[0]+=m_workingAreaY*sin(line[1]);
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
      m_edgeRadius=r;
      m_edgeAngle=theta;
    }
  else
    emit updateImage(imgnew);
}

void DicedChipAnalysis::analyzeFindGrooveCross(const QImage& img)
{
  bool inWorkThread=QThread::currentThread()==thread();

  QImage imgArea=img.copy(m_workingAreaX,m_workingAreaY,m_workingAreaWidth,m_workingAreaHeight);

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
      m_crossPoint=crossPoint-QPoint(img.width(),img.height())/2;
    }
  else
    {
      emit updateImage(imgnew);
      emit foundCross(avgAngle);
    }
}

void DicedChipAnalysis::analyzeAlignChip(const QImage& img)
{
    bool inWorkThread=QThread::currentThread()==thread();

    QImage newimg=img;
    cv::Mat cvimg(img.height(), img.width(), CV_8UC1 , newimg.bits(), img.bytesPerLine());

    cv::Mat result;
    cv::matchTemplate(cvimg,m_chipTemplate.cvChipImage(),result,cv::TM_CCOEFF_NORMED);

    double min, max;
    cv::Point min_loc, max_loc;
    cv::minMaxLoc(result, &min, &max, &min_loc, &max_loc);
    //qInfo() << "Found maximum of" << max << "at" << max_loc.x << "" << max_loc.y;

    if(!inWorkThread)
    {
        QImage imgnew=img.convertToFormat(QImage::Format_RGB32);
        QPainter painter(&imgnew);
        painter.setBrush(Qt::NoBrush);
        if(0<max && max < 0.1)
            painter.setPen(Qt::red);
        else if(0.2<max && max < 0.7)
            painter.setPen(Qt::yellow);
        else
            painter.setPen(Qt::green);

        painter.drawRect(max_loc.x,max_loc.y,m_chipTemplate.cvChipImage().cols,m_chipTemplate.cvChipImage().rows);

        painter.end();

        emit chipFound(max);
        emit updateImage(imgnew);
    }
    else
    {
        m_chipOffsetScore=max;
        m_chipOffsetX=max_loc.x*0.0076;
        m_chipOffsetY=max_loc.y*0.0076;
    }
}

void DicedChipAnalysis::run()
{ }

void DicedChipAnalysis::runLoadChips()
{
  emit message(tr("MOVING TO LOAD POSITION"));
  logStatus("LOADING CHIPS");
  m_imageAnalysisState=None;

  getStage()->moveLoad();
  getStage()->waitForIdle();

  emit donePrepareLoadChips();
}

void DicedChipAnalysis::runFindProbes()
{
  emit message(tr("FINDING PROBES"));
  logStatus("FINDING PROBES");

  getStage()->moveAbsolute(0, -24);
  getStage()->waitForIdle();

  m_imageAnalysisState=FindProbes;
  QThread::msleep(500);
  const QImage &img=getFrameGrabber()->getImage(true);
  img.save("findprobes.png");
  analyzeFindProbes(img);

  if(m_probesOffsetScore<0.5)
    {
      logStatus(QString("Cannot find probes. Best score is %1.").arg(m_probesOffsetScore));
      emit message(tr("CANNOT FIND PROBES!"));
      return;
    }
  logStatus(QString("Found probes with score %1 at %2,%3").arg(m_probesOffsetScore).arg(m_probesOffsetX).arg(m_probesOffsetY));
  emit message(tr("PROBES FOUND"));
  m_imageAnalysisState=None;

  emit doneFindProbes();
}

void DicedChipAnalysis::runCalibratePosition()
{
  emit message(tr("POSITION CALIBRATION"));
  logStatus("FINDING POSITION CALIBRATION GROOVE");

  getStage()->moveAbsolute(-2.2, -27);
  getStage()->waitForIdle();

  // Find the groove
  m_imageAnalysisState=FindGroove;
  for(uint i=0;i<20;i++)
    {
      QThread::msleep(200);
      QImage img=getFrameGrabber()->getImage(true);
      analyzeFindGroove(img);

      if(m_edgeFound) break;

      getStage()->moveIncrement(0,-int(1./getStage()->getIncrementY()));
      getStage()->waitForIdle();
    }

  if(!m_edgeFound)
    {
      emit message("EDGE NOT FOUND");
      logStatus("No edge found.");
      return;
    }

  // Move the edge away from the probes
  getStage()->moveIncrement(0,-int((230-m_edgeRadius*cos(m_edgeAngle))*0.0076/getStage()->getIncrementY()));
  getStage()->waitForIdle();

  // Report on status
  emit message("EDGE FOUND. FINDING CROSS");
  logStatus("Edge found.");

  //
  // Find the cross!
  m_imageAnalysisState=FindGrooveCross;
  for(uint i=0;i<10;i++)
    {
      QThread::msleep(200);
      QImage img=getFrameGrabber()->getImage(true);
      analyzeFindGrooveCross(img);
      if(m_crossFound) break;

      getStage()->moveIncrement(int(cos(m_edgeAngle)/getStage()->getIncrementX()),-int(sin(m_edgeAngle)/getStage()->getIncrementY()));
      getStage()->waitForIdle();
    }

  if(!m_crossFound)
    {
      emit message("CROSS NOT FOUND");
      logStatus("No cross found.");
    }
  else
    {
      emit message("CROSS FOUND. ROTATE!");
      logStatus(QString("Initial cross found at %1,%2.").arg(m_crossPoint.x()).arg(m_crossPoint.y()));
    }
  emit doneFindCross();
}

void DicedChipAnalysis::runCrossTest()
{
    logStatus("RUNNING CROSS TEST");
    //
    // Get current position
    QImage img=getFrameGrabber()->getImage(true);
    std::vector<cv::Vec2f> candidates=findGrooves(img);

    getStage()->updateInfo();
    getStage()->waitForIdle();
    float oldX=getStage()->getX();
    float oldLX=0.;
    for(const auto& candidate : candidates)
    {
        if(fabs(cos(candidate[1]))>0.5)
        {
            oldLX=candidate[0]/cos(candidate[1]);
            break;
        }
    }

    //
    // Move far away (200 mm)
    getStage()->moveIncrement(-int(200./getStage()->getIncrementX()),0);
    getStage()->waitForIdle();

    //
    // Get new position
    img=getFrameGrabber()->getImage(true);
    candidates=findGrooves(img);

    getStage()->updateInfo();
    getStage()->waitForIdle();
    float newX=getStage()->getX();
    float newLX=0.;
    for(const auto& candidate : candidates)
    {
        if(fabs(cos(candidate[1]))<0.5)
        {
            newLX=candidate[0]/sin(candidate[1]);
            break;
        }
    }
    float angle=atan2((oldX-newX)/0.0076,oldLX-newLX);

    //
    // Move back
    getStage()->moveIncrement(+int(200./getStage()->getIncrementX()),0);
    getStage()->waitForIdle();

    emit testCrossAngle(angle);
    logStatus(QString("Measured angle as %1").arg(angle));
}

void DicedChipAnalysis::runCrossSave()
{
    //
    // Get final position
    QImage img=getFrameGrabber()->getImage(true);
    analyzeFindGrooveCross(img);

    getStage()->updateInfo();
    getStage()->waitForIdle();

    logStatus(QString("Final cross position is %1,%2.").arg(m_crossPoint.x()).arg(m_crossPoint.y()));
    m_crossPoint=m_crossPoint*0.0076+QPointF(getStage()->getY(),getStage()->getX());
    logStatus(QString("Absolute cross position is %1,%2.").arg(m_crossPoint.x()).arg(m_crossPoint.y()));

    runFindChips();
}

void DicedChipAnalysis::runFindChips()
{
  m_imageAnalysisState=None;

  emit startFindChips();

  //
  //  Find and move to the next slot
  if(m_validSlots.size()>0) // Automatic movement to the next needed chip
    {
      // Find the next untested pointer
      QList<DicedChipSlot*>::Iterator validSlotsIter;
      for(validSlotsIter=m_validSlots.begin(); validSlotsIter!=m_validSlots.end(); validSlotsIter++)
	{
	  if((*validSlotsIter)->m_status==DicedChipSlot::Misaligned) continue; // Already checked and found to be misaligned
	  if(!(*validSlotsIter)->position().isNull()) continue; // Already checked and aligned
	  break; // Unchecked chip!
	}

      if(validSlotsIter!=m_validSlots.end()) // Found an unchecked chip, move there
	{
	  runFindChip(*validSlotsIter);
	}
      else // All chips checked, done...
	{
	  emit doneFindChips();
	}
    }
  else // No chips defined, just move to slot 1
    runFindChip(new DicedChipSlot(0,0,this));
}

void DicedChipAnalysis::runFindChip(DicedChipSlot *slot)
{

  logStatus(QString("-- CHIP TEST %1,%2 --").arg(slot->slot().first).arg(slot->slot().second));

  emit startFindChip(slot);
  m_activeSlot=slot;

  m_imageAnalysisState=None;
  QPointF chipPos=m_crossPoint // reference to the cross
    +QPointF(34.675,0)
    -QPointF((((int)slot->slot().second)-2)*7.9375,
	     -(((int)slot->slot().first)-2)*6.35+3.175) // Center the hole
    +QPointF(0.3,0.5); // offset slightly to expose chip corner
  getStage()->moveAbsolute(chipPos.y(),chipPos.x());
  getStage()->waitForIdle();
  QThread::sleep(1);
  runAlignChip();
}

void DicedChipAnalysis::runAlignChip()
{
  m_imageAnalysisState=AlignChip;

  QImage img=getFrameGrabber()->getImage(true);
  analyzeAlignChip(img);

  if(m_chipOffsetScore>0.7)
    {
      getStage()->updateInfo();
      getStage()->waitForIdle();
      QPointF newPos=QPointF(getStage()->getY(),getStage()->getX())
	+QPointF(m_chipOffsetX,m_chipOffsetY)
	+QPointF(m_chipTemplate.chipOffset())*0.0076
	-QPointF(m_probesOffsetX,m_probesOffsetY)
	-QPointF(m_chipTemplate.probesOffset())*0.0076;
      getStage()->moveAbsolute(newPos.y(),newPos.x());
      getStage()->waitForIdle();
      logStatus(QString("Chip found with score %1 at position %2,%3.").arg(m_chipOffsetScore).arg(m_chipOffsetX).arg(m_chipOffsetY));
      emit chipAlignSuccess();
    }
  else if(m_chipOffsetScore<0.2)
    {
      logStatus(QString("No chip found. Score is %1.").arg(m_chipOffsetScore));
      emit chipAlignFailed();
    }
}

void DicedChipAnalysis::runChipSave()
{
  logStatus("Save chip position.");

  getStage()->updateState();
  getStage()->waitForIdle();

  m_activeSlot->setPosition(QPointF(getStage()->getX(), getStage()->getY()));
  emit chipUpdated(m_activeSlot);

  runFindChips();
}

void DicedChipAnalysis::runChipSkip()
{
  logStatus("Skip chip.");

  m_activeSlot->setPosition(QPointF(getStage()->getX(), getStage()->getY()));
  m_activeSlot->m_status=DicedChipSlot::Misaligned;
  emit chipUpdated(m_activeSlot);

  runFindChips();
}

void DicedChipAnalysis::runTestChips()
{
  qInfo() << "DicedChipAnalysis::runTestChips()";
  m_imageAnalysisState=None;

  //
  // Move to chip 1
  if(m_validSlots.size()>0)
    {
      // Find the next defined slot
      QList<DicedChipSlot*>::Iterator validSlotsIter;
      for(validSlotsIter=m_validSlots.begin(); validSlotsIter!=m_validSlots.end(); validSlotsIter++)
	{
	  qInfo() << "Check " << (*validSlotsIter)->slot().first << " " << (*validSlotsIter)->slot().second;
	  if((*validSlotsIter)->m_status!=DicedChipSlot::Untested) continue; // Already checked and found to be misaligned
	  if((*validSlotsIter)->position().isNull()) continue; // There is no position set...
	  
	  break; // Unchecked chip!
	}

      // Found a valid slot! Pause...
      if(validSlotsIter!=m_validSlots.end())
	{
	  m_activeSlot=(*validSlotsIter);
	  getStage()->moveAbsolute((*validSlotsIter)->position().x(),
				   (*validSlotsIter)->position().y());
	  getStage()->waitForIdle();
	  return;
	}
      qInfo() << "Nothing found..";
    }

  emit doneTestChips();
}

void DicedChipAnalysis::runChipTest()
{
  logStatus("Running chip test.");

  getStage()->separate(false);
  getStage()->separate(true);
  getStage()->separate(false);
  getStage()->waitForIdle();

  // Take picture
  QImage img=getFrameGrabber()->getImage();
  img.save(QString("chip_%1.png").arg(m_activeSlot->m_name));

  QString code=m_chipTemplate.m_script.arg(m_activeSlot->m_name);

  int ret=QProcess::execute(code);
  if(ret==0)
    m_activeSlot->m_status=DicedChipSlot::Pass;
  else
    m_activeSlot->m_status=DicedChipSlot::Fail;
  emit chipUpdated(m_activeSlot);

  getStage()->separate(true);
  getStage()->waitForIdle();

  runTestChips();
}

void DicedChipAnalysis::done()
{
  // Cleanup test status
  m_logDirectory=QString();
  for(DicedChipSlot* slot : m_validSlots) delete slot;
  m_validSlots.clear();

  // Log
  QFile m_logFH;
  QTextStream  m_log;
  m_logFH.close();

  // Testing state
  m_imageAnalysisState=None;

  emit finished();
}
