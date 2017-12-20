#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QBoxLayout>
#include <QPushButton>
#include <QGridLayout>
#include <QDir>
#include <QFile>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QLabel>
#include <QMessageBox>
#include <QCloseEvent>
#include <QLineEdit>
#include <QMessageBox>
#include <QFileDialog>

//#include <QTest> add Qt += testlib

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowIcon(QIcon(":/icons/a108.png"));

    journalIndex = 1;

    //setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint);//убирает свернуть и развернуть

    trayIcon = new QSystemTrayIcon();
    QIcon * icon = new QIcon(":/icons/a108.png");

    trayIcon->setIcon(*icon);
    trayIcon->show();

    connect(trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::showHide);

    QSettings settings("settings.ini", QSettings::IniFormat);

    QString configName;

    if (settings.value("config_file").isValid())
    {
        configName = settings.value("config_file").toString();
    }
    else
    {
        configName = "config.xml";
        settings.setValue("config_file", "config.xml");
    }

    QFile *file = new QFile(configName);
    if (!file->exists())
       {
           configName = "config.xml";
       }
    delete file;

    loadMain(configName);

    selfDiagnostic = new SelfDiagnostic;
}

MainWindow::~MainWindow()
{
    delete ui;
    trayIcon->hide();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    int r = QMessageBox::question(this, "Подтвердите", "Закрыть приложение?", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

    if (r == QMessageBox::Yes)
        event->accept();
    else
        event->ignore();
}

void MainWindow::datagramSender(Global::Packet::Type type, QStringList addressList)
{
    QStringList message;
    QString parent;
    QString source;
    QString result;
    QString datagramInfo;
    const QString OK = "УСПЕШНО!";
    const QString ERR = "ОШИБКА!";
    const QString LOGOUT = "отправка комманды завершения сеанса - ";
    const QString REBOOT = "отправка комманды перезагрузки - ";
    const QString SHUTDOWN = "отправка комманды выключения - ";

    switch (type)
    {
    case Global::Packet::logoff:
        result = LOGOUT;
        break;

    case Global::Packet::reboot:
        result = REBOOT;
        break;

    case Global::Packet::shutdown:
        result = SHUTDOWN;
        break;

    default:
        break;
    }

    for (int i = 0; i < addressList.size(); ++i)
    {
        if (addressList.at(i) == "")
            return;

        sendSocket = new QUdpSocket(this);

        QSettings settings("settings.ini", QSettings::IniFormat);
        sendSocket->bind(QHostAddress(settings.value("ip").toString()));

        factory = new LmkPacketBuilder;
        factory->createBundle(type);
        factory->setDstAddr(addressList.at(i));

        for (int j = 0; j < itemList.size(); ++j)
        {
            if (itemList.at(j)->getIp() == addressList.at(i))
            {
                source = itemList.at(j)->getName();
                //qDebug() << source;
                parent = QString::number(itemList.at(j)->getParentId());
                //qDebug() << parent;
            }
        }

        if (sendSocket->writeDatagram(factory->getPacketBundle()->data.data(),
                                      factory->getPacketBundle()->data.size(),
                                      QHostAddress(factory->getPacketBundle()->net.dstAddr),
                                      factory->getPacketBundle()->net.port) != -1)
        {
            datagramInfo.append(objectsList->getItemById(parent.toInt())->getName() + " [" + factory->getPacketBundle()->net.dstAddr +
                                ":" + QString::number(factory->getPacketBundle()->net.port) + "]" +
                                " - " + OK + "\n");
            message.append(result + OK);
            message.append(source);
            message.append(parent);
        }
        else
        {
            datagramInfo.append(objectsList->getItemById(parent.toInt())->getName() + " [" + factory->getPacketBundle()->net.dstAddr +
                                ":" + QString::number(factory->getPacketBundle()->net.port) + "]" +
                                " - " + ERR + "\n");
            message.append(result + ERR);
            message.append(source);
            message.append(parent);
        }

        journalTableWrite(message);

        delete factory;
        sendSocket->close();
        delete sendSocket;
        message.clear();
        //QTest::qWait(10);
        //QThread::usleep(5);
    }

    messageBox->setDetailedText(datagramInfo);
    datagramInfo.clear();
}

void MainWindow::updateSettings()
{
    QSettings settings("settings.ini", QSettings::IniFormat);

    for (int i = 0; i < objectsList->getObjectList().size(); i++)
    {
        objectsList->getObjectList().at(i)->getItem()->setMinimumHeight(settings.value("object_height").toInt());
    }
}

void MainWindow::diagnosticTimeoutChange()
{
    QSettings settings("settings.ini", QSettings::IniFormat);

    for (int i = 0; i < itemList.size(); i++)
    {
        this->itemList.at(i)->setStateTimeout(settings.value("diagnostic_timeout").toInt());
        //qDebug() << QString::number(this->itemList.at(i)->getStateTimeout());
    }
}

void MainWindow::objectHeightChange(quint16 height)
{
    QSettings settings("settings.ini", QSettings::IniFormat);
    settings.setValue("object_height", height);

    for (int i = 0; i < objectsList->getObjectList().size(); i++)
    {
        objectsList->getObjectList().at(i)->getItem()->setMinimumHeight(height);
    }
}

void MainWindow::fontBoldChange(bool bold)
{
    QSettings settings("settings.ini", QSettings::IniFormat);
    settings.setValue("font_bold", bold);

    for (int i = 0; i < objectsList->getObjectList().size(); i++)
    {
        objectsList->getObjectList().at(i)->setBold(bold);
    }

    for (int i = 0; i < objectsList->getGroupList().size(); i++)
    {
        objectsList->getGroupList().at(i)->setBold(bold);
    }
}

void MainWindow::journalWriteChange(bool write)
{
    QSettings settings("settings.ini", QSettings::IniFormat);

    if (!settings.value("journal_dir").isValid())
    {
        settings.setValue("journal_dir",QApplication::applicationDirPath() + "/Журнал");
    }

    if (write)
    {
        journalAutoSave = true;
        settings.setValue("autosave", true);

        logFile = new QFile();
        logFile->setFileName(settings.value("journal_dir").toString() + "/" + QDateTime::currentDateTime().toString("dd.MM.yy_hh.mm.ss") + ".txt"); //тут еще csv!!!
        if (!logFile->open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QDir *logDir = new QDir;
            logDir->mkdir(settings.value("journal_dir").toString());//для винды путь работает
            logFile->open(QIODevice::WriteOnly | QIODevice::Text);
        }
        QTextStream outLog(logFile);
        //outLog << (QString(tr("Запись журнала начата ")) + QDateTime::currentDateTime().toString("dd.MM.yy hh:mm:ss") + "...\n");
    }
    else
    {
        journalAutoSave = false;
        settings.setValue("autosave", false);
    }
}

void MainWindow::showHide(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::Trigger)
    {
        //если нажато левой кнопкой продолжаем
        if (!this->isVisible())	//если окно было не видимо - отображаем его
            this->show();
        else
            this->hide();	//иначе скрываем
    }
}

void MainWindow::initDiagnostic(QString name)
{
    xmlConfig = new XmlConfig(name);
    xmlConfig->readConfig();

    objectsList = new DiagnosticObjectList;
    itemList.clear();

    objectsList = xmlConfig->getObjectsList();
    itemList = xmlConfig->getItemList();

    for (int i = 0; i < objectsList->getLayoutItemList().size(); i++)	//добавляем вычислители к группам
    {
        if (objectsList->getLayoutItemList().at(i)->getParentId() != 0)
            objectsList->getItemById(objectsList->getLayoutItemList().at(i)->getParentId())->addItem(objectsList->getLayoutItemList().at(i));
    }

    for (int i = 0; i < itemList.size(); i++)
    {
        if (itemList.at(i)->getParentId() != 0)
        {
            VisualObject *object = (VisualObject *) objectsList->getItemById(itemList.at(i)->getParentId());
            object->addDiagnosticItem(itemList.at(i));
        }
    }

    for (int i = 0; i < objectsList->getObjectList().size(); i++)
    {
        quint32 parentId = objectsList->getObjectList().at(i)->getParentId();
        objectsList->getObjectList().at(i)->getItem()->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(objectsList->getObjectList().at(i)->getItem(), &QPushButton::customContextMenuRequested, this, &MainWindow::objectMenu);
        connect(objectsList->getObjectList().at(i)->getItem(), &QPushButton::clicked, this, &MainWindow::button_reciever);
        connect((VisualObject *)(objectsList->getObjectList().at(i)), &VisualObject::stateChanged,
                (VisualGroup *)(objectsList->getItemById(parentId)), &VisualGroup::changeState);
        connect((VisualObject *)(objectsList->getObjectList().at(i)), &VisualObject::logWrite, this, &MainWindow::journalTableWrite);

        for (int j = 0; j < objectsList->getObjectList().at(i)->getDiagnosticItemList().size(); j++)
        {
            connect (objectsList->getObjectList().at(i)->getDiagnosticItemList().at(
                         j), &DiagnosticItem::stateChanged, objectsList->getObjectList().at(i), &VisualObject::changeState);
        }
    }

    QSettings settings("settings.ini", QSettings::IniFormat);

    for (int i = 0; i < itemList.size(); i++)
    {
        itemList.at(i)->setStateTimeout(settings.value("diagnostic_timeout").toInt());

        connect(itemList.at(i), &DiagnosticItem::stateChanged, this, &MainWindow::on_diagnosticItemUpdated);
        connect(itemList.at(i), &DiagnosticItem::logWrite, this, &MainWindow::journalTableWrite);
    }

    QFile *config = new QFile(settings.value("config_file").toString());
    if (!config->exists())
    {
        VisualGrid *errorGrid = new VisualGrid;
        VisualObject *errorObject = new VisualObject(this, "Файл конфигурации отсутствует или поврежден! Загрузите файл конфигурации и пропишите его в настройках.");

        objectsList->addObject(errorObject);
        objectsList->addGrid(errorGrid);

        errorObject->setBold(true);
        errorGrid->addItem(errorObject);
        rootGrid = errorGrid;
    }
    else
    {
        rootGrid = xmlConfig->getRootGrid();
    }
}

void MainWindow::initInfoTable()
{
    infoItem = new QStandardItem();
    infoModel = new QStandardItemModel();

    QStringList infoTableHeader;
    infoTableHeader.append("№");
    infoTableHeader.append("Модуль");
    infoTableHeader.append("Время");
    infoTableHeader.append("Состояние");

    infoModel->setHorizontalHeaderLabels(infoTableHeader);

    ui->infoTable->setModel(infoModel);
    ui->infoTable->verticalHeader()->setVisible(false);
    ui->infoTable->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    ui->infoTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->infoTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->infoTable->setShowGrid(false);
    ui->infoTable->setGridStyle(Qt::NoPen);
    ui->infoTable->setWordWrap(false);
    ui->infoTable->setCornerButtonEnabled(false);
    ui->infoTable->horizontalHeader()->setHighlightSections(false);
    ui->infoTable->verticalHeader()->setVisible(false);
    ui->infoTable->verticalHeader()->setHighlightSections(false);
    ui->infoTable->setSortingEnabled(true);

    ui->infoTable->setColumnWidth(0, 40);
    ui->infoTable->setColumnWidth(1, 290);
    ui->infoTable->setColumnWidth(2, 100);
    ui->infoTable->horizontalHeader()->setStretchLastSection(true);
}

void MainWindow::initJournalTable()
{
    journalItem = new QStandardItem();
    journalModel = new QStandardItemModel();

    QStringList journalTableHeader;
    journalTableHeader.append("№");
    journalTableHeader.append("Время");
    journalTableHeader.append("Модуль");
    journalTableHeader.append("Расположение");
    journalTableHeader.append("Информация");

    journalModel->setHorizontalHeaderLabels(journalTableHeader);

    ui->journalTable->setModel(journalModel);
    ui->journalTable->verticalHeader()->setVisible(false);
    ui->journalTable->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    ui->journalTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->journalTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->journalTable->setShowGrid(false);
    ui->journalTable->setGridStyle(Qt::NoPen);
    ui->journalTable->setWordWrap(false);
    ui->journalTable->setCornerButtonEnabled(false);
    ui->journalTable->horizontalHeader()->setHighlightSections(false);
    ui->journalTable->verticalHeader()->setVisible(false);
    ui->journalTable->verticalHeader()->setHighlightSections(false);
    ui->journalTable->setSortingEnabled(true);

    ui->journalTable->setColumnWidth(0, 40);
    ui->journalTable->setColumnWidth(1, 100);
    ui->journalTable->setColumnWidth(2, 160);
    ui->journalTable->setColumnWidth(3, 160);
    ui->journalTable->horizontalHeader()->setStretchLastSection(true);
}

void MainWindow::populateInfoTable()
{
    if (currentVisualObject != Q_NULLPTR)
    {
        if (!currentVisualObject->getDiagnosticItemList().isEmpty())
        {
            initInfoTable();

            for (int i = 0; i < currentVisualObject->getDiagnosticItemList().size(); i++)
            {
                infoItem = new QStandardItem(QString::number(i + 1));
                infoModel->setItem(i, 0, infoItem);

                infoItem = new QStandardItem(QString(currentVisualObject->getDiagnosticItemList().at(i)->getName()));
                infoModel->setItem(i, 1, infoItem);

                infoItem =
                        new QStandardItem(QString(currentVisualObject->getDiagnosticItemList().at(i)->getTimestamp().toString(
                                                      "dd.MM.yy hh:mm:ss")));
                infoModel->setItem(i, 2, infoItem);

                Global::State::states state;
                state = currentVisualObject->getDiagnosticItemList().at(i)->getState();

                infoItem = new QStandardItem(QString(Global::State::state(state)));
                infoModel->setItem(i, 3, infoItem);
            }
        }
        else// очищаем таблицу если нет диагностических итемов

            initInfoTable();
    }
}

void MainWindow::clearAll()
{
    udpListener->blockSignals(true);
    packetParser->blockSignals(true);
    diagnosticParser->blockSignals(true);

    delete udpListener;
    delete packetParser;
    delete diagnosticParser;

    itemMap.clear();

    //udpListener = new UdpSocketListener(this, Global::Packet::multicast, Global::DiagnosticAddress, Global::SysDiagnosticPortListen);
    //packetParser = new LmkPacketParser;
    //diagnosticParser = new LmkDiagnosticParser;

    for (int i = 0; i < itemList.size(); ++i)
    {
        //qDebug() << itemList.at(i)->getName();
        itemList.at(i)->setState(Global::State::offcheck);
        itemList.at(i)->remove();
        itemList.clear();
    }

    for (int i = 0; i < objectsList->getObjectList().size(); ++i)
    {
        //qDebug() << objectsList->getObjectList().at(i)->getName();
        objectsList->getObjectList().at(i)->remove();
    }

    for (int i = 0; i < objectsList->getGroupList().size(); ++i)
    {
        //qDebug() << objectsList->getGroupList().at(i)->getName();
        objectsList->getGroupList().at(i)->remove();
    }

    for (int i = 0; i < objectsList->getGridList().size(); ++i)
    {
        //qDebug() << objectsList->getGridList().at(i)->getName();
        objectsList->getGridList().at(i)->remove();
    }

    objectsList->clear();
}

void MainWindow::infoMenu(const QPoint &position)
{
    if (ui->infoTable->indexAt(position).column() == 1)
    {
        bool onCheck = true;

        for (int i = 0; i < currentVisualObject->getDiagnosticItemList().size(); i++)
        {
            if (currentVisualObject->getDiagnosticItemList().at(i)->getName() == ui->infoTable->indexAt(position).data().toString())
            {
                currentItemIndex = i;
                if (currentVisualObject->getDiagnosticItemList().at(i)->getState() == Global::State::offcheck)
                    onCheck = false;
            }
        }

        QMenu *menu = new QMenu(this);
        menu->addAction("Выключение модуля", this, SLOT(itemMenuServiceOff()));
        menu->addAction("Включение модуля", this, SLOT(itemMenuServiceOn()));
        menu->addSeparator();
        menu->addAction("Принудительный контроль", this, SLOT(itemMenuForceCheck()));

        if (onCheck)
            menu->addAction("Снять с контроля", this, SLOT(itemMenuSetOffCheck()));
        else
            menu->addAction("Взять на контроль", this, SLOT(itemMenuSetOnCheck()));

        menu->popup(ui->infoTable->viewport()->mapToGlobal(position));
    }
}

void MainWindow::objectMenu(const QPoint &position)
{
    currentItemIndex = sender()->property("id").toInt();//быстрее получать так, задав пропертю ид, чем перебором, как строчкой выше
    currentVisualObject = (VisualObject *) (objectsList->getItemById(currentItemIndex));

    QMenu *menu = new QMenu(this);
    menu->addAction("Перезагрузить", this, SLOT(objectMenuReboot()));
    menu->addAction("Выключить", this, SLOT(objectMenuShutdown()));
    menu->addAction("Выход пользователя", this, SLOT(objectMenuLogOff()));
    menu->addSeparator();

    if (currentVisualObject->getState() != Global::State::offcheck)
        menu->addAction("Cнять с контроля", this, SLOT(objectMenuSetOffCheck()));
    else
        menu->addAction("Взять на контроль", this, SLOT(objectMenuSetOnCheck()));

    menu->popup(currentVisualObject->getItem()->mapToGlobal(position));
}

void MainWindow::objectMenuReboot()
{
    QStringList address;

    for (int i = 0; i < currentVisualObject->getDiagnosticItemList().size(); ++i)
    {
        //qDebug() << currentVisualObject->getName();
        if (currentVisualObject->getDiagnosticItemList().at(i)->getSys() == 0 &&
                currentVisualObject->getDiagnosticItemList().at(i)->getSubSys() == 0 &&
                currentVisualObject->getDiagnosticItemList().at(i)->getIp() != "")
            //qDebug() << currentVisualObject->getDiagnosticItemList().at(i)->getIp();
            //address.append(itemList.at(i)->getIp()); по моему берется левый ип
            address.append(currentVisualObject->getDiagnosticItemList().at(i)->getIp());
    }

    messageBox = new QMessageBox;
    datagramSender(Global::Packet::reboot, address);
    address.clear();
}

void MainWindow::objectMenuShutdown()
{
    QStringList address;

    for (int i = 0; i < currentVisualObject->getDiagnosticItemList().size(); ++i)
    {
        if (currentVisualObject->getDiagnosticItemList().at(i)->getSys() == 0 &&
                currentVisualObject->getDiagnosticItemList().at(i)->getSubSys() == 0 &&
                currentVisualObject->getDiagnosticItemList().at(i)->getIp() != "")
            //address.append(itemList.at(i)->getIp());
            address.append(currentVisualObject->getDiagnosticItemList().at(i)->getIp());
    }

    messageBox = new QMessageBox;
    datagramSender(Global::Packet::shutdown, address);
    address.clear();
}

void MainWindow::objectMenuLogOff()
{
    QStringList address;

    for (int i = 0; i < currentVisualObject->getDiagnosticItemList().size(); ++i)
    {
        if (currentVisualObject->getDiagnosticItemList().at(i)->getSys() == 0 &&
                currentVisualObject->getDiagnosticItemList().at(i)->getSubSys() == 0 &&
                currentVisualObject->getDiagnosticItemList().at(i)->getIp() != "")
            //address.append(itemList.at(i)->getIp());
        address.append(currentVisualObject->getDiagnosticItemList().at(i)->getIp());
    }

    messageBox = new QMessageBox;
    datagramSender(Global::Packet::logoff, address);
    address.clear();
}

void MainWindow::objectMenuSetOffCheck()
{
    //qDebug() << currentVisualObject->getName();
    //qDebug() << currentVisualObject->getDiagnosticItemList().size();
    //qDebug() << currentVisualObject->getDiagnosticItemList().at(currentItemIndex)->getName();
    //qDebug() << vo->getName();
    for (int i = 0; i < currentVisualObject->getDiagnosticItemList().size(); i++)
    {

        currentVisualObject->getDiagnosticItemList().at(i)->setState(Global::State::offcheck);
    }
    //currentVisualObject->setState(Global::State::offcheck); //родительский элемент тоже в оффчек
}

void MainWindow::objectMenuSetOnCheck()
{
    for (int i = 0; i < currentVisualObject->getDiagnosticItemList().size(); i++)
    {
        currentVisualObject->getDiagnosticItemList().at(i)->setState(Global::State::na);
    }
    //currentVisualObject->setState(Global::State::na);
    //qDebug() << currentVisualObject->getDiagnosticItemList().at(currentItemIndex)->getName();
}

void MainWindow::objectMenuForceCheck()
{
}

void MainWindow::itemMenuSetOffCheck()
{
    //qDebug() << currentVisualObject->getName();
    //qDebug() << currentVisualObject->getDiagnosticItemList().size();
    //qDebug() << currentVisualObject->getDiagnosticItemList().at(currentItemIndex)->getName();
    currentVisualObject->getDiagnosticItemList().at(currentItemIndex)->setState(Global::State::offcheck);
}

void MainWindow::itemMenuSetOnCheck()
{
    //    qDebug() << currentVisualObject->getName();
    //    qDebug() << currentVisualObject->getDiagnosticItemList().size();
    //    qDebug() << currentVisualObject->getDiagnosticItemList().at(currentItemIndex)->getName();
    currentVisualObject->getDiagnosticItemList().at(currentItemIndex)->setState(Global::State::na);
}

void MainWindow::itemMenuForceCheck()
{
}

void MainWindow::itemMenuServiceOff()
{
    //запилить пакет на выключение программы
}

void MainWindow::itemMenuServiceOn()
{
    //запилить пакет на включение программы
}

void MainWindow::button_reciever()
{
    clickedItemId = sender()->property("id").toInt();	//быстрее получать так, задав пропертю ид, чем перебором, как строчкой выше
    currentVisualObject = (VisualObject *) (objectsList->getItemById(clickedItemId));

    populateInfoTable();
}

void MainWindow::on_diagnosticItemUpdated()
{
    populateInfoTable();
}

void MainWindow::journalTableWrite(QStringList message)
{
    QStandardItem *parent;
    QStandardItem *source;
    QStandardItem *number;
    QStandardItem *date;
    QStandardItem *log;

    QString logStr;
    QString sourceStr;
    QString parentStr;
    QString dateStr;
    QString writeToLogStr;
    QString numberString;

    logStr = message.at(0);
    sourceStr = message.at(1);
    if (message.size() == 2)
        parentStr = "Ядро программы";
    else
        parentStr = objectsList->getItemById(QString(message.at(2)).toInt())->getName();

    dateStr = QDateTime::currentDateTime().toString("dd.MM.yy hh:mm:ss");

    writeToLogStr = QString::number(journalIndex) + ";" + dateStr + ";" + sourceStr + ";" + parentStr + ";" + logStr + "\n";

    number = new QStandardItem;
    number->setData(journalIndex, Qt::DisplayRole); //так работает сортировка по числам, а не строкам
    log = new QStandardItem(QString(logStr));
    source = new QStandardItem(QString(sourceStr));
    parent = new QStandardItem(QString(parentStr));
    date = new QStandardItem(QString(dateStr));

    QList<QStandardItem *> journalItemList;
    journalItemList.append(number);
    journalItemList.append(date);
    journalItemList.append(source);
    journalItemList.append(parent);
    journalItemList.append(log);

    journalModel->appendRow(journalItemList);

    if (ui->scrollJournalButton->isChecked() == true)
    {
        ui->journalTable->scrollToBottom();
    }

    if (journalAutoSave)
    {
        QTextStream outLog(logFile);
        outLog << writeToLogStr;
    }

    journalIndex++;
}

void MainWindow::on_actionSelectItems_triggered(bool checked)
{
    if (checked)
        for (int i = 0; i < objectsList->getObjectList().size(); i++)
        {
            objectsList->getObjectList().at(i)->getItem()->setCheckable(true);
        }
    else
        for (int i = 0; i < objectsList->getObjectList().size(); i++)
        {
            objectsList->getObjectList().at(i)->getItem()->setChecked(false);
            objectsList->getObjectList().at(i)->getItem()->setCheckable(false);
        }
}

void MainWindow::on_actionReboot_triggered()
{
    bool show = false;
    QString address;
    messageBox = new QMessageBox(QMessageBox::Information, "Удаленная перезагрузка",
                                 "Выдача в сеть комманды на удаленную перезагрузку вычислителей.");
    messageBox->setWindowIcon(QIcon(":/icons/a108.png"));

    if (ui->actionSelectItems->isChecked())
    {
        for (int i = 0; i < itemList.size(); ++i)
        {
            if (itemList.at(i)->getSys() == 0 && itemList.at(i)->getSubSys() == 0 && itemList.at(i)->getIp() != "")
                if (objectsList->getItemById(itemList.at(i)->getParentId())->getItem()->isChecked())
                {
                    address.append(itemList.at(i)->getIp());
                    address.append("\n");

                    if (!show)
                    {
                        messageBox->show();
                        show = true;
                    }
                }
        }
        QStringList addressList = address.split("\n");
        addressList.removeLast();

        datagramSender(Global::Packet::reboot, addressList);
    }
    else
    {
        for (int i = 0; i < itemList.size(); ++i)
        {
            if (itemList.at(i)->getSys() == 0 && itemList.at(i)->getSubSys() == 0 && itemList.at(i)->getIp() != "")
            {
                address.append(itemList.at(i)->getIp());
                address.append("\n");

                if (!show)
                {
                    messageBox->show();
                    show = true;
                }
            }
        }
        QStringList addressList = address.split("\n");
        addressList.removeLast();

        datagramSender(Global::Packet::reboot, addressList);
    }
}

void MainWindow::on_actionShutdown_triggered()
{
    bool show = false;
    QString address;
    messageBox = new QMessageBox(QMessageBox::Information, "Удаленное завершение работы",
                                 "Выдача в сеть комманды на удаленное завершение работы вычислителей.");
    messageBox->setWindowIcon(QIcon(":/icons/a108.png"));

    if (ui->actionSelectItems->isChecked())
    {
        for (int i = 0; i < itemList.size(); ++i)
        {
            if (itemList.at(i)->getSys() == 0 && itemList.at(i)->getSubSys() == 0 && itemList.at(i)->getIp() != "")
                if (objectsList->getItemById(itemList.at(i)->getParentId())->getItem()->isChecked())
                {
                    address.append(itemList.at(i)->getIp());
                    address.append("\n");

                    if (!show)
                    {
                        messageBox->show();
                        show = true;
                    }
                }
        }
        QStringList addressList = address.split("\n");
        addressList.removeLast();

        datagramSender(Global::Packet::shutdown, addressList);
    }
    else
    {
        for (int i = 0; i < itemList.size(); ++i)
        {
            if (itemList.at(i)->getSys() == 0 && itemList.at(i)->getSubSys() == 0 && itemList.at(i)->getIp() != "")
            {
                address.append(itemList.at(i)->getIp());
                address.append("\n");

                if (!show)
                {
                    messageBox->show();
                    show = true;
                }
            }
        }
        QStringList addressList = address.split("\n");
        addressList.removeLast();

        datagramSender(Global::Packet::shutdown, addressList);
    }
}

void MainWindow::on_clearJournalButton_clicked()
{
    initJournalTable();
}

void MainWindow::on_actionQuit_triggered()
{
    trayIcon->hide();
    qApp->quit();
}

void MainWindow::on_scrollJournalButton_clicked(bool checked)
{
    qDebug() << "checked";

    QSettings settings("settings.ini", QSettings::IniFormat);

    if (checked)
    {
        settings.setValue("autoscroll", true);
        journalAutoScroll = true;
        ui->journalTable->scrollToBottom();
    }
    else
    {
        settings.setValue("autoscroll", false);
        journalAutoScroll = false;
    }
}

void MainWindow::on_actionPreferences_triggered()
{
    QSettings settings("settings.ini", QSettings::IniFormat);

    settingsWindow->setJournalDir(settings.value("journal_dir").toString());
    settingsWindow->setConfigFileName(settings.value("config_file").toString());

    if (settings.value("object_height").toInt() != 0)
        settingsWindow->setObjectHeight(settings.value("object_height").toInt());
    else if (!objectsList->getObjectList().isEmpty())
    {
        settings.setValue("object_height", objectsList->getObjectList().at(0)->getItem()->height());
        settingsWindow->setObjectHeight(objectsList->getObjectList().at(0)->getItem()->height());
    }

    if (settings.value("autosave").toBool())
        settingsWindow->setJournalWrite(true);
    else
        settingsWindow->setJournalWrite(false);

    if (settings.value("font_bold").toBool())
        settingsWindow->setBoldFont(true);
    else
        settingsWindow->setBoldFont(false);

    settingsWindow->netIfListUpdate();
    settingsWindow->show();
}

void MainWindow::on_actionAbout_triggered()
{
    QString about =
            "ФГУП ГосНИИАС \nподр. 1900 лаб. 1920\n\n ФПО ЮСИЯ.01130-01 \"АРМ Пульт\" Версия 1.01";
    //about.append(
    //            "TODO:\n1. В настройках файл начальной конфигурации. (+)\n2. Реализовать сценарии.\n3. В настройках путь сохранения журнала. (+)\n\n");
    //about.append(
    //            "Не работает:\n1. В настройках выбор сетевой ни к чему не привязан.\n2. Настройки - Выключение поста.\n3. Настройки - Отработка постом после комплекса.");
    about.append(" \n");
    QMessageBox::about(this, "О программе", about);
}

void MainWindow::on_actionBuild_triggered()
{
    QMessageBox::aboutQt(this, "Сборка в среде");
}

void MainWindow::on_actionLogout_triggered()
{
    bool show = false;
    QString address;
    messageBox = new QMessageBox(QMessageBox::Information, "Завершение сеанса",
                                 "Выдача в сеть комманды на удаленное завершение сеанса операторов.");
    messageBox->setWindowIcon(QIcon(":/icons/a108.png"));

    if (ui->actionSelectItems->isChecked())
    {
        for (int i = 0; i < itemList.size(); ++i)
        {
            if (itemList.at(i)->getSys() == 0 && itemList.at(i)->getSubSys() == 0 && itemList.at(i)->getIp() != "")
                if (objectsList->getItemById(itemList.at(i)->getParentId())->getItem()->isChecked())
                {
                    address.append(itemList.at(i)->getIp());
                    address.append("\n");

                    if (!show)
                    {
                        messageBox->show();
                        show = true;
                    }
                }
        }
        QStringList addressList = address.split("\n");
        addressList.removeLast();

        datagramSender(Global::Packet::logoff, addressList);
    }
    else
    {
        for (int i = 0; i < itemList.size(); ++i)
        {
            if (itemList.at(i)->getSys() == 0 && itemList.at(i)->getSubSys() == 0 && itemList.at(i)->getIp() != "")
            {
                address.append(itemList.at(i)->getIp());
                address.append("\n");

                if (!show)
                {
                    messageBox->show();
                    show = true;
                }
            }
        }
        QStringList addressList = address.split("\n");
        addressList.removeLast();

        datagramSender(Global::Packet::logoff, addressList);
    }
}

void MainWindow::on_actionSelfShutdown_triggered()
{
    qDebug() << "method_stub: shutdown this workstation";
}

void MainWindow::on_actionConfigFileChanged()
{
    disconnect(settingsWindow, &SettingsWindow::configFileChanged, this, &MainWindow::on_actionConfigFileChanged);

    clearAll();

    QSettings settings("settings.ini", QSettings::IniFormat);

    QString configName;

    if (settings.value("config_file").isValid())
    {
        configName = settings.value("config_file").toString();
    }
    else
    {
        configName = "config.xml";
        settings.setValue("config_file", "config.xml");
    }

    loadMain(configName);

    updateSettings();
}

void MainWindow::loadMain(QString name)
{
    initDiagnostic(name);
    ui->rootVerticalLayout->insertLayout(0, rootGrid->getLayout());

    ui->infoTable->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->infoTable, &QTableView::customContextMenuRequested, this, &MainWindow::infoMenu);

    initInfoTable();
    initJournalTable();

    currentVisualObject = Q_NULLPTR;
    clickedItemId = -1;

    journalAutoSave = false;
    journalAutoScroll = false;

    QSettings settings("settings.ini", QSettings::IniFormat);

   // qDebug() << settings.value("diagnostic_timeout").toInt();

    //    if (settings.value("diagnostic_timeout").toInt() == 0)
//        settings.setValue("diagnostic_timeout", 1500);

    if (settings.value("autosave").toBool())
        journalWriteChange(true);

    if (settings.value("autoscroll").toBool())
        ui->scrollJournalButton->toggle();

    if (settings.value("font_bold").toBool())
        fontBoldChange(true);

    if (settings.value("network").toString() != "")
    {
        //qDebug() << "network is present";
    }

    settingsWindow = new SettingsWindow(this);
    settingsWindow->setWindowTitle("Настройки");
    settingsWindow->setModal(true);

    connect(settingsWindow, &SettingsWindow::objectHeightChanged, this, &MainWindow::objectHeightChange);
    connect(settingsWindow, &SettingsWindow::fontBoldChanged, this, &MainWindow::fontBoldChange);
    connect(settingsWindow, &SettingsWindow::journalWriteChanged, this, &MainWindow::journalWriteChange);
    connect(settingsWindow, &SettingsWindow::diagnosticTimeoutChanged, this, &MainWindow::diagnosticTimeoutChange);
    connect(settingsWindow, &SettingsWindow::configFileChanged, this, &MainWindow::on_actionConfigFileChanged);

    //setAnimated(true);

    QStringList msg;
    msg << "Старт оболочки" << "Инициализация";
    journalTableWrite(msg);

    //заполняем мап для поиска объекта по индексу
    for (int i = 0; i < itemList.size(); ++i)
    {
        QString key1 = QString::number(itemList.at(i)->getHsys());
        QString key2 = QString::number(itemList.at(i)->getHsubSys());
        QString key3 = QString::number(itemList.at(i)->getSys());
        QString key4 = QString::number(itemList.at(i)->getSubSys());
        QString key5 = itemList.at(i)->getIp();
        QString key = key1 + "|" + key2 + "|" + key3 + "|" + key4 + "|" + key5;

        //qDebug() << key << " в Мапу";

        itemMap.insert(key, itemList.at(i));
    }

    //ui->statusBar->showMessage("Готов");

    diagnosticTimeoutChange();

    udpListener = new UdpSocketListener(this, Global::Packet::multicast, Global::DiagnosticAddress, Global::SysDiagnosticPortListen);
    packetParser = new LmkPacketParser;
    diagnosticParser = new LmkDiagnosticParser;

    diagnosticParser->setItemMap(itemMap);

    connect(udpListener, &UdpSocketListener::onDatagram, packetParser, &LmkPacketParser::datagram);
    connect(packetParser, &LmkPacketParser::onDiagnostic, diagnosticParser, &LmkDiagnosticParser::diagnosticPacket);

    for (int i = 0; i < objectsList->getObjectList().size(); ++i)
    {
        objectsList->getObjectList().at(i)->changeState();
    }
}

void MainWindow::on_actionLoadConfig_triggered()
{
    QFileDialog *dialog = new QFileDialog(0);
    dialog->setFileMode(QFileDialog::ExistingFile);
    dialog->setAcceptMode(QFileDialog::AcceptOpen);
    dialog->setNameFilter(tr("Файлы конфигурации и состояния (*.xml)"));
    dialog->show();

    QStringList fileNames;
    if (dialog->exec())
    {
        fileNames = dialog->selectedFiles();
        QString loadFileName = fileNames.at(0);

        clearAll();

        loadMain(loadFileName);

        updateSettings();
    }

    dialog->close();
}

void MainWindow::on_actionSaveConfig_triggered()
{
    QFileDialog *dialog = new QFileDialog(0);
    dialog->setFileMode(QFileDialog::AnyFile);
    dialog->setAcceptMode(QFileDialog::AcceptSave);
    dialog->setNameFilter("Файлы конфигурации и состояния (*.xml)");
    dialog->show();

    QStringList fileName;
    if (dialog->exec())
    {
        fileName = dialog->selectedFiles();
        QString saveFileName;

        if (!QString(fileName.at(0)).contains(".xml"))
            saveFileName = QString(fileName.at(0)).append(".xml");
        else
            saveFileName = QString(fileName.at(0));

        xmlConfig = new XmlConfig(saveFileName);
        xmlConfig->setObjectsList(objectsList);
        xmlConfig->setItemList(itemList);
        xmlConfig->writeConfig();
    }
    dialog->close();
}


void MainWindow::on_actionSelfReboot_triggered()
{

}
