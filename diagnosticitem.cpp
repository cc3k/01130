#include "diagnosticitem.h"

DiagnosticItem::DiagnosticItem(QObject *parent, QString name) : QObject(parent)
{
    id = -1;
    parentId = -1;
    stateTimeout = 3000;
    this->name = name;
    this->state = Global::State::na;
    this->ip = "";
    timestamp = QDateTime::currentDateTime();
    timestampMsec = QDateTime::currentMSecsSinceEpoch();
    emit stateChanged();
    stateTimer = new QTimer(this);
    connect(stateTimer, &QTimer::timeout, this, &DiagnosticItem::checkTimeout);
    stateTimer->start(400);
}

DiagnosticItem::~DiagnosticItem()
{

}

QString DiagnosticItem::getIp() const
{
    return ip;
}

void DiagnosticItem::setIp(const QString &value)
{
    ip = value;
}

quint8 DiagnosticItem::getSubSys() const
{
    return subsys;
}

void DiagnosticItem::setSubSys(const quint8 &value)
{
    subsys = value;
}

quint8 DiagnosticItem::getSys() const
{
    return sys;
}

void DiagnosticItem::setSys(const quint8 &value)
{
    sys = value;
}

quint8 DiagnosticItem::getHsubSys() const
{
    return hsubsys;
}

void DiagnosticItem::setHsubSys(const quint8 &value)
{
    hsubsys = value;
}

quint8 DiagnosticItem::getHsys() const
{
    return hsys;
}

void DiagnosticItem::setHsys(const quint8 &value)
{
    hsys = value;
}

Global::State::states DiagnosticItem::getState() const
{
    return state;
}

void DiagnosticItem::setState(const Global::State::states value)
{
    oldState = state;
    state = value;
    timestamp = QDateTime::currentDateTime();
    timestampMsec = QDateTime::currentMSecsSinceEpoch();

    if (state != oldState)
    {
        emit stateChanged();

        QString infoString;
        infoString = "Переход из состояния " + Global::State::state(oldState).toLower() + " в " + Global::State::state(state).toLower();
        message << infoString << this->name << QString::number(parentId);
        emit logWrite(message);
        message.clear();

    }

    //!!
    emit stateChanged();  //если удалить то время в информации по блоку будет статично
    //!!

    //tst1
    //startStateCounter();
}

QString DiagnosticItem::getName() const
{
    return name;
}

void DiagnosticItem::setName(const QString &value)
{
    name = value;
}

QDateTime DiagnosticItem::getTimestamp() const
{
    return timestamp;
}

Global::State::states DiagnosticItem::getOldState() const
{
    return oldState;
}

void DiagnosticItem::changeStateTimeout()
{

    QString infoString;

    oldState = this->state;
    this->state = Global::State::bad;
    timestamp = QDateTime::currentDateTime();
    emit stateChanged();
    infoString = "Переход из состояния " + Global::State::state(oldState).toLower() + " в " + Global::State::state(state).toLower();
    message << infoString << this->name << QString::number(parentId);
    emit logWrite(message);
    message.clear();

    //qDebug() << name;
    //qDebug() << BrkStatic::State::getStringState(BrkStatic::State::bad);
    //qDebug() << " due to timeout";

    //qDebug() << BrkStatic::State::getStringState(BrkStatic::State::bad);

    //QString oldStateString;
    //QString stateString;

}

QString DiagnosticItem::getStateText() const
{
    return stateText;
}

void DiagnosticItem::setStateText(const QString &value)
{
    stateText = value;
}

quint32 DiagnosticItem::getStateTimeout() const
{
    return stateTimeout;
}

void DiagnosticItem::setStateTimeout(const quint32 &value)
{
    stateTimeout = value;
}

void DiagnosticItem::remove()
{
    //qDebug() << "remove diagnostic item";
    stateTimer->stop();
    disconnect(stateTimer, &QTimer::timeout, this, &DiagnosticItem::checkTimeout);
    stateTimer->deleteLater();
    delete this;
}

qint32 DiagnosticItem::getParentId() const
{
    return parentId;
}

void DiagnosticItem::setParentId(const qint32 &value)
{
    parentId = value;
}

qint32 DiagnosticItem::getId() const
{
    return id;
}

void DiagnosticItem::setId(const qint32 &value)
{
    id = value;
}

void DiagnosticItem::startStateCounter()
{
    if ((state == Global::State::bad) || (state == Global::State::offcheck)) {}
    else
    {
        //QTimer::singleShot(stateTimeout, this, SLOT(changeStateTimeout()));
    }
}

void DiagnosticItem::checkTimeout()
{
    if ((state == Global::State::bad) || (state == Global::State::offcheck)) {}
    else
    {
        //qDebug() << QDateTime::currentMSecsSinceEpoch() - timestampMsec > stateTimeout;
        if (QDateTime::currentMSecsSinceEpoch() - timestampMsec > stateTimeout) //ТУТ ПРОВЕРЯТЬ ТАЙМАУТ ВСТАВИТЬ ИЗ КУСЕТТИНГЗ
        {
            changeStateTimeout();
        }
    }
}
