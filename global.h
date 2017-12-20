#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>

class Global
{
public:
    static QString SysDiagnosticAddress;
    static quint16 SysDiagnosticPortSend;
    static quint16 SysDiagnosticPortListen;
    static QString DiagnosticAddress;
    static quint16 DiagnosticPortSend;
    static quint16 RemoteControlPort;

    struct Packet
    {
        enum UdpProto
        {
            unicast, multicast, noproto
        };

        struct Info
        {
            Global::Packet::UdpProto proto;
            QString srcAddr;
            QString dstAddr;
            quint16 port;
        };

        enum Type
        {
            sysdiagnostic, diagnostic, logoff, reboot, shutdown, data, notype
        };

        struct Struct
        {
            QByteArray PacketSignature;     //3 bytes
            QByteArray PacketLength;        //2 bytes
            QByteArray PacketCycleCounter;  //2 bytes
            QByteArray PacketId;            //2 bytes
            QByteArray PacketSysId;         //1 byte
            QByteArray PacketSubSysId;      //1 byte
            //11 bytes summary  header
            QByteArray PacketData;          //n bytes
        };
    };

    struct PacketBundle
    {
        Global::Packet::Type type;
        Global::Packet::Info net;
        Global::Packet::Struct packet;
        QByteArray data;
    };

    class State
    {
    public:
        enum states
        {
            good, bad, average, offcheck, na, none
        };

        static QString state(Global::State::states state);
    };

private:
    Global();

};

#endif // GLOBAL_H
