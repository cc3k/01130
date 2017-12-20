#ifndef UDPSOCKETLISTENER_H
#define UDPSOCKETLISTENER_H

#include <QObject>
#include <QHostAddress>
#include <QUdpSocket>

#include <QTimer>
#include <QDateTime>

#include "udpdatagram.h"
#include "global.h"

class UdpSocketListener : public QObject
{
	Q_OBJECT
public:
    explicit UdpSocketListener(QObject *parent = 0,
                               Global::Packet::UdpProto = Global::Packet::noproto,
                               QString ip = "0.0.0.0", quint16 port = 0);
	~UdpSocketListener();

private:
    Global::Packet::UdpProto proto;
	QUdpSocket *socket;
	QHostAddress address;
	quint16 port;
	QByteArray datagram;
	QHostAddress host;
	quint16 hostPort;
    qint64 timestamp;

    qint64 current;
    quint32 packetTimeout;
    QTimer *packetTimer;

    void createSocket();

signals:
    void onDatagram(UdpDatagram);

public slots:

private slots:
    void stateCheck(QAbstractSocket::SocketState state);
    void checkPacketTimeout();
    void reinitSocket();
    void connected_();
    void processPendingDatagram();

};

#endif // UDPSOCKETLISTENER_H
