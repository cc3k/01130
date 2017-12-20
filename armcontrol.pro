#-------------------------------------------------
#
# Project created by QtCreator 2016-01-12T10:47:14
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = armcontrol
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    diagnosticitem.cpp \
    layoutitem.cpp \
    visualgrid.cpp \
    visualgroup.cpp \
    visualobject.cpp \
    diagnosticobjectlist.cpp \
    settingswindow.cpp \
    xmlconfig.cpp \
    udpsocketlistener.cpp \
	udpdatagram.cpp \
    lmkpacketparser.cpp \
    lmkdiagnosticparser.cpp \
	global.cpp \
    selfdiagnostic.cpp \
    lmkpacketbuilder.cpp

HEADERS  += mainwindow.h \
    diagnosticitem.h \
    layoutitem.h \
    visualgrid.h \
    visualgroup.h \
    visualobject.h \
    diagnosticobjectlist.h \
    settingswindow.h \
    xmlconfig.h \
    udpsocketlistener.h \
	udpdatagram.h \
    lmkpacketparser.h \
    lmkdiagnosticparser.h \
	global.h \
    selfdiagnostic.h \
    lmkpacketbuilder.h

FORMS    += mainwindow.ui \
    settingswindow.ui \

RESOURCES += \
        resources.qrc

RC_FILE = armcontrol.rc
