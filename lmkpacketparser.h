#ifndef LMKPACKETPARSER_H
#define LMKPACKETPARSER_H

#include <QObject>
#include <QByteArray>

#include "udpdatagram.h"

class LmkPacketParser : public QObject
{
	Q_OBJECT
public:
	explicit LmkPacketParser(QObject *parent = 0);

private:
	bool checkSignature(QByteArray data);
	bool checkLength(QByteArray data);

signals:
	void onDiagnostic(UdpDatagram datagram);

public slots:
void datagram(UdpDatagram datagram);

};

#endif // LMKPACKETPARSER_H
