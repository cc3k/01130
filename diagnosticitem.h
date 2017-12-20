#ifndef DIAGNOSTICITEM_H
#define DIAGNOSTICITEM_H

#include <global.h>

#include <QObject>
#include <QDateTime>
#include <QTimer>

class DiagnosticItem : public QObject
{
    Q_OBJECT
public:
    explicit DiagnosticItem(QObject *parent = 0, QString name = "empty");
    ~DiagnosticItem();

    QString getIp() const;
    void setIp(const QString &value);

    quint8 getSubSys() const;
    void setSubSys(const quint8 &value);

    quint8 getSys() const;
    void setSys(const quint8 &value);

    quint8 getHsubSys() const;
    void setHsubSys(const quint8 &value);

    quint8 getHsys() const;
    void setHsys(const quint8 &value);

    Global::State::states getState() const;
    void setState(const Global::State::states value);

    QString getName() const;
    void setName(const QString &value);

	QDateTime getTimestamp() const;

    Global::State::states getOldState() const;

    qint32 getId() const;
    void setId(const qint32 &value);

    qint32 getParentId() const;
    void setParentId(const qint32 &value);

    quint32 getStateTimeout() const;
    void setStateTimeout(const quint32 &value);

    void remove();

    QString getStateText() const;
    void setStateText(const QString &value);

signals:
    void stateChanged();
    void logWrite(QStringList);

public slots:
    void changeStateTimeout();

private:
    qint32 id;
    qint32 parentId;
    QString name;
    QString stateText;
	QDateTime timestamp;
	qint64 timestampMsec;
    quint8 hsys;
    quint8 hsubsys;
    quint8 sys;
    quint8 subsys;
    QString ip;
    Global::State::states state;
    Global::State::states oldState;

    quint32 stateTimeout;

	QTimer *stateTimer;

    QStringList message;

    void startStateCounter();

public slots:

private slots:
	void checkTimeout();

};

#endif // DIAGNOSTICITEM_H
