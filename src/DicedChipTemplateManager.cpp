#include "DicedChipTemplateManager.h"

#include <QDir>
#include <QSettings>

DicedChipTemplateManager::DicedChipTemplateManager(const QString& directory)
  : m_directory(directory)
{ }

QStringList DicedChipTemplateManager::list() const
{ return m_templates.keys(); }

bool DicedChipTemplateManager::contains(const QString& name) const
{ return m_templates.contains(name); }

void DicedChipTemplateManager::load()
{
  QDir templatesDir(m_directory);
  templatesDir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
  for(QFileInfo fileInfo : templatesDir.entryInfoList())
    {
      QDir templateDir(fileInfo.filePath());
      if(!templateDir.exists("config.ini")) continue;
      QSettings settings(fileInfo.filePath()+"/config.ini", QSettings::IniFormat);

      DicedChipTemplate loadTemplate;
      loadTemplate.m_name=fileInfo.baseName();

      loadTemplate.m_chipX=settings.value("chip/x",0).toInt();
      loadTemplate.m_chipY=settings.value("chip/y",0).toInt();
      if(templateDir.exists("chip.png")) loadTemplate.setChipImage(QImage(fileInfo.filePath()+"/chip.png"));

      loadTemplate.m_probesX=settings.value("probes/x",0).toInt();
      loadTemplate.m_probesY=settings.value("probes/y",0).toInt();
      if(templateDir.exists("probes.png")) loadTemplate.setProbesImage(QImage(fileInfo.filePath()+"/probes.png"));

      add(loadTemplate);
    }
}

void DicedChipTemplateManager::add(const DicedChipTemplate& newtemplate)
{ m_templates[newtemplate.m_name]=newtemplate; }

DicedChipTemplate DicedChipTemplateManager::get(const QString& name)
{ return m_templates[name]; }

void DicedChipTemplateManager::update(const DicedChipTemplate& newtemplate)
{
  m_templates[newtemplate.m_name]=newtemplate;

  QString templatePath=m_directory+"/"+newtemplate.m_name+"/";
  QDir().mkpath(templatePath);

  if(!newtemplate.qtChipImage()  .isNull()) newtemplate.qtChipImage()  .save(templatePath+"/chip.png");
  if(!newtemplate.qtProbesImage().isNull()) newtemplate.qtProbesImage().save(templatePath+"/probes.png");

  QSettings settings(templatePath+"/config.ini", QSettings::IniFormat);
  settings.setValue("chip/x", newtemplate.m_chipX);
  settings.setValue("chip/y", newtemplate.m_chipY);
  settings.setValue("probes/x", newtemplate.m_probesX);
  settings.setValue("probes/y", newtemplate.m_probesY);
}

