#ifndef DIALOGMOVETO_H
#define DIALOGMOVETO_H

#include <QDialog>

namespace Ui {
  class DialogMoveTo;
}

class DialogMoveTo : public QDialog
{
  Q_OBJECT

public:
  explicit DialogMoveTo(QWidget *parent = 0);
  ~DialogMoveTo();

  void setX(double x);
  void setY(double y);

  double x() const;
  double y() const;

private:
  Ui::DialogMoveTo *ui;
};

#endif // DIALOGMOVETO_H
