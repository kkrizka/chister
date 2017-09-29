#include "V4L2FrameGrabber.h"

#include <QMutex>
#include <QDebug>
#include <QPainter>

#include <linux/videodev2.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>


static int xioctl(int fd, int request, void *arg)
{
  int r;

  do r = ioctl (fd, request, arg);
  while (-1 == r && EINTR == errno);

  return r;
}

FrameGrabber::FrameGrabber(QObject *parent)
  : QObject(parent),
    m_fd(-1)
{
  // Open the video capture card frame descriptor
  m_fd = open("/dev/video0", O_RDWR);
  if(m_fd==-1)
    {
      perror("Opening video device");
      return;
    }

  // Test the capture card
  struct v4l2_capability caps = {0};
  if(xioctl(m_fd, VIDIOC_QUERYCAP, &caps)==-1)
    {
      perror("Querying Capabilities");
      return;
    }

  qInfo() << "Device /dev/video0 information...";
  qInfo() << " Driver:" << (char*)caps.driver;
  qInfo() << " Card:" << (char*)caps.card;
  qInfo() << " Bus Info:" << (char*)caps.bus_info;

  struct v4l2_input input = {0};
  input.index=1;
  if(xioctl(m_fd, VIDIOC_ENUMINPUT, &input)==-1)
    {
      perror("Querying Inputs");
      return;
    }
  qInfo() << "Input information...";
  qInfo() << " Index:" << input.index;
  qInfo() << " Name:" << (char*)input.name;
  

  int index=1;
  if(xioctl(m_fd, VIDIOC_S_INPUT, &index)==-1)
    {
      perror("Setting Input");
      return;
    }

  struct v4l2_format fmt = {};
  fmt.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
  if(xioctl(m_fd, VIDIOC_G_FMT, &fmt)==-1)
    {
      perror("Querying Format");
      return;
    }

  char fourcc[5] = {0};
  strncpy(fourcc, (char *)&fmt.fmt.pix.pixelformat, 4);
  qInfo() << "Format information...";
  qInfo() << " Width:" << fmt.fmt.pix.width;
  qInfo() << " Height:" << fmt.fmt.pix.height;
  qInfo() << " Pixel Format:" << fourcc;

  m_width=fmt.fmt.pix.width;
  m_height=fmt.fmt.pix.height;
  m_pixelformat=fmt.fmt.pix.pixelformat;

  // Request buffers for streaming
  struct v4l2_requestbuffers req = {0};
  req.count = 1;
  req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  req.memory = V4L2_MEMORY_MMAP;
 
  if(xioctl(m_fd, VIDIOC_REQBUFS, &req)==-1)
    {
      perror("Requesting Buffer");
      return;
    }

  struct v4l2_buffer buf = {0};
  buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  buf.memory = V4L2_MEMORY_MMAP;
  buf.index = 0;
  if(xioctl(m_fd, VIDIOC_QUERYBUF, &buf)==-1)
    {
      perror("Querying Buffer");
      return;
    }

  m_buffer = (uint8_t*)mmap(NULL, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, m_fd, buf.m.offset);
  qInfo() << "Buffer information...";
  qInfo() << " Length:" << buf.length;
  qInfo() << " Address:" << m_buffer;
  qInfo() << " Image Length:" << buf.bytesused;

  // Prepare acquisition timer
  m_cameraTimer=new QTimer(this);
  connect(m_cameraTimer, &QTimer::timeout, this, &FrameGrabber::updateCamera);
}

const QImage& FrameGrabber::getImage(bool update)
{
  if(update)
    {
      QMutex waitMutex;
      waitMutex.lock();
      m_waitForNew.wait(&waitMutex);
      waitMutex.unlock();
    }
  return m_img;
}

void FrameGrabber::startAcquisition()
{
  // Start acquitision timer
  m_cameraTimer->start(100);
}

void FrameGrabber::stopAcquisition()
{
  // Start acquitision timer
  m_cameraTimer->stop();
  emit finished();
}

void FrameGrabber::updateCamera()
{
  // Start capture
  struct v4l2_buffer buf = {0};
  buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  buf.memory = V4L2_MEMORY_MMAP;
  buf.index = 0;
  if(xioctl(m_fd, VIDIOC_QBUF, &buf)==-1)
    {
      perror("Query Buffer");
      return;
    }

  if(xioctl(m_fd, VIDIOC_STREAMON, &buf.type)==-1)
    {
      perror("Start Capture");
      return;
    }
 
  fd_set fds;
  FD_ZERO(&fds);
  FD_SET(m_fd, &fds);
  struct timeval tv = {0};
  tv.tv_sec = 2;
  int r = select(m_fd+1, &fds, NULL, NULL, &tv);
  if(r==-1)
    {
      perror("Waiting for Frame");
      return;
    }

  if(xioctl(m_fd, VIDIOC_DQBUF, &buf)==-1)
    {
      perror("Retrieving Frame");
      return;
    }

  m_img=QImage(m_buffer, m_width, m_height, QImage::Format_RGB888);

  m_waitForNew.wakeAll();
  emit newImage(m_img);
}
