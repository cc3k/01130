#ifndef XMLCONFIG_H
#define XMLCONFIG_H

#include <QObject>
#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

#include "diagnosticitem.h"
#include "visualgrid.h"
#include "visualgroup.h"
#include "visualobject.h"
#include "diagnosticobjectlist.h"

class XmlConfig
{
public:
    XmlConfig(QString name);

    void writeConfig();
    void readConfig();

    void setItemList(const QList<DiagnosticItem *> &value);
    void setObjectsList(DiagnosticObjectList *value);

    QList<DiagnosticItem *> getItemList() const;
    DiagnosticObjectList* getObjectsList() const;

    VisualGrid* getRootGrid();

private:
    QString configName;
    QFile* configFile;
    QXmlStreamWriter* xmlWriter;
    QXmlStreamReader* xmlReader;

    QList<DiagnosticItem* > itemList;
    DiagnosticObjectList* objectsList;

    void writeItem(QXmlStreamWriter *writer, const DiagnosticItem* item);
    void writeGrid(QXmlStreamWriter *writer, const VisualGrid* grid);
    void writeGroup(QXmlStreamWriter *writer, const VisualGroup* group);
    void writeObject(QXmlStreamWriter *writer, const VisualObject* object);

};

#endif // XMLCONFIG_H
