#include "SwissHCCPreferencesForm.h"
#include "ui_SwissHCCPreferencesForm.h"

#include <QSerialPortInfo>
#include <QInputDialog>
#include <QDir>
#include <QSettings>
#include <QDebug>

SwissHCCPreferencesForm::SwissHCCPreferencesForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SwissHCCPreferencesForm)
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
  QString templatesPath=QApplication::applicationDirPath()+"/SwissHCCTemplates/";
  QDir templatesDir(templatesPath);
  templatesDir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
  for(QFileInfo fileInfo : templatesDir.entryInfoList())
    {
      QDir templateDir(fileInfo.filePath());
      if(!templateDir.exists("config.ini")) continue;
      QSettings settings(fileInfo.filePath()+"/config.ini", QSettings::IniFormat);

      SwissHCCTemplate loadTemplate;
      loadTemplate.m_name=fileInfo.baseName();

      loadTemplate.m_chipX=settings.value("chip/x",0).toInt();
      loadTemplate.m_chipY=settings.value("chip/y",0).toInt();
      if(templateDir.exists("chip.png")) loadTemplate.m_chipImage=QImage(fileInfo.filePath()+"/chip.png");

      loadTemplate.m_probesX=settings.value("probes/x",0).toInt();
      loadTemplate.m_probesY=settings.value("probes/y",0).toInt();
      if(templateDir.exists("probes.png")) loadTemplate.m_probesImage=QImage(fileInfo.filePath()+"/probes.png");

      addTemplate(loadTemplate);
    }
}

void SwissHCCPreferencesForm::addTemplate(const SwissHCCTemplate& newtemplate)
{
  m_templates[newtemplate.m_name]=newtemplate;
  ui->templatesComboBox->addItem(newtemplate.m_name);
}

void SwissHCCPreferencesForm::saveTemplate(const QString& name)
{
  QString templatePath=QApplication::applicationDirPath()+"/SwissHCCTemplates/"+name+"/";
  QDir().mkpath(templatePath);

  if(!m_templates[name].m_chipImage.isNull())   m_templates[name].m_chipImage.save(templatePath+"/chip.png");
  if(!m_templates[name].m_probesImage.isNull()) m_templates[name].m_probesImage.save(templatePath+"/probes.png");

  QSettings settings(templatePath+"/config.ini", QSettings::IniFormat);
  settings.setValue("chip/x", m_templates[name].m_chipX);
  settings.setValue("chip/y", m_templates[name].m_chipY);
  settings.setValue("probes/x", m_templates[name].m_probesX);
  settings.setValue("probes/y", m_templates[name].m_probesY);
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

  saveTemplate(name);
}

void SwissHCCPreferencesForm::on_templatesComboBox_activated(const QString &templateName)
{
  showTemplate(templateName);
}
