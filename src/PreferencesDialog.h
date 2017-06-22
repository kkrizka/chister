#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>
#include <QListWidgetItem>

namespace Ui {
class PreferencesDialog;
}

class PreferencesDialog : public QDialog
{
  Q_OBJECT

public:
  explicit PreferencesDialog(QWidget *parent = 0);
  ~PreferencesDialog();

  /**
   * \brief Add a settings form.
   *
   * \param title The title used to refer to the form.
   * \oaram widget The widget with all the controls
   */
  void addForm(const QString& title, QWidget *widget);

public slots:
  void changeForm(QListWidgetItem *current, QListWidgetItem *previous);

private slots:
  void on_okPushButton_clicked();
  void on_cancelPushButton_clicked();

private:
  Ui::PreferencesDialog *ui;
};

#endif // PREFERENCESDIALOG_H
