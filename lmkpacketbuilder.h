#ifndef LMKPACKETFACTORY_H
#define LMKPACKETFACTORY_H

#include <QObject>
#include <QDebug>

#include "global.h"

class LmkPacketBuilder : public QObject
{
	Q_OBJECT
public:
    explicit LmkPacketBuilder(QObject *parent = 0);
    ~LmkPacketBuilder();

    void createBundle(Global::Packet::Type type);
	void setData(const QByteArray &value);
	void setPort(const quint16 &value);
    void setProto(const Global::Packet::UdpProto value);
	void setSrcAddr(const QString &value);
	void setDstAddr(const QString &value);
    void setHeadSysId(const QByteArray &value);
    void setHeadSubsysId(const QByteArray &value);
	void setCycleCounter(const quint16 &value);
	void incCycleCounter();

    Global::PacketBundle *getPacketBundle() const;

private:
    Global::PacketBundle *bundle;

	void calcLength();

signals:

public slots:
};

#endif // LMKPACKETFACTORY_H
