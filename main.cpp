#include "mainwindow.h"

#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    QTranslator qtTranslator;
        if (qtTranslator.load("qt_ru", QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
        {
            a.installTranslator(&qtTranslator);
        }

	w.showMaximized();
    w.updateSettings();

    return a.exec();
}

//QApplication::setQuitOnLastWindowClosed(false); //чтобы не выходила по кресту на окне
