#include "udpsocketlistener.h"
#include "udpdatagram.h"

#include <QDateTime>

#include <QDebug>
#include <QNetworkInterface>

UdpSocketListener::UdpSocketListener(QObject *parent,
                                     Global::Packet::UdpProto proto,
                                     QString ip, quint16 port) : QObject(parent)
{
    qDebug() << "udp listener start";
    qDebug() << ip;
    this->setObjectName(trUtf8("UDP socket listener"));
    this->proto = proto;
    this->timestamp = 0;
    this->address = QHostAddress(ip);
    this->port = port;

    createSocket();

    packetTimer = new QTimer(this);
    connect(packetTimer, &QTimer::timeout, this, &UdpSocketListener::checkPacketTimeout);
    packetTimer->start(5000);
}

UdpSocketListener::~UdpSocketListener()
{
    socket->close();
    delete socket;
}

void UdpSocketListener::createSocket()
{
    this->socket = new QUdpSocket(this);

    switch (proto)
    {
    case Global::Packet::multicast:
    {
        qDebug() << "socket bind"
                 << socket->bind(QHostAddress::AnyIPv4, this->port, QAbstractSocket::ShareAddress |
                                 QAbstractSocket::ReuseAddressHint);

        QList<QNetworkInterface> mListIfaces = QNetworkInterface::allInterfaces();
        for (int i = 0; i < mListIfaces.length(); ++i) {
            qDebug() << mListIfaces.at(i).name();
            qDebug() << socket->joinMulticastGroup(QHostAddress(address), mListIfaces.at(i));
        }

        //qDebug() << "socket joinMulticastGroup" << socket->joinMulticastGroup(address);
        // джойн на все адаптеры, так работает стабильней
    }
        break;

    case Global::Packet::unicast:
        break;

    case Global::Packet::noproto:
        qDebug() << "set proto in ctor";
        break;

    default:
        break;
    }

    connect(socket, &QUdpSocket::readyRead, this, &UdpSocketListener::processPendingDatagram);

    //    connect(socket, &QUdpSocket::disconnected, this, &UdpSocketListener::reinitSocket);
    //    connect(socket, &QUdpSocket::connected, this, &UdpSocketListener::reinitSocket);
    //    connect(socket, &QUdpSocket::stateChanged, this, &UdpSocketListener::stateCheck);

    qDebug() << socket->state();

}

void UdpSocketListener::stateCheck(QAbstractSocket::SocketState state)
{
    qDebug() << state;
}

void UdpSocketListener::checkPacketTimeout()
{
    if ((QDateTime::currentMSecsSinceEpoch() - (timestamp * 1000)) > 5000)
    {
        qDebug() << "idle socket for 5 sec";
        disconnect(socket, &QUdpSocket::readyRead, this,
                   &UdpSocketListener::processPendingDatagram);
        socket->close();
        createSocket();
    }
}

void UdpSocketListener::reinitSocket()
{
    qDebug() << "get disconnected signal";
}

void UdpSocketListener::connected_()
{
    qDebug() << "get connected signal";
}

void UdpSocketListener::processPendingDatagram()
{
    while (socket->hasPendingDatagrams())
    {
        datagram.resize(socket->pendingDatagramSize());
        socket->readDatagram(datagram.data(), datagram.size(), &host, &hostPort);
        timestamp = QDateTime::currentDateTime().toTime_t();

        UdpDatagram udpDatagram = UdpDatagram(datagram);
        udpDatagram.setHost(host);
        udpDatagram.setPort(port);
        udpDatagram.setTimestamp(timestamp);

        //qDebug() << host;

        emit onDatagram(udpDatagram);
    }
}
