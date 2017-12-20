#ifndef LAYOUTITEM_H
#define LAYOUTITEM_H

#include <QObject>
#include <QGroupBox>
#include <QGridLayout>
#include <QPushButton>

#include "global.h"

class LayoutItem : public QObject
{
    Q_OBJECT
public:
    explicit LayoutItem(QObject *parent = 0, QString name = "empty");

    qint32 getId() const;
    void setId(const qint32 &value);

    qint32 getParentId() const;
    void setParentId(const qint32 &value);

    quint8 getPosx() const;

    quint8 getPosy() const;

    QGroupBox *getWidget() const;
    void setWidget(QGroupBox *value);

    QGridLayout *getLayout();
    void setLayout(QGridLayout *value);

    QPushButton *getItem() const;
    void setItem(QPushButton *value);

    void setPosXY(const quint8 &valueX, const quint8 &valueY);

    QString getName() const;

    void addItem(LayoutItem *item);

    Global::State::states getState() const;

    void remove();

protected:
    QPushButton* item;
    QGroupBox* widget;
    QGridLayout* boxGrid;
    QGridLayout* layout;
    QString name;
    qint32 id;
    qint32 parentId;
    quint8 posx;
    quint8 posy;
    Global::State::states state;
    QList<LayoutItem *> childSet;


};

#endif // LAYOUTITEM_H
