#ifndef LMKDIAGNOSTICPARSER_H
#define LMKDIAGNOSTICPARSER_H

#include <QObject>
#include <QMultiMap>

#include "diagnosticitem.h"
#include "udpdatagram.h"

class LmkDiagnosticParser : public QObject
{
	Q_OBJECT
public:
	explicit LmkDiagnosticParser(QObject *parent = 0);

	void setItemMap(const QMultiMap<QString, DiagnosticItem *> &value);

private:
	QMultiMap<QString, DiagnosticItem*> itemMap;

signals:

public slots:
    void diagnosticPacket(UdpDatagram datagram);
};

#endif // LMKDIAGNOSTICPARSER_H
