#include "lmkdiagnosticparser.h"

#include <QDataStream>

LmkDiagnosticParser::LmkDiagnosticParser(QObject *parent) : QObject(parent)
{

}

void LmkDiagnosticParser::setItemMap(const QMultiMap<QString, DiagnosticItem *> &value)
{
    itemMap = value;
}

void LmkDiagnosticParser::diagnosticPacket(UdpDatagram datagram)
{
    double chunkSizeD = (double) (datagram.getData().size() - 11) / 3;
    uint chunkSizeI = (datagram.getData().size() - 11) / 3;

//    qDebug() << chunkSizeD;
//    qDebug() << chunkSizeI;

    if (chunkSizeD - chunkSizeI != 0)
    {
        qDebug() << "not 3 bytes chunk";
    }

//  так могу вытащить ИД
    QDataStream dataStream(datagram.getData().mid(7, 2));
    quint8 byte0;
    quint8 byte1;
    dataStream >> byte0 >> byte1;
    quint16 id = (byte0 << 8) + byte1;

    QString ip = datagram.getHost().toString();

    if (chunkSizeD - chunkSizeI == 0 && id == 0) //запилить проверку по ID и по ip (забыл сделать)
    {
        for (uint i = 0; i < chunkSizeI; ++i)
        {
            QString headSys = QString::number(datagram.getData()[9]);
            QString headSubsys = QString::number(datagram.getData()[10]);
            QString sys = QString::number(datagram.getData()[11+i*3]);
            QString subsys = QString::number(datagram.getData()[12+i*3]);
            uint state = datagram.getData()[13+i*3];

            QString key;

            QString keyWoIp = headSys + "|" + headSubsys + "|" + sys + "|" + subsys + "|";
            QString keyIp= headSys + "|" + headSubsys + "|" + sys + "|" + subsys + "|" + ip;

            if (itemMap.contains(keyWoIp))
            {
                key = keyWoIp;
            }

            if (itemMap.contains(keyIp))
            {
                key = keyIp;
            }

            //qDebug() << "key " << key << " state " << state;

            QList<DiagnosticItem *> items = itemMap.values(key);

            for (int i = 0; i < items.size(); ++i)
            {
                qDebug() << items.at(i)->getName();
                switch (state) {
                case 2:
                    //qDebug() << "good";
                    if (items.at(i)->getState() != Global::State::offcheck)
                    {
                        items.at(i)->setState(Global::State::good);
                    }
                    break;

                case 1:
                    //qDebug() << "average";
                    if (items.at(i)->getState() != Global::State::offcheck)
                    {
                        items.at(i)->setState(Global::State::average);
                    }
                    break;

                case 0:
                    //qDebug() << "bad";
                    if (items.at(i)->getState() != Global::State::offcheck)
                    {
                        items.at(i)->setState(Global::State::bad);
                    }
                    break;

                default:
                    break;
                }
            }
        }
    }
}
