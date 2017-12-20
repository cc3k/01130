#ifndef VISUALGROUP_H
#define VISUALGROUP_H

#include <QGroupBox>
#include <QGridLayout>

#include "visualobject.h"
#include "layoutitem.h"
#include "global.h"

class VisualGroup : public LayoutItem
{
Q_OBJECT
public:
    VisualGroup(QObject *parent = 0, QString name = "empty");

    QList<VisualObject *> getObjectList() const;
    void setObjectList(const QList<VisualObject *> &value);

    Global::State::states getState() const;
    void setState(const Global::State::states value);

    void setBad();
    void setGood();
    void setAverage();
    void setOffcheck();
    void setNa();
    void setNone();

    void setAlignment(int alignment);
    int getAlignment() const;

    bool getBold() const;
    QString getNone() const;
    void setBold(bool value);

    void remove();

private:
    QList<VisualObject* > objectList;
    bool bold;

    void virtual mouseReleaseEvent();

signals:
    void stateChanged();

public slots:
    void changeState();
};

#endif // VISUALGROUP_H
