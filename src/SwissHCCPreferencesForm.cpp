#include "SwissHCCPreferencesForm.h"
#include "ui_SwissHCCPreferencesForm.h"

#include <QSerialPortInfo>
#include <QInputDialog>
#include <QDebug>

SwissHCCPreferencesForm::SwissHCCPreferencesForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SwissHCCPreferencesForm)
{
    ui->setupUi(this);

    for(QSerialPortInfo port : QSerialPortInfo::availablePorts())
      ui->serialPortComboBox->addItem(port.portName(),port.portName());
}

SwissHCCPreferencesForm::~SwissHCCPreferencesForm()
{
    delete ui;
}

void SwissHCCPreferencesForm::addTemplate(const SwissHCCTemplate& newtemplate)
{
  m_templates[newtemplate.m_name]=newtemplate;
  ui->templatesComboBox->addItem(newtemplate.m_name);
}

void SwissHCCPreferencesForm::showTemplate(const QString& name)
{
  ui->chipTemplateLabel->setX(m_templates[name].m_chipX);
  ui->chipTemplateLabel->setY(m_templates[name].m_chipY);
  ui->chipTemplateLabel->setImage(m_templates[name].m_chipImage);

  ui->probesTemplateLabel->setX(m_templates[name].m_probesX);
  ui->probesTemplateLabel->setY(m_templates[name].m_probesY);
  ui->probesTemplateLabel->setImage(m_templates[name].m_probesImage);
}

void SwissHCCPreferencesForm::on_newPushButton_clicked()
{
  bool ok;
  QString name = QInputDialog::getText(this, tr("Template Name"),
				       tr("Name:"), QLineEdit::Normal, "", &ok);

  if(!ok || name.isEmpty()) return;

  if(m_templates.contains(name))
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

      if(m_templates.contains(name))
	return; // TODO: Handle error

      SwissHCCTemplate newTemplate;
      newTemplate.m_name=name;
      addTemplate(newTemplate);
    }

  // Save data from labels
  m_templates[name].m_chipX=ui->chipTemplateLabel->getX();
  m_templates[name].m_chipY=ui->chipTemplateLabel->getY();
  m_templates[name].m_chipImage=ui->chipTemplateLabel->getImage();

  m_templates[name].m_probesX=ui->probesTemplateLabel->getX();
  m_templates[name].m_probesY=ui->probesTemplateLabel->getY();
  m_templates[name].m_probesImage=ui->probesTemplateLabel->getImage();
}

void SwissHCCPreferencesForm::on_templatesComboBox_activated(const QString &templateName)
{
  showTemplate(templateName);
}
