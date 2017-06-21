#ifndef SWISSHCC_CONFIGUREFORM_H
#define SWISSHCC_CONFIGUREFORM_H

#include <QWidget>

namespace Ui {
class SwissHCC_ConfigureForm;
}

class SwissHCC_ConfigureForm : public QWidget
{
  Q_OBJECT

public:
  explicit SwissHCC_ConfigureForm(QWidget *parent = 0);
  ~SwissHCC_ConfigureForm();

  void setTemplates(const QStringList& templates);

private slots:
  void on_logPushButton_clicked();
  void on_donePushButton_clicked();

signals:
  void done(bool loadChips, bool findProbes, bool calibratePosition, const QString& logDirectory);

private:
  Ui::SwissHCC_ConfigureForm *ui;

  QString m_logDirectory;
};

#endif // SWISSHCC_CONFIGUREFORM_H
