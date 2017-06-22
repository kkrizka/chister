#include "SwissHCCPreferencesForm.h"
#include "ui_SwissHCCPreferencesForm.h"

#include <QSerialPortInfo>
#include <QInputDialog>
#include <QDir>
#include <QSettings>
#include <QDebug>

SwissHCCPreferencesForm::SwissHCCPreferencesForm(SwissHCCTemplateManager *templates, QWidget *parent)
  : QWidget(parent),
    ui(new Ui::SwissHCCPreferencesForm),
    m_templates(templates)
{
    ui->setupUi(this);

    for(QSerialPortInfo port : QSerialPortInfo::availablePorts())
      ui->serialPortComboBox->addItem(port.portName(),port.portName());

    loadTemplates();
}

SwissHCCPreferencesForm::~SwissHCCPreferencesForm()
{
    delete ui;
}

void SwissHCCPreferencesForm::loadTemplates()
{
  for(const QString& name : m_templates->list())
    addTemplate(m_templates->get(name));
}

void SwissHCCPreferencesForm::addTemplate(const SwissHCCTemplate& newtemplate)
{
  m_templates->add(newtemplate);
  ui->templatesComboBox->addItem(newtemplate.m_name);
}

void SwissHCCPreferencesForm::showTemplate(const QString& name)
{
  ui->chipTemplateLabel->setX(m_templates->get(name).m_chipX);
  ui->chipTemplateLabel->setY(m_templates->get(name).m_chipY);
  ui->chipTemplateLabel->setImage(m_templates->get(name).qtChipImage());

  ui->probesTemplateLabel->setX(m_templates->get(name).m_probesX);
  ui->probesTemplateLabel->setY(m_templates->get(name).m_probesY);
  ui->probesTemplateLabel->setImage(m_templates->get(name).qtProbesImage());
}

void SwissHCCPreferencesForm::on_newPushButton_clicked()
{
  bool ok;
  QString name = QInputDialog::getText(this, tr("Template Name"),
				       tr("Name:"), QLineEdit::Normal, "", &ok);

  if(!ok || name.isEmpty()) return;

  if(m_templates->contains(name))
    return; // TODO: Handle error

  SwissHCCTemplate newTemplate;
  newTemplate.m_name=name;
  addTemplate(newTemplate);
  showTemplate(name);
  ui->templatesComboBox->setCurrentText(name);
}

void SwissHCCPreferencesForm::on_savePushButton_clicked()
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

      SwissHCCTemplate newTemplate;
      newTemplate.m_name=name;
      addTemplate(newTemplate);
    }

  // Save data from labels
  SwissHCCTemplate thetemplate=m_templates->get(name);
  thetemplate.m_chipX=ui->chipTemplateLabel->getX();
  thetemplate.m_chipY=ui->chipTemplateLabel->getY();
  thetemplate.setChipImage(ui->chipTemplateLabel->getImage());

  thetemplate.m_probesX=ui->probesTemplateLabel->getX();
  thetemplate.m_probesY=ui->probesTemplateLabel->getY();
  thetemplate.setProbesImage(ui->probesTemplateLabel->getImage());

  m_templates->update(thetemplate);
}

void SwissHCCPreferencesForm::on_templatesComboBox_activated(const QString &templateName)
{
  showTemplate(templateName);
}
