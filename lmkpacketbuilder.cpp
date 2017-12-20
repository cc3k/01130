#include "lmkpacketbuilder.h"


LmkPacketBuilder::LmkPacketBuilder(QObject *parent) : QObject(parent)
{
	//	bundle = new LmkGlobal::PacketBundle;
}

LmkPacketBuilder::~LmkPacketBuilder()
{

}

void LmkPacketBuilder::createBundle(Global::Packet::Type type)
{
    bundle = new Global::PacketBundle;

	bundle->type = type;

	switch (type) {

    case Global::Packet::logoff:

        bundle->net.port = Global::RemoteControlPort;
        bundle->net.proto = Global::Packet::unicast;

		bundle->packet.PacketSignature = "LMK";
		bundle->packet.PacketLength = QByteArray::fromHex("000B");
		bundle->packet.PacketCycleCounter = QByteArray::fromHex("0001");
        bundle->packet.PacketId = QByteArray::fromHex("0005");
		bundle->packet.PacketSysId = QByteArray::fromHex("00");
		bundle->packet.PacketSubSysId = QByteArray::fromHex("00");

		break;

    case Global::Packet::reboot:

        bundle->net.port = Global::RemoteControlPort;
        bundle->net.proto = Global::Packet::unicast;

		bundle->packet.PacketSignature = "LMK";
		bundle->packet.PacketLength = QByteArray::fromHex("000B");
		bundle->packet.PacketCycleCounter = QByteArray::fromHex("0001");
        bundle->packet.PacketId = QByteArray::fromHex("0003");
		bundle->packet.PacketSysId = QByteArray::fromHex("00");
		bundle->packet.PacketSubSysId = QByteArray::fromHex("00");

		break;

    case Global::Packet::shutdown:

        bundle->net.port = Global::RemoteControlPort;
        bundle->net.proto = Global::Packet::unicast;

		bundle->packet.PacketSignature = "LMK";
		bundle->packet.PacketLength = QByteArray::fromHex("000B");
		bundle->packet.PacketCycleCounter = QByteArray::fromHex("0001");
        bundle->packet.PacketId = QByteArray::fromHex("0004");
		bundle->packet.PacketSysId = QByteArray::fromHex("00");
		bundle->packet.PacketSubSysId = QByteArray::fromHex("00");

		break;

    case Global::Packet::diagnostic:

        bundle->net.port = Global::DiagnosticPortSend;
        bundle->net.dstAddr = Global::DiagnosticAddress;
        bundle->net.proto = Global::Packet::multicast;

		bundle->packet.PacketSignature = "LMK";
		bundle->packet.PacketLength = QByteArray::fromHex("000B");
		bundle->packet.PacketCycleCounter = QByteArray::fromHex("0001");
        bundle->packet.PacketId = QByteArray::fromHex("0000");
		bundle->packet.PacketSysId = QByteArray::fromHex("00");
		bundle->packet.PacketSubSysId = QByteArray::fromHex("00");

		break;

    case Global::Packet::sysdiagnostic:

        bundle->net.port = Global::DiagnosticPortSend;
        bundle->net.dstAddr = Global::DiagnosticAddress;
        bundle->net.proto = Global::Packet::multicast;

		bundle->packet.PacketSignature = "LMK";
		bundle->packet.PacketLength = QByteArray::fromHex("000B");
		bundle->packet.PacketCycleCounter = QByteArray::fromHex("0001");
        bundle->packet.PacketId = QByteArray::fromHex("0000");
		bundle->packet.PacketSysId = QByteArray::fromHex("00");
		bundle->packet.PacketSubSysId = QByteArray::fromHex("00");

		break;

    case Global::Packet::data:

		bundle->packet.PacketSignature = "LMK";
		bundle->packet.PacketLength = QByteArray::fromHex("000B");
		bundle->packet.PacketCycleCounter = QByteArray::fromHex("0001");
        bundle->packet.PacketId = QByteArray::fromHex("0000");
		bundle->packet.PacketSysId = QByteArray::fromHex("00");
		bundle->packet.PacketSubSysId = QByteArray::fromHex("00");

		break;

	default:
		break;

	}

	bundle->data.clear();
	bundle->data.append(bundle->packet.PacketSignature);
	bundle->data.append(bundle->packet.PacketLength);
	bundle->data.append(bundle->packet.PacketCycleCounter);
	bundle->data.append(bundle->packet.PacketId);
	bundle->data.append(bundle->packet.PacketSysId);
	bundle->data.append(bundle->packet.PacketSubSysId);
	bundle->data.append(bundle->packet.PacketData);

}

void LmkPacketBuilder::setData(const QByteArray &value)
{
	bundle->packet.PacketData.clear();
	bundle->packet.PacketData = value;
    bundle->data.insert(11, bundle->packet.PacketData);
    bundle->data.resize(11 + value.size());

	calcLength();
}

void LmkPacketBuilder::setPort(const quint16 &value)
{
	bundle->net.port = value;
}

void LmkPacketBuilder::setProto(const Global::Packet::UdpProto value)
{
	bundle->net.proto = value;
}

void LmkPacketBuilder::setSrcAddr(const QString &value)
{
	bundle->net.srcAddr = value;
}

void LmkPacketBuilder::setDstAddr(const QString &value)
{
    bundle->net.dstAddr = value;
}

void LmkPacketBuilder::setHeadSysId(const QByteArray &value)
{
    bundle->packet.PacketSysId = value;
    bundle->data.replace(9, 1, bundle->packet.PacketSysId);
}

void LmkPacketBuilder::setHeadSubsysId(const QByteArray &value)
{
    bundle->packet.PacketSubSysId = value;
    bundle->data.replace(10, 1, bundle->packet.PacketSubSysId);
}

Global::PacketBundle *LmkPacketBuilder::getPacketBundle() const
{
	return bundle;
}

void LmkPacketBuilder::calcLength()
{
	char a = (bundle->data.size() >> 8) & 0XFF;
	char b = bundle->data.size() & 0XFF;

	bundle->packet.PacketLength[0] = a;
	bundle->packet.PacketLength[1] = b;

	bundle->data.replace(3, 2, bundle->packet.PacketLength);
}

void LmkPacketBuilder::incCycleCounter()
{
	if ((quint8) bundle->packet.PacketCycleCounter[1] < 255)
	{
		bundle->packet.PacketCycleCounter[1] = bundle->packet.PacketCycleCounter[1] + 1;
	}
	else
	{
		if ((quint8) bundle->packet.PacketCycleCounter[0] < 255)
		{
			bundle->packet.PacketCycleCounter[0] = bundle->packet.PacketCycleCounter[0] + 1;
		}
		else
		{
			bundle->packet.PacketCycleCounter[0] = 0;
			bundle->packet.PacketCycleCounter[1] = 0;
		}
	}

	bundle->data.replace(5, 2, bundle->packet.PacketCycleCounter);
}

void LmkPacketBuilder::setCycleCounter(const quint16 &value)
{
	char a = (value >> 8) & 0XFF;
	char b = value & 0XFF;

	bundle->packet.PacketCycleCounter[0] = a;
	bundle->packet.PacketCycleCounter[1] = b;

	bundle->data.replace(5, 2, bundle->packet.PacketCycleCounter);
}
