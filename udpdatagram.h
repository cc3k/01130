#ifndef UDPDATAGRAM_H
#define UDPDATAGRAM_H

#include <QHostAddress>

class UdpDatagram
{
public:
	explicit UdpDatagram(QByteArray data);

	QHostAddress getHost() const;
	void setHost(const QHostAddress &value);

	quint16 getPort() const;
	void setPort(const quint16 &value);

	uint getTimestamp() const;
	void setTimestamp(const uint &value);

	QByteArray getData() const;

private:
	QByteArray data;
	QHostAddress host;
	quint16 port;
	uint timestamp;

};

#endif // UDPDATAGRAM_H
