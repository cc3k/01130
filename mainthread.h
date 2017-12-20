#ifndef MAINTHREAD_H
#define MAINTHREAD_H

#include <QObject>

#include <QGridLayout>

#include "brkstatic.h"

class MainThread
{
public:
    MainThread();

    void init();

     QGridLayout* getGrid() const;

};

#endif // MAINTHREAD_H
