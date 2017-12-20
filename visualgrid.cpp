#include "visualgrid.h"

VisualGrid::VisualGrid(QObject *parent, QString name) : LayoutItem (parent, name)
{
    // , quint8 rowcount, quint8 columncount раньше это было в конструкторе !!! хз зачем
   // qDebug() << "VisualGrid constructor for" << this->name << "rows: " << rowcount << " columns: " << columncount;

    this->layout = new QGridLayout();
    this->layout->setObjectName(name);

//    QWidget *emptycell = new QWidget();
//    for (int i = 0; i < (rowcount + 1); ++i)
//    {
//        for (int j = 0; j < (columncount + 1); ++j)
//        {
//            layout->addWidget(emptycell, i, j);
//        }
    //    }
}

void VisualGrid::remove()
{
    //qDebug() << "remove visualgrid";
    layout->deleteLater();
    delete this;
}
