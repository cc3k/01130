#include "udpdatagram.h"

UdpDatagram::UdpDatagram(QByteArray data)
{
	this->data = data;
}

QHostAddress UdpDatagram::getHost() const
{
	return host;
}

void UdpDatagram::setHost(const QHostAddress &value)
{
	host = value;
}

quint16 UdpDatagram::getPort() const
{
	return port;
}

void UdpDatagram::setPort(const quint16 &value)
{
	port = value;
}

uint UdpDatagram::getTimestamp() const
{
	return timestamp;
}

void UdpDatagram::setTimestamp(const uint &value)
{
	timestamp = value;
}

QByteArray UdpDatagram::getData() const
{
	return data;
}
