#ifndef SWISSHCCTEMPLATE_H_
#define SWISSHCCTEMPLATE_H_

#include <QString>
#include <QImage>

class SwissHCCTemplate
{
public:
  QString m_name;

  QImage m_chipImage;
  int m_chipX, m_chipY;

  QImage m_probesImage;
  int m_probesX, m_probesY;
};

#endif // SWISSHCCTEMPLATE_H_
