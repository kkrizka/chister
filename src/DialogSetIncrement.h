#ifndef DIALOGSETINCREMENT_H
#define DIALOGSETINCREMENT_H

#include <QDialog>

namespace Ui {
  class DialogSetIncrement;
}

class DialogSetIncrement : public QDialog
{
  Q_OBJECT

public:
  explicit DialogSetIncrement(QWidget *parent = 0);
  ~DialogSetIncrement();

  void setX(double x);
  void setY(double y);

  double x() const;
  double y() const;

private:
  Ui::DialogSetIncrement *ui;
};

#endif // DIALOGSETINCREMENT_H
