#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QSettings>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QFile>
#include <QSystemTrayIcon>
#include <QTextStream>
#include <QUdpSocket>
#include <QMultiMap>
#include <QMessageBox>

#include "global.h"
#include "layoutitem.h"
#include "visualgrid.h"
#include "diagnosticobjectlist.h"
#include "diagnosticitem.h"
#include "settingswindow.h"
#include "xmlconfig.h"

#include "lmkpacketparser.h"
#include "lmkdiagnosticparser.h"

#include "udpsocketlistener.h"

#include "lmkpacketbuilder.h"

#include "selfdiagnostic.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void updateSettings();

private slots:
    void diagnosticTimeoutChange();
    void objectHeightChange(quint16 height);
    void fontBoldChange(bool bold);
    void journalWriteChange(bool write);

    void showHide(QSystemTrayIcon::ActivationReason);

    void button_reciever();

    void on_diagnosticItemUpdated();

    void journalTableWrite(QStringList message);

    void infoMenu(const QPoint& position);
    void objectMenu(const QPoint& position);

    void objectMenuReboot();
    void objectMenuShutdown();
    void objectMenuLogOff();
    void objectMenuSetOffCheck();
    void objectMenuSetOnCheck();
    void objectMenuForceCheck();
    void itemMenuSetOffCheck();
    void itemMenuSetOnCheck();
    void itemMenuForceCheck();
    void itemMenuServiceOff();
    void itemMenuServiceOn();

    void on_actionSelectItems_triggered(bool checked);
    void on_actionReboot_triggered();
    void on_actionShutdown_triggered();
    void on_clearJournalButton_clicked();
    void on_actionQuit_triggered();
    void on_scrollJournalButton_clicked(bool checked);

    //void on_actionJournalWrite_triggered(bool checked);

    void on_actionPreferences_triggered();
    void on_actionAbout_triggered();
    void on_actionBuild_triggered();
	void on_actionLogout_triggered();
    void on_actionSelfShutdown_triggered();

    void on_actionConfigFileChanged();

private:
    Ui::MainWindow *ui;

    VisualGrid *rootGrid;

    SettingsWindow * settingsWindow;

    DiagnosticObjectList * objectsList;
    QList<DiagnosticItem * > itemList;

    QMultiMap<QString, DiagnosticItem *> itemMap;

    qint32 clickedItemId;

    VisualObject *currentVisualObject;
    qint16 currentItemIndex;

    quint32 buttonId;

    bool journalAutoScroll;
    bool journalAutoSave;

    QSystemTrayIcon *trayIcon;

    QFile * logFile;
    QTextStream * outLog;

    QStandardItem * infoItem;
    QStandardItemModel * infoModel;

    QStandardItem *journalItem;
    QStandardItemModel * journalModel;

    XmlConfig *xmlConfig;

    LmkPacketBuilder *factory;
	QUdpSocket *sendSocket;
	UdpSocketListener *udpListener;

    QMessageBox *messageBox;

	LmkPacketParser *packetParser;
	LmkDiagnosticParser *diagnosticParser;

    SelfDiagnostic *selfDiagnostic;

    int journalIndex;

    void loadMain(QString name);
    void initDiagnostic(QString name);
    void initInfoTable();
    void initJournalTable();

    void populateInfoTable();

    void getItemHeight();

    void clearAll();

protected:
    virtual void closeEvent(QCloseEvent *event);

private slots:
    void datagramSender(Global::Packet::Type type, QStringList addressList);

    void on_actionLoadConfig_triggered();
    void on_actionSaveConfig_triggered();
    void on_actionSelfReboot_triggered();
};

#endif	// MAINWINDOW_H
