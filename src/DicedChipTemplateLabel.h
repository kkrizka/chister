#ifndef DICEDCHIPTEMPLATELABEL_H_
#define DICEDCHIPTEMPLATELABEL_H_

#include <QLabel>
#include <QImage>

class DicedChipTemplateLabel : public QLabel
{
  Q_OBJECT

public:
  explicit DicedChipTemplateLabel(QWidget *parent);
  ~DicedChipTemplateLabel();

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

#endif // DICEDCHIPTEMPLATELABEL_H_
