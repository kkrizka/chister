#ifndef DICEDCHIPTEMPLATEMANAGER_H_
#define DICEDCHIPTEMPLATEMANAGER_H_

#include <QMap>

#include "DicedChipTemplate.h"

class DicedChipTemplateManager
{
public:
  DicedChipTemplateManager(const QString& directory);

  void load();

  QStringList list() const;
  bool contains(const QString& name) const;

  void add(const DicedChipTemplate& newtemplate);
  DicedChipTemplate get(const QString& name);
  void update(const DicedChipTemplate& newtemplate);

private:
  QString m_directory;
  QMap<QString, DicedChipTemplate> m_templates;
};

#endif // DICEDCHIPTEMPLATEMANAGER_H_
