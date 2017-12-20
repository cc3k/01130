#include "visualgroup.h"
#include "global.h"


#include <QDebug>

VisualGroup::VisualGroup(QObject *parent, QString name) : LayoutItem(parent, name)
{
    this->widget = new QGroupBox(name);
    //this->widget->setFlat(true);
    //this->widget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    //this->widget->setCheckable(true);
    //this->widget->setChecked(false);
    QVBoxLayout *vbox = new QVBoxLayout(widget);
    boxGrid = new QGridLayout();
    vbox->addLayout(boxGrid);
    QWidget *emptycell = new QWidget();
    emptycell->resize(0,0);
    boxGrid->addWidget(emptycell, 0, 0);
    boxGrid->addWidget(emptycell, 1, 0);
    boxGrid->addWidget(emptycell, 2, 0);
    boxGrid->addWidget(emptycell, 3, 0);
    this->bold = false;
    setNa();
}

QList<VisualObject *> VisualGroup::getObjectList() const
{
    return objectList;
}

void VisualGroup::setObjectList(const QList<VisualObject *> &value)
{
    objectList = value;
}

Global::State::states VisualGroup::getState() const
{
    return state;
}

void VisualGroup::setState(const Global::State::states value)
{
    state = value;

    if (state == Global::State::good)
    {
        setGood();
    }

    if (state == Global::State::bad)
    {
        setBad();
    }

    if (state == Global::State::average)
    {
        setAverage();
    }

    if (state == Global::State::offcheck)
    {
        setOffcheck();
    }
}

void VisualGroup::setBad()
{
    state = Global::State::bad;
    if (bold)
    {
        this->widget->setStyleSheet("QGroupBox { font-weight: bold; color: red } ");
    }
    else
    {
        this->widget->setStyleSheet("QGroupBox { color: red; } ");
    }
    //emit stateChanged();

}

void VisualGroup::setGood()
{
    state = Global::State::good;
    if (bold)
    {
        this->widget->setStyleSheet("QGroupBox { font-weight: bold; color: green } ");
    }
    else
    {
        this->widget->setStyleSheet("QGroupBox { color: green; } ");
    }//emit stateChanged();

}

void VisualGroup::setAverage()
{
    state = Global::State::average;

    if (bold)
    {
        this->widget->setStyleSheet("QGroupBox { font-weight: bold; color: orange } ");
    }
    else
    {
        this->widget->setStyleSheet("QGroupBox { color: orange; } ");
    }
    //emit stateChanged();
}

void VisualGroup::setOffcheck()
{
    state = Global::State::offcheck;

    if (bold)
    {
        this->widget->setStyleSheet("QGroupBox { font-weight: bold; color: grey } ");
    }
    else
    {
        this->widget->setStyleSheet("QGroupBox { color: grey; } ");
    }
    //this->widget->update();
    //emit stateChanged();
}

void VisualGroup::setNa()
{
    state = Global::State::na;

    if (bold)
    {
        this->widget->setStyleSheet("QGroupBox { font-weight: bold; color: grey } ");
    }
    else
    {
        this->widget->setStyleSheet("QGroupBox { color: grey; } ");
    }
}

void VisualGroup::setNone()
{
    state = Global::State::none;

    if (bold)
    {
        this->widget->setStyleSheet("QGroupBox { font-weight: bold; color: black } ");
    }
    else
    {
        this->widget->setStyleSheet("QGroupBox { color: black; } ");
    }
}

void VisualGroup::setAlignment(int alignment)
{
    this->widget->setAlignment(alignment);
}

int VisualGroup::getAlignment() const
{
    return this->widget->alignment();
}

bool VisualGroup::getBold() const
{
    return bold;
}

QString VisualGroup::getNone() const
{
    if (state == Global::State::none)
    {
        return "true";
    }
    return "false";
}


void VisualGroup::setBold(bool value)
{
    //qDebug() << "group setBold called";

    bold = value;

    if (bold)
    {
        if (state == Global::State::good)
        {
            this->widget->setStyleSheet("QGroupBox { font-weight: bold; color: green } ");
        }

        if (state == Global::State::bad)
        {
            this->widget->setStyleSheet("QGroupBox { font-weight: bold; color: red } ");
        }

        if (state == Global::State::average)
        {
            this->widget->setStyleSheet("QGroupBox { font-weight: bold; color: orange } ");
        }

        if (state == Global::State::offcheck)
        {
            this->widget->setStyleSheet("QGroupBox { font-weight: bold; color: grey } ");
        }

        if (state == Global::State::na)
        {
            this->widget->setStyleSheet("QGroupBox { font-weight: bold; color: grey } ");
        }

        if (state == Global::State::none)
        {
            this->widget->setStyleSheet("QGroupBox { font-weight: bold; color: black } ");
        }
    }
    else
    {
        if (state == Global::State::good)
        {
            this->widget->setStyleSheet("QGroupBox { color: green } ");
        }

        if (state == Global::State::bad)
        {
            this->widget->setStyleSheet("QGroupBox { color: red } ");
        }

        if (state == Global::State::average)
        {
            this->widget->setStyleSheet("QGroupBox { color: orange } ");
        }

        if (state == Global::State::offcheck)
        {
            this->widget->setStyleSheet("QGroupBox { color: grey } ");
        }

        if (state == Global::State::na)
        {
            this->widget->setStyleSheet("QGroupBox { color: grey } ");
        }

        if (state == Global::State::none)
        {
            this->widget->setStyleSheet("QGroupBox { color: black } ");
        }
    }
}

void VisualGroup::remove()
{
    //qDebug() << "remove visualgroup";
    widget->deleteLater();
    delete this;
}

void VisualGroup::mouseReleaseEvent()
{
    qDebug() << this->name << " clicked";
}

void VisualGroup::changeState()
{    
    if (this->state == Global::State::none)
    {
        return;
    }

    Global::State::states result = Global::State::na;
    int size = childSet.size();
    int good = 0;
    int average = 0;
    int bad = 0;
    int offcheck = 0;
    int none = 0;
    int na = 0;

    for (int i = 0; i < size; ++i)
    {
        switch (childSet.at(i)->getState())
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
