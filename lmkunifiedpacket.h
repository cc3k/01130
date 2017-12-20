#ifndef LMKUNIFIEDPACKET_H
#define LMKUNIFIEDPACKET_H

#include <QString>
#include <QByteArray>

#include "global.h"

class LmkUnifiedPacket
{
public:
	LmkUnifiedPacket();
	~LmkUnifiedPacket();

    Global::Packet::Type type;
    Global::Packet::Info net;
    Global::Packet packet;
};

#endif // LMKUNIFIEDPACKET_H
