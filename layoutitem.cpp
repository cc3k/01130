#include "layoutitem.h"

LayoutItem::LayoutItem(QObject *parent, QString name) : QObject(parent)
{
    this->name = name;

    this->widget = Q_NULLPTR;
    this->item = Q_NULLPTR;
    this->layout = Q_NULLPTR;

    this->parentId = 0;
    this->id = 0;
    this->posx = 127;
    this->posy = 127;

    this->state = Global::State::na;
}

qint32 LayoutItem::getId() const
{
    return id;
}

void LayoutItem::setId(const qint32 &value)
{
    id = value;
}

qint32 LayoutItem::getParentId() const
{
    return parentId;
}

void LayoutItem::setParentId(const qint32 &value)
{
    parentId = value;
}

quint8 LayoutItem::getPosx() const
{
    return posx;
}

quint8 LayoutItem::getPosy() const
{
    return posy;
}

QGroupBox *LayoutItem::getWidget() const
{
    return widget;
}

void LayoutItem::setWidget(QGroupBox *value)
{
    widget = value;
}

QGridLayout *LayoutItem::getLayout()
{
    return layout;
}

void LayoutItem::setLayout(QGridLayout *value)
{
    delete layout;
    layout = value;
}

QPushButton *LayoutItem::getItem() const
{
    return item;
}

void LayoutItem::setItem(QPushButton *value)
{
    item = value;
}

void LayoutItem::setPosXY(const quint8 &valueX, const quint8 &valueY)
{
    posx = valueX;
    posy = valueY;
}

QString LayoutItem::getName() const
{
    return name;
}

void LayoutItem::addItem(LayoutItem *item)
{
    childSet.append(item);

    if (this->widget != Q_NULLPTR)
    {
        if (item->getLayout() != Q_NULLPTR)
        {
            this->boxGrid->addLayout(item->getLayout(), item->getPosy(), item->getPosx());
            //this->childIdSet.append(item->getId());
           // qDebug() << item->getName() << "with id " << item->getId() << "layout added to widget" << this->name;
            //qDebug() << "position: " << item->getPosx() << ", " << item->getPosy();

        }
        if (item->getWidget() != Q_NULLPTR)
        {
            this->boxGrid->addWidget(item->getWidget(), item->getPosy(), item->getPosx());
            //this->childIdSet.append(item->getId());
            //qDebug() << item->getName() << "with id " << item->getId() << "widget added to widget" << this->name;
            //qDebug() << "position: " << item->getPosx() << ", " << item->getPosy();

        }
        if (item->getItem() != Q_NULLPTR)
        {
            this->boxGrid->addWidget(item->getItem(), item->getPosy(), item->getPosx());
            //this->childIdSet.append(item->getId());
            //qDebug() << item->getName() << "with id " << item->getId() << "item added to widget" << this->name;
            //qDebug() << "position: " << item->getPosx() << ", " << item->getPosy();
        }

    }

    if (this->layout != Q_NULLPTR)
    {
        if (item->getLayout() != Q_NULLPTR)
        {
            this->layout->addLayout(item->getLayout(), item->getPosy(), item->getPosy());
            //this->childIdSet.append(item->getId());
            //qDebug() << item->getName() << "with id " << item->getId() <<  "layout added to layout" << this->name;
            //qDebug() << "position: " << item->getPosx() << ", " << item->getPosy();
        }
        if (item->getWidget() != Q_NULLPTR)
        {
            this->layout->addWidget(item->getWidget(), item->getPosy(), item->getPosx());
            //this->childIdSet.append(item->getId());
            //qDebug() << item->getName() << "with id " << item->getId() <<  "widget added to layout" << this->name;
            //qDebug() << "position: " << item->getPosx() << ", " << item->getPosy();
        }
        if (item->getItem() != Q_NULLPTR)
        {
            this->layout->addWidget(item->getItem(), item->getPosy(), item->getPosx());
            //this->childIdSet.append(item->getId());
            //qDebug() << item->getName() << "with id " << item->getId() << "item added to layout" << this->name;
            //qDebug() << "position: " << item->getPosx() << ", " << item->getPosy();
        }
    }
}

Global::State::states LayoutItem::getState() const
{
    return state;
}

void LayoutItem::remove()
{
    //qDebug() << "remove layoutitem";
}
