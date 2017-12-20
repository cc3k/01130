#ifndef SELFDIAGNOSTIC_H
#define SELFDIAGNOSTIC_H

#include <QObject>
#include <QTimer>
#include <QSettings>
#include <QUdpSocket>
#include <QDebug>

#include <global.h>
#include "lmkpacketbuilder.h"

class SelfDiagnostic : public QObject
{
    Q_OBJECT
public:
    explicit SelfDiagnostic(QObject *parent = nullptr);

private:
    QTimer *timer;
    QByteArray packet;
    LmkPacketBuilder *builder;
    QUdpSocket *socket;


signals:

private slots:
    void onTimeut();

public slots:
};

#endif // SELFDIAGNOSTIC_H
