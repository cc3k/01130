#include "selfdiagnostic.h"

SelfDiagnostic::SelfDiagnostic(QObject *parent) : QObject(parent)
{
    QSettings settings("settings.ini", QSettings::IniFormat);
    timer = new QTimer(this);
    timer->setInterval(settings.value("diagnostic_emit_timeout").toInt());
    connect(timer, &QTimer::timeout, this, &SelfDiagnostic::onTimeut);
    timer->start();

    builder = new LmkPacketBuilder;
    builder->createBundle(Global::Packet::diagnostic);
    QByteArray hsys;
    QByteArray hsubsys;
    hsys.append(settings.value("hsys").toInt());
    hsubsys.append(settings.value("hsubsys").toInt());
    builder->setHeadSysId(hsys);
    builder->setHeadSubsysId(hsubsys);

    QByteArray diagnosticData;
    diagnosticData.append(settings.value("sys").toInt());
    diagnosticData.append(settings.value("subsys").toInt());
    diagnosticData.append(QByteArray::fromHex("02")); // колхоз, но сойдет :)
    builder->setData(diagnosticData);

    socket = new QUdpSocket;
    qDebug() << socket->bind(QHostAddress(settings.value("ip").toString()));
    qDebug() << QHostAddress(builder->getPacketBundle()->net.dstAddr);
    qDebug() << builder->getPacketBundle()->net.port;

}

void SelfDiagnostic::onTimeut()
{

    socket->writeDatagram(builder->getPacketBundle()->data,
                          builder->getPacketBundle()->data.size(),
                          QHostAddress(builder->getPacketBundle()->net.dstAddr),
                          builder->getPacketBundle()->net.port);
}
