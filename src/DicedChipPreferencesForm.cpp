#include "DicedChipPreferencesForm.h"
#include "ui_DicedChipPreferencesForm.h"

#include <QInputDialog>
#include <QDir>
#include <QSettings>
#include <QDebug>

DicedChipPreferencesForm::DicedChipPreferencesForm(DicedChipTemplateManager *templates, QWidget *parent)
  : QWidget(parent),
    ui(new Ui::DicedChipPreferencesForm),
    m_templates(templates)
{
  ui->setupUi(this);

  loadTemplates();
}

DicedChipPreferencesForm::~DicedChipPreferencesForm()
{
    delete ui;
}

void DicedChipPreferencesForm::loadTemplates()
{
  for(const QString& name : m_templates->list())
    addTemplate(m_templates->get(name));
  showTemplate(ui->templatesComboBox->currentText());
}

void DicedChipPreferencesForm::addTemplate(const DicedChipTemplate& newtemplate)
{
  m_templates->add(newtemplate);
  ui->templatesComboBox->addItem(newtemplate.m_name);
}

void DicedChipPreferencesForm::showTemplate(const QString& name)
{
  ui->chipTemplateLabel->setX(m_templates->get(name).m_chipX);
  ui->chipTemplateLabel->setY(m_templates->get(name).m_chipY);
  ui->chipTemplateLabel->setImage(m_templates->get(name).qtChipImage());

  ui->probesTemplateLabel->setX(m_templates->get(name).m_probesX);
  ui->probesTemplateLabel->setY(m_templates->get(name).m_probesY);
  ui->probesTemplateLabel->setImage(m_templates->get(name).qtProbesImage());
}

void DicedChipPreferencesForm::on_newPushButton_clicked()
{
  bool ok;
  QString name = QInputDialog::getText(this, tr("Template Name"),
				       tr("Name:"), QLineEdit::Normal, "", &ok);

  if(!ok || name.isEmpty()) return;

  if(m_templates->contains(name))
    return; // TODO: Handle error

  DicedChipTemplate newTemplate;
  newTemplate.m_name=name;
  addTemplate(newTemplate);
  showTemplate(name);
  ui->templatesComboBox->setCurrentText(name);
}

void DicedChipPreferencesForm::on_savePushButton_clicked()
{
  QString name=ui->templatesComboBox->currentText();
  if(name.isEmpty())
    { // Create new template as no template is selected
      bool ok;
      name = QInputDialog::getText(this, tr("Template Name"),
				   tr("Name:"), QLineEdit::Normal, "", &ok);

      if(!ok || name.isEmpty()) return;

      if(m_templates->contains(name))
	return; // TODO: Handle error

      DicedChipTemplate newTemplate;
      newTemplate.m_name=name;
      addTemplate(newTemplate);
    }

  // Save data from labels
  DicedChipTemplate thetemplate=m_templates->get(name);
  thetemplate.m_chipX=ui->chipTemplateLabel->getX();
  thetemplate.m_chipY=ui->chipTemplateLabel->getY();
  thetemplate.setChipImage(ui->chipTemplateLabel->getImage());

  thetemplate.m_probesX=ui->probesTemplateLabel->getX();
  thetemplate.m_probesY=ui->probesTemplateLabel->getY();
  thetemplate.setProbesImage(ui->probesTemplateLabel->getImage());

  m_templates->update(thetemplate);
}

void DicedChipPreferencesForm::on_templatesComboBox_activated(const QString &templateName)
{
  showTemplate(templateName);
}
