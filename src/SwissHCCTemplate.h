#ifndef SWISSHCCTEMPLATE_H_
#define SWISSHCCTEMPLATE_H_

#include <QString>
#include <QImage>

#include <opencv2/opencv.hpp>

class SwissHCCTemplate
{
public:
  QString m_name;

  int m_chipX, m_chipY;
  int m_probesX, m_probesY;

  QPoint chipOffset() const;
  QPoint probesOffset() const;

  void setChipImage(const QImage& chipImage);
  void setProbesImage(const QImage& probesImage);
  
  cv::Mat cvChipImage() const;
  cv::Mat cvProbesImage() const;

  QImage qtChipImage() const;
  QImage qtProbesImage() const;

private:
  QImage m_qtChipImage;
  cv::Mat m_cvChipImage;

  QImage m_qtProbesImage;
  cv::Mat m_cvProbesImage;
};

#endif // SWISSHCCTEMPLATE_H_
