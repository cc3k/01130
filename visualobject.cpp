#include "visualobject.h"

#include <QVariant>
#include <QIcon>

VisualObject::VisualObject(QObject *parent, QString name) : LayoutItem(parent, name)
{
    this->item = new QPushButton(" " + name);
    this->name = name;
    this->bold = false;
    item->setObjectName("mQButton");
    this->state = Global::State::na;
    this->oldState = Global::State::na;
    this->item->setAutoFillBackground(true);
    this->item->setIcon(QIcon(":/icons/circle_white.png"));
    setNa();
}

Global::State::states VisualObject::getState() const
{
    return state;
}

void VisualObject::setState(const Global::State::states &value)
{
    if (state != value)
    {
        if (value == Global::State::good)
        {
            setGood();
        }

        if (value == Global::State::bad)
        {
            setBad();
        }

        if (value == Global::State::average)
        {
            setAverage();
        }

        if (value == Global::State::offcheck)
        {
            setOffcheck();
        }

        if (value == Global::State::na)
        {
            setNa();
        }
    }
}

QString VisualObject::getRole() const
{
    return role;
}

void VisualObject::setRole(const QString &value)
{
    role = value;
}

void VisualObject::setGood()
{
    if (this->state != Global::State::good)
    {
        this->item->setAutoFillBackground(true);
        if (bold)
        {
            this->item->setStyleSheet("#mQButton { font-weight: bold; color: green; } ");
        }
        else
        {
            this->item->setStyleSheet("#mQButton { color: green;} ");
        }
        oldState = this->state;
        this->state = Global::State::good;
        this->item->setIcon(QIcon(":/icons/circle_green.png"));
        emit stateChanged();
        QString infoString = "Переход из состояния " + Global::State::state(oldState).toLower() + " в " + Global::State::state(state).toLower();
        message << infoString << this->name << QString::number(parentId);
        emit logWrite(message);
        message.clear();
    }
}

void VisualObject::setBad()
{
    if (this->state != Global::State::bad)
    {
        if (bold)
        {
            this->item->setStyleSheet("#mQButton { font-weight: bold; color: red; } ");
        }
        else
        {
            this->item->setStyleSheet("#mQButton { color: red;} ");
        }
        oldState = this->state;
        this->state = Global::State::bad;
        this->item->setIcon(QIcon(":/icons/circle_red.png"));
        emit stateChanged();
        QString infoString = "Переход из состояния " + Global::State::state(oldState).toLower() + " в " + Global::State::state(state).toLower();
        message << infoString << this->name << QString::number(parentId);
        emit logWrite(message);
        message.clear();
    }
}

void VisualObject::setAverage()
{
    if (this->state != Global::State::average)
    {
        if (bold)
        {
            this->item->setStyleSheet("#mQButton { font-weight: bold; color: orange; } ");
        }
        else
        {
            this->item->setStyleSheet("#mQButton { color: orange;} ");
        }
        oldState = this->state;
        this->state = Global::State::average;
        this->item->setIcon(QIcon(":/icons/circle_orange.png"));
        emit stateChanged();
        QString infoString = "Переход из состояния " + Global::State::state(oldState).toLower() + " в " + Global::State::state(state).toLower();
        message << infoString << this->name << QString::number(parentId);
        emit logWrite(message);
        message.clear();
    }
}

void VisualObject::setOffcheck()
{
    if (this->state != Global::State::offcheck)
    {
        if (bold)
        {
            this->item->setStyleSheet("#mQButton { font-weight: bold; color: grey; } ");
        }
        else
        {
            this->item->setStyleSheet("#mQButton { color: grey;} ");
        }
        oldState = this->state;
        this->state = Global::State::offcheck;
        this->item->setIcon(QIcon(":/icons/circle_grey.png"));
        emit stateChanged();
        QString infoString = "Переход из состояния " + Global::State::state(oldState).toLower() + " в " + Global::State::state(state).toLower();
        message << infoString << this->name << QString::number(parentId);
        emit logWrite(message);
        message.clear();
    }
}

void VisualObject::setNa()
{
    if (this->state != Global::State::na)
    {
        if (bold)
        {
            this->item->setStyleSheet("#mQButton { font-weight: bold; color: grey; } ");
        }
        else
        {
            this->item->setStyleSheet("#mQButton { color: grey;} ");
        }
        oldState = this->state;
        this->state = Global::State::na;
        this->item->setIcon(QIcon(":/icons/circle_grey.png"));
        //this->item->setIcon(QIcon(":/icons/blank.png"));
        emit stateChanged();
        QString infoString = "Переход из состояния " + Global::State::state(oldState).toLower() + " в " + Global::State::state(state).toLower();
        message << infoString << this->name << QString::number(parentId);
        emit logWrite(message);
        message.clear();
    }

}

void VisualObject::setNone()
{
    if (this->state != Global::State::none)
    {
        if (bold)
        {
            this->item->setStyleSheet("#mQButton { font-weight: bold; color: black; } ");
        }
        else
        {
            this->item->setStyleSheet("#mQButton { color: black;} ");
        }
        oldState = this->state;
        this->state = Global::State::none;
        this->item->setIcon(QIcon(":/icons/blank.png"));
        emit stateChanged();
        QString infoString = "Переход из состояния " + Global::State::state(oldState).toLower() + " в " + Global::State::state(state).toLower();
        message << infoString << this->name << QString::number(parentId);
        emit logWrite(message);
        message.clear();
    }
}

void VisualObject::setId(const qint16 &value)
{
    this->id = value;
    this->item->setProperty("id", value);
    //чойта здесь нечисто с присвоением проперти :) без include QVariant не пашет
}

QList<DiagnosticItem *> VisualObject::getDiagnosticItemList() const
{
    return diagnosticItemList;
}

void VisualObject::addDiagnosticItem(DiagnosticItem *item)
{
   this->diagnosticItemList.append(item);
}

bool VisualObject::getBold() const
{
    return bold;
}

void VisualObject::setBold(bool value)
{
    bold = value;

    if (bold)
    {
        if (this->state == Global::State::offcheck)
        {
            this->item->setStyleSheet("#mQButton { font-weight: bold; color: grey; } ");
        }

        if (this->state == Global::State::good)
        {
            this->item->setStyleSheet("#mQButton { font-weight: bold; color: green; } ");
        }

        if (this->state == Global::State::bad)
        {
            this->item->setStyleSheet("#mQButton { font-weight: bold; color: red; } ");
        }

        if (this->state == Global::State::average)
        {
            this->item->setStyleSheet("#mQButton { font-weight: bold; color: orange; } ");
        }

    }
    else
    {
        if (this->state == Global::State::offcheck)
        {
            this->item->setStyleSheet("#mQButton { color: grey; } ");
        }

        if (this->state == Global::State::good)
        {
            this->item->setStyleSheet("#mQButton { color: green; } ");
        }

        if (this->state == Global::State::bad)
        {
            this->item->setStyleSheet("#mQButton { color: red; } ");
        }

        if (this->state == Global::State::average)
        {
            this->item->setStyleSheet("#mQButton { color: orange; } ");
        }
    }

    this->item->update();
}

void VisualObject::changeState()
{
    Global::State::states result = Global::State::na;
    int size = diagnosticItemList.size();
    int good = 0;
    int average = 0;
    int bad = 0;
    int offcheck = 0;
    int none = 0;
    int na = 0;

    for (int i = 0; i < size; ++i)
    {
        switch (diagnosticItemList.at(i)->getState())
        {
        case Global::State::good:
            ++good;
            break;

        case Global::State::average:
            ++average;
            break;

        case Global::State::bad:
            ++bad;
            break;

        case Global::State::offcheck:
            ++offcheck;
            break;

        case Global::State::na:
            ++na;
            break;

        case Global::State::none:
            ++none;
            break;

        default:
            break;
        }
    }

    if (offcheck == size)
    {
        result = Global::State::offcheck;
    }

    if (none == size)
    {
        result = Global::State::none;
    }

    if (na == size)
    {
        result = Global::State::na;
    }

    if ((good > 0) && (bad == 0 && average == 0))
    {
        result = Global::State::good;
    }

    if ((average > 0) && (bad == 0))
    {
        result = Global::State::average;
    }

    if (bad > 0)
    {
        result = Global::State::bad;
    }

    setState(result);
}

void VisualObject::remove()
{
    //qDebug() << "remove visualobject";
    item->deleteLater();
    delete this;
}
