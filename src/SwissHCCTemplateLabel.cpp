#include "SwissHCCTemplateLabel.h"

#include <QFileDialog>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

SwissHCCTemplateLabel::SwissHCCTemplateLabel(QWidget *parent)
  : QLabel(parent)
{}

SwissHCCTemplateLabel::~SwissHCCTemplateLabel() { }

void SwissHCCTemplateLabel::setX(int x)
{ m_x=x; }

int SwissHCCTemplateLabel::getX()
{ return m_x; redraw(); }

void SwissHCCTemplateLabel::setY(int y)
{ m_y=y; }

int SwissHCCTemplateLabel::getY()
{ return m_y; redraw(); }

void SwissHCCTemplateLabel::setImage(const QImage& image)
{ m_image=image; redraw(); }

QImage& SwissHCCTemplateLabel::getImage()
{ return m_image; }

void SwissHCCTemplateLabel::mouseDoubleClickEvent(QMouseEvent *event)
{
  QString fileName = QFileDialog::getOpenFileName(this,
						  "Save Image", "",
						  "Images (*.png *.xpm *.jpg *.bmp);;All Files (*)");
  if(fileName.isEmpty()) return;
  m_image=QImage(fileName);
  redraw();
}

void SwissHCCTemplateLabel::mousePressEvent(QMouseEvent *event)
{
  m_x=event->x();
  m_y=event->y();
  redraw();
}

void SwissHCCTemplateLabel::redraw()
{
  if(!text().isEmpty()) m_nonImageText=text(); // save the non image text
  if(m_image.isNull())
    {
      setPixmap(QPixmap());
      setText(m_nonImageText);
      return;
    }
  
  QPixmap pix(QPixmap::fromImage(m_image));
  QPainter painter(&pix);
  painter.setBrush(QBrush(Qt::red));
  painter.drawEllipse(m_x-2,m_y-2,4,4);
  setPixmap(pix);
}
