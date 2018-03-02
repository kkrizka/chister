#include "DicedChipTemplateLabel.h"

#include <QFileDialog>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

DicedChipTemplateLabel::DicedChipTemplateLabel(QWidget *parent)
  : QLabel(parent)
{}

DicedChipTemplateLabel::~DicedChipTemplateLabel() { }

void DicedChipTemplateLabel::setX(int x)
{ m_x=x; }

int DicedChipTemplateLabel::getX()
{ return m_x; redraw(); }

void DicedChipTemplateLabel::setY(int y)
{ m_y=y; }

int DicedChipTemplateLabel::getY()
{ return m_y; redraw(); }

void DicedChipTemplateLabel::setImage(const QImage& image)
{ m_image=image; redraw(); }

QImage& DicedChipTemplateLabel::getImage()
{ return m_image; }

void DicedChipTemplateLabel::mouseDoubleClickEvent(QMouseEvent *event)
{
  QString fileName = QFileDialog::getOpenFileName(this,
						  "Save Image", "",
						  "Images (*.png *.xpm *.jpg *.bmp);;All Files (*)");
  if(fileName.isEmpty()) return;
  m_image=QImage(fileName);
  redraw();
}

void DicedChipTemplateLabel::mousePressEvent(QMouseEvent *event)
{
  m_x=event->x();
  m_y=event->y();
  if(!m_image.isNull())
    { // Correct for label being bigger than image
      m_x-=(width() -m_image.width ())/2;
      m_y-=(height()-m_image.height())/2;
    }
  redraw();
}

void DicedChipTemplateLabel::redraw()
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
