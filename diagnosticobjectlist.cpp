#include "diagnosticobjectlist.h"

DiagnosticObjectList::DiagnosticObjectList(QObject *parent) : QObject(parent)
{

}

QList<LayoutItem *> DiagnosticObjectList::getLayoutItemList() const
{
    return itemList;
}

QList<VisualGrid *> DiagnosticObjectList::getGridList() const
{
    return gridList;
}

void DiagnosticObjectList::addGrid(VisualGrid *item)
{
    this->gridList.append(item);
    this->itemList.append(item);
}

QList<VisualGroup *> DiagnosticObjectList::getGroupList() const
{
    return groupList;
}

void DiagnosticObjectList::addGroup(VisualGroup *item)
{
    this->groupList.append(item);
    this->itemList.append(item);
}

QList<VisualObject *> DiagnosticObjectList::getObjectList() const
{
    return objectList;
}

void DiagnosticObjectList::addObject(VisualObject *item)
{
    this->objectList.append(item);
    this->itemList.append(item);
}

LayoutItem *DiagnosticObjectList::getItemById(qint16 value) const
{
    for (int i = 0; i < itemList.size(); ++i)
    {
        if (itemList.at(i)->getId() == value)
        {
            return itemList.at(i);
        }
    }
    return new LayoutItem(0, "");
}

LayoutItem *DiagnosticObjectList::getItemByName(QString value) const
{
    for (int i = 0; i < itemList.size(); ++i)
    {
        if (itemList.at(i)->getName() == value)
        {
            return itemList.at(i);
        }
    }
    return Q_NULLPTR;
}

void DiagnosticObjectList::clear()
{
    itemList.clear();
    gridList.clear();
    groupList.clear();
    objectList.clear();
}
