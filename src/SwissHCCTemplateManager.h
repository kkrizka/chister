#ifndef SWISSHCCTEMPLATEMANAGER_H_
#define SWISSHCCTEMPLATEMANAGER_H_

#include <QMap>

#include "SwissHCCTemplate.h"

class SwissHCCTemplateManager
{
public:
  SwissHCCTemplateManager(const QString& directory);

  void load();

  QStringList list() const;
  bool contains(const QString& name) const;

  void add(const SwissHCCTemplate& newtemplate);
  SwissHCCTemplate get(const QString& name);
  void update(const SwissHCCTemplate& newtemplate);

private:
  QString m_directory;
  QMap<QString, SwissHCCTemplate> m_templates;
};

#endif // SWISSHCCTEMPLATEMANAGER_H_
