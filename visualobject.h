#ifndef VISUALOBJECT_H
#define VISUALOBJECT_H

#include "layoutitem.h"
#include "global.h"
#include "diagnosticitem.h"

class VisualObject : public LayoutItem
{
Q_OBJECT
public:
    VisualObject(QObject *parent = 0, QString name = "empty");

    Global::State::states getState() const;
    void setState(const Global::State::states &value);

    QString getRole() const;
    void setRole(const QString &value);

    void setGood();
    void setBad();
    void setAverage();
    void setOffcheck();
    void setNa();
    void setNone();

    void setId(const qint16 &value);

    QList<DiagnosticItem *> getDiagnosticItemList() const;

    void addDiagnosticItem(DiagnosticItem* item);

    bool getBold() const;
    void setBold(bool value);

    void remove();

signals:
    void stateChanged();
    void logWrite(QStringList);

private:
    QString role; //роль РТР ОПО РЭО...
    QString name; //имя вычислителя. Для системы назначение и группа вычислителей, чтобы не морочиться

    QList<DiagnosticItem *> diagnosticItemList;
    Global::State::states oldState;
    QStringList message;

    bool bold;

public slots:
    void changeState();
};

#endif // VISUALOBJECT_H
