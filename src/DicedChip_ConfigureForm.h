#ifndef DICEDCHIP_CONFIGUREFORM_H
#define DICEDCHIP_CONFIGUREFORM_H

#include <QWidget>

namespace Ui {
class DicedChip_ConfigureForm;
}

class DicedChip_ConfigureForm : public QWidget
{
  Q_OBJECT

public:
  explicit DicedChip_ConfigureForm(QWidget *parent = 0);
  ~DicedChip_ConfigureForm();

  void setTemplates(const QStringList& templates);

private slots:
  void on_logPushButton_clicked();
  void on_donePushButton_clicked();

signals:
  void done(const QString& chiptemplate, bool loadChips, bool findProbes, bool calibratePosition, const QString& logDirectory);

private:
  Ui::DicedChip_ConfigureForm *ui;

  QString m_logDirectory;
};

#endif // DICEDCHIP_CONFIGUREFORM_H
