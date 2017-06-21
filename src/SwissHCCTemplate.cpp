#include "SwissHCCTemplate.h"

void SwissHCCTemplate::setChipImage(const QImage& chipImage)
{
  m_qtChipImage=chipImage;
  m_cvChipImage=cv::Mat(m_qtChipImage.height(), m_qtChipImage.width(), CV_8UC1 , m_qtChipImage.bits(), m_qtChipImage.bytesPerLine());
}

void SwissHCCTemplate::setProbesImage(const QImage& probesImage)
{
  m_qtProbesImage=probesImage;
  m_cvProbesImage=cv::Mat(m_qtProbesImage.height(), m_qtProbesImage.width(), CV_8UC1 , m_qtProbesImage.bits(), m_qtProbesImage.bytesPerLine());
}

QPoint SwissHCCTemplate::chipOffset() const
{ return QPoint(m_chipX, m_chipY); }

QPoint SwissHCCTemplate::probesOffset() const
{ return QPoint(m_probesX, m_probesY); }
  
cv::Mat SwissHCCTemplate::cvChipImage() const
{ return m_cvChipImage; }

cv::Mat SwissHCCTemplate::cvProbesImage() const
{ return m_cvProbesImage; }

QImage SwissHCCTemplate::qtChipImage() const
{ return m_qtChipImage; }

QImage SwissHCCTemplate::qtProbesImage() const
{ return m_qtProbesImage; }
