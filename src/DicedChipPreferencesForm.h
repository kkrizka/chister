#ifndef DICEDCHIPPREFERENCESFORM_H
#define DICEDCHIPPREFERENCESFORM_H

#include <QWidget>
#include <QMap>

#include "DicedChipTemplate.h"
#include "DicedChipTemplateManager.h"

namespace Ui {
  class DicedChipPreferencesForm;
}

class DicedChipPreferencesForm : public QWidget
{
  Q_OBJECT

public:
  explicit DicedChipPreferencesForm(DicedChipTemplateManager *templates, QWidget *parent = 0);
  ~DicedChipPreferencesForm();

private slots:
  void on_newPushButton_clicked();
  void on_savePushButton_clicked();
  void on_templatesComboBox_activated(const QString& templateName);

private:
  Ui::DicedChipPreferencesForm *ui;

  DicedChipTemplateManager *m_templates;

  void loadTemplates();
  void addTemplate(const DicedChipTemplate& newtemplate);
  void showTemplate(const QString& name);
};

#endif // DICEDCHIPPREFERENCESFORM_H
