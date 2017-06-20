#ifndef SWISSHCCTEMPLATELABEL_H_
#define SWISSHCCTEMPLATELABEL_H_

#include <QLabel>
#include <QImage>

class SwissHCCTemplateLabel : public QLabel
{
  Q_OBJECT

public:
  explicit SwissHCCTemplateLabel(QWidget *parent);
  ~SwissHCCTemplateLabel();

  void setX(int x);
  int getX();

  void setY(int y);
  int getY();

  void setImage(const QImage& image);
  QImage& getImage();

protected:
  void mouseDoubleClickEvent(QMouseEvent *event);
  void mousePressEvent(QMouseEvent *event);

private:
  QString m_nonImageText;

  QImage m_image;
  int m_x, m_y;

  void redraw();
};

#endif // SWISSHCCTEMPLATELABEL_H_
