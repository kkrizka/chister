#include "SwissHCCTemplateManager.h"

#include <QDir>
#include <QSettings>

SwissHCCTemplateManager::SwissHCCTemplateManager(const QString& directory)
  : m_directory(directory)
{ }

QStringList SwissHCCTemplateManager::list() const
{ return m_templates.keys(); }

bool SwissHCCTemplateManager::contains(const QString& name) const
{ return m_templates.contains(name); }

void SwissHCCTemplateManager::load()
{
  QDir templatesDir(m_directory);
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

      add(loadTemplate);
    }
}

void SwissHCCTemplateManager::add(const SwissHCCTemplate& newtemplate)
{ m_templates[newtemplate.m_name]=newtemplate; }

SwissHCCTemplate SwissHCCTemplateManager::get(const QString& name)
{ return m_templates[name]; }

void SwissHCCTemplateManager::update(const SwissHCCTemplate& newtemplate)
{
  m_templates[newtemplate.m_name]=newtemplate;

  QString templatePath=m_directory+"/"+newtemplate.m_name+"/";
  QDir().mkpath(templatePath);

  if(!newtemplate.m_chipImage.isNull())   newtemplate.m_chipImage.save(templatePath+"/chip.png");
  if(!newtemplate.m_probesImage.isNull()) newtemplate.m_probesImage.save(templatePath+"/probes.png");

  QSettings settings(templatePath+"/config.ini", QSettings::IniFormat);
  settings.setValue("chip/x", newtemplate.m_chipX);
  settings.setValue("chip/y", newtemplate.m_chipY);
  settings.setValue("probes/x", newtemplate.m_probesX);
  settings.setValue("probes/y", newtemplate.m_probesY);
}

