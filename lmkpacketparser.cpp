#include "lmkpacketparser.h"
#include "global.h"

LmkPacketParser::LmkPacketParser(QObject *parent) : QObject(parent)
{

}

bool LmkPacketParser::checkSignature(QByteArray data)
{
	if (data.size() < 11)
	{
		qDebug() << "length <11";
		return false;
	}
	else
	{
		if ((quint8) data[0] == 76 && (quint8) data[1] == 77 && (quint8) data[2] == 75) //L = 76, M = 77, K = 75
		{
			return true;
		}
	}
    qDebug() << "bad signature" << data[0] << data[1] << data[2];
	return false;
}

bool LmkPacketParser::checkLength(QByteArray data)
{
	int packetLength = (data[3] << 8 | data[4]); //3 and 4 digits are the length
	if (packetLength == data.size())
	{
		return true;
	}
	qDebug() << "error length " << packetLength;
	qDebug() << "actual length " << data.size();
	return false;
}

void LmkPacketParser::datagram(UdpDatagram datagram)
{
	if (checkSignature(datagram.getData()) && checkLength(datagram.getData()))
	{
        //qDebug() << "packet valid";
        if (datagram.getPort() == Global::SysDiagnosticPortListen)
		{
            //qDebug() << "diag packet" << datagram.getData();
			emit onDiagnostic(datagram);
		}
	}
}
