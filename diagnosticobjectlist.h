#ifndef DIAGNOSTICOBJECTLIST_H
#define DIAGNOSTICOBJECTLIST_H

#include <QObject>
#include <QList>

#include "layoutitem.h"
#include "visualgrid.h"
#include "visualobject.h"
#include "visualgroup.h"

class DiagnosticObjectList : public QObject
{
    Q_OBJECT
public:
    explicit DiagnosticObjectList(QObject *parent = 0);

    QList<LayoutItem *> getLayoutItemList() const;

    QList<VisualGrid *> getGridList() const;
    void addGrid(VisualGrid* item);

    QList<VisualGroup *> getGroupList() const;
    void addGroup(VisualGroup* item);

    QList<VisualObject *> getObjectList() const;
    void addObject(VisualObject* item);

    LayoutItem* getItemById(qint16 value) const;

    LayoutItem* getItemByName(QString value) const;

    void clear();

private:
    QList<LayoutItem* > itemList;
    QList<VisualGrid* > gridList;
    QList<VisualGroup* > groupList;
    QList<VisualObject* > objectList;
};

#endif // DIAGNOSTICOBJECTLIST_H
