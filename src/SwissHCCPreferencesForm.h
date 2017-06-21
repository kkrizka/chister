#ifndef SWISSHCCPREFERENCESFORM_H
#define SWISSHCCPREFERENCESFORM_H

#include <QWidget>
#include <QMap>

#include "SwissHCCTemplate.h"
#include "SwissHCCTemplateManager.h"

namespace Ui {
class SwissHCCPreferencesForm;
}

class SwissHCCPreferencesForm : public QWidget
{
  Q_OBJECT

public:
  explicit SwissHCCPreferencesForm(SwissHCCTemplateManager *templates, QWidget *parent = 0);
  ~SwissHCCPreferencesForm();

private slots:
  void on_newPushButton_clicked();
  void on_savePushButton_clicked();
  void on_templatesComboBox_activated(const QString& templateName);

private:
  Ui::SwissHCCPreferencesForm *ui;

  SwissHCCTemplateManager *m_templates;

  void loadTemplates();
  void addTemplate(const SwissHCCTemplate& newtemplate);
  void showTemplate(const QString& name);
};

#endif // SWISSHCCPREFERENCESFORM_H
