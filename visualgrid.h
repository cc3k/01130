#ifndef VISUALGRID_H
#define VISUALGRID_H

#include <QGridLayout>

#include "layoutitem.h"

class VisualGrid : public LayoutItem
{
public:
    VisualGrid(QObject *parent = 0, QString name = "empty");

    void remove();
};

#endif // VISUALGRID_H
