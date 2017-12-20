#include "settingswindow.h"
#include "ui_settingswindow.h"

#include <QSettings>
#include <QStringList>
#include <QFileDialog>

SettingsWindow::SettingsWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);

    IPSelection = QStringList() << "MANUAL" << "AUTO";

    //setSizeGripEnabled(false); //запретить растягивать это окно
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint); //Фыксед сайз :)

    QSettings settings("settings.ini", QSettings::IniFormat);
    diagnosticTimeout = settings.value("diagnostic_timeout").toDouble()/1000;
    ui->diagnosticTimeout->setValue(settings.value("diagnostic_timeout").toDouble()/1000);


    diagEmitTimeout = settings.value("diagnostic_emit_timeout").toDouble()/1000;
    ui->packetOutTimeout->setValue(settings.value("diagnostic_emit_timeout").toDouble()/1000);

    hSys = settings.value("hsys").toInt();
    ui->hSysLineEdit->setText(QString::number(hSys));

    hSubsys = settings.value("hsubsys").toInt();
    ui->hSubsysLineEdit->setText(QString::number(hSubsys));

    sys = settings.value("sys").toInt();
    ui->sysLineEdit->setText(QString::number(sys));

    subsys = settings.value("subsys").toInt();
    ui->subsysLineEdit->setText(QString::number(subsys));

    if (settings.value("getipmode").toString() == "AUTO")
    {
        ipSelection = "AUTO";
        ui->autoIpAddress->setChecked(true);
    }

    if (settings.value("getipmode").toString() == "MANUAL")
    {
        ipSelection = "MANUAL";
        ui->manualIpAddress->setChecked(true);
    }

    if (settings.value("reinit_socket").toBool())
    {
        reinitSocket = true;
    }
    else
    {
        reinitSocket = false;
    }

    QString iface = settings.value("network").toString();
    //qDebug() << iface;

    netIfListUpdate();
    ui->netIfcomboBox->addItems(netInterfacesStringList);



    for (int i = 0; i < netInterfaces.size(); i++)
    {
        if (netInterfaces.at(i).name() == iface)
        {
            //qDebug() << netInterfaces.at(i).name();
            index = i;
        }
    }

    ui->netIfcomboBox->setCurrentIndex(index);

    //qDebug() << netInterfaces;

    if (ipSelection == "AUTO")
    {
        int i = ui->netIfcomboBox->currentIndex();
        ip =  netInterfaces.at(i).addressEntries().at(0).ip().toString();
        ui->ipAddressLineEdit->setText(ip);
    }

    if (ipSelection == "MANUAL")
    {
        ip =  settings.value("ip").toString();
    }

    //запилить фильтры на сис сабсис ип и тому подобное, чтобы только цифры и тд
    ui->hSysLineEdit->setValidator(new QIntValidator(0, 255, this));
    ui->hSubsysLineEdit->setValidator(new QIntValidator(0, 255, this));
    ui->sysLineEdit->setValidator(new QIntValidator(0, 255, this));
    ui->subsysLineEdit->setValidator(new QIntValidator(0, 255, this));

    QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    QRegExp ipRegex ("^" + ipRange
                     + "\\." + ipRange
                     + "\\." + ipRange
                     + "\\." + ipRange + "$");
    QRegExpValidator *ipValidator = new QRegExpValidator(ipRegex, this);
    ui->ipAddressLineEdit->setValidator(ipValidator);

    if (ui->autoIpAddress->isChecked())
    {
        ui->ipAddressLineEdit->setDisabled(true);
        ui->netIfcomboBox->setDisabled(false);
    }

    if (ui->manualIpAddress->isChecked())
    {
        ui->ipAddressLineEdit->setDisabled(false);
        ui->netIfcomboBox->setDisabled(true);
    }

}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}

void SettingsWindow::setObjectHeight(quint16 height)
{
    objectHeight = height;
    ui->objectHeight->setValue(height);
}

void SettingsWindow::on_objectHeight_valueChanged(int height)
{
    emit objectHeightChanged(height);
}

void SettingsWindow::on_boldFont_toggled(bool checked)
{
    if (checked)
    {
        emit fontBoldChanged(true);
    }
    else
    {
        emit fontBoldChanged(false);
    }
}

void SettingsWindow::on_writeLog_toggled(bool checked)
{
    if (checked)
    {
        //qDebug() << "signal checked";
        emit journalWriteChanged(true);
    }
    else
    {
        //qDebug() << "signal unchecked";
        emit journalWriteChanged(false);
    }
}

void SettingsWindow::on_okButton_clicked()
{
    QSettings settings("settings.ini", QSettings::IniFormat);

    settings.setValue("diagnostic_timeout", ui->diagnosticTimeout->value() * 1000);
    diagnosticTimeout = diagnosticTimeout = settings.value("diagnostic_timeout").toDouble()/1000;
    emit diagnosticTimeoutChanged();
    this->close();

    if (ui->hSysLineEdit->text() == "")
    {
        ui->hSysLineEdit->setText(QString::number(hSys));
        settings.setValue("hsys", hSys);
    }

    if (ui->hSubsysLineEdit->text() == "")
    {
        ui->hSubsysLineEdit->setText(QString::number(hSubsys));
        settings.setValue("hsubsys", hSubsys);
    }

    if (ui->sysLineEdit->text() == "")
    {
        ui->sysLineEdit->setText(QString::number(sys));
        settings.setValue("sys", sys);
    }

    if (ui->subsysLineEdit->text() == "")
    {
        ui->subsysLineEdit->setText(QString::number(subsys));
        settings.setValue("subsys", subsys);
    }

    hSys = settings.value("hsys").toInt();
    hSubsys = settings.value("hsubsys").toInt();
    sys = settings.value("sys").toInt();
    subsys = settings.value("subsys").toInt();

    if (settings.value("getipmode").toString() == "AUTO")
    {
        ipSelection = "AUTO";

    }

    if (settings.value("getipmode").toString() == "MANUAL")
    {
        ipSelection = "MANUAL";

    }

    if (settings.value("reinit_socket").toBool())
    {
        reinitSocket = true;
    }
    else
    {
        reinitSocket = false;
    }

    settings.setValue("ip", ui->ipAddressLineEdit->text());
    ip = ui->ipAddressLineEdit->text();

    index = ui->netIfcomboBox->currentIndex();
}

void SettingsWindow::on_cancelButton_clicked()
{
    close();

    QSettings settings("settings.ini", QSettings::IniFormat);

    //objectHeight section
    emit objectHeightChanged(objectHeight);
    ui->objectHeight->setValue(objectHeight);

    //journalWrite section
    emit journalWriteChanged(journalWrite);
    ui->writeLog->setChecked(journalWrite);

    //boldFont section
    emit fontBoldChanged(boldFont);
    ui->boldFont->setChecked(boldFont);

    //timeout section
    ui->diagnosticTimeout->setValue(diagnosticTimeout);
    settings.setValue("diagnostic_timeout", diagnosticTimeout * 1000);


    //emit_timeout section
    settings.setValue("diagnostic_emit_timeout", diagEmitTimeout * 1000);

    //diag packet_set
    ui->hSysLineEdit->setText(QString::number(hSys));
    settings.setValue("hsys", hSys);
    ui->hSubsysLineEdit->setText(QString::number(hSubsys));
    settings.setValue("hsubsys", hSubsys);
    ui->sysLineEdit->setText(QString::number(sys));
    settings.setValue("sys", sys);
    ui->subsysLineEdit->setText(QString::number(subsys));
    settings.setValue("subsys", subsys);

    //net settings
    if (ipSelection == "AUTO")
    {
        ui->autoIpAddress->setChecked(true);
    }

    if (ipSelection == "MANUAL")
    {
        ui->manualIpAddress->setChecked(true);
    }

    if (reinitSocket)
    {
        ui->reinitCheckBox->setChecked(true);
    }
    else
    {
        ui->reinitCheckBox->setChecked(false);
    }

    ui->netIfcomboBox->setCurrentIndex(index);
}

void SettingsWindow::setBoldFont(bool value)
{
    ui->boldFont->setChecked(value);
    boldFont = value;
}


double SettingsWindow::getDiagnosticTimeout() const
{
    return diagnosticTimeout;
}

void SettingsWindow::setDiagnosticTimeout(double value)
{
    diagnosticTimeout = value;
}

bool SettingsWindow::getBoldFont() const
{
    return boldFont;
}

bool SettingsWindow::getJournalWrite() const
{
    return journalWrite;
}

void SettingsWindow::setJournalWrite(bool value)
{
    journalWrite = value;
    ui->writeLog->setChecked(value);
}

void SettingsWindow::netIfListUpdate()
{
    //	qDebug() << "oldskool";
    QStringList activeNetInterfaces;
    foreach (const QNetworkInterface& interface, QNetworkInterface::allInterfaces())
    {
        if (!interface.flags().testFlag(QNetworkInterface::IsLoopBack) && interface.flags().testFlag(QNetworkInterface::IsUp))
        {
            foreach (QNetworkAddressEntry entry, interface.addressEntries())
            {
                if (interface.hardwareAddress() != "00:00:00:00:00:00" && entry.ip().toString().contains("."))
                {
                    //qDebug() << ("Сеть: " + interface.humanReadableName());
                    //qDebug() << interface.hardwareAddress();
                    //qDebug() << ("Адрес адаптера: " + entry.ip().toString());

                    //activeNetInterfaces.append(interface.name());

                    activeNetInterfaces.append(interface.humanReadableName());
                }
            }
        }
    }

    netInterfaces.clear();
    netInterfacesStringList.clear();

    //qDebug() << "\nnuwave";
    for (int i = 0; i < QNetworkInterface::allInterfaces().size(); i++)
    {
        if (QNetworkInterface::allInterfaces().at(i).isValid())
        {
            netInterfaces.append(QNetworkInterface::allInterfaces().at(i));
            netInterfacesStringList.append(QNetworkInterface::allInterfaces().at(i).humanReadableName());
            //			qDebug() << "-----------";
            //			qDebug() << QNetworkInterface::allInterfaces().at(i).name();
            //			qDebug() << QNetworkInterface::allInterfaces().at(i).humanReadableName();
            //			qDebug() << QNetworkInterface::allInterfaces().at(i).hardwareAddress();
            //			qDebug() << QNetworkInterface::allInterfaces().at(i).index();
            //			qDebug() << QNetworkInterface::allInterfaces().at(i).flags().testFlag(QNetworkInterface::CanMulticast);
            if ((!QNetworkInterface::allInterfaces().at(i).flags().testFlag(QNetworkInterface::CanMulticast) &&
                 !QNetworkInterface::allInterfaces().at(i).flags().testFlag(QNetworkInterface::IsUp)) ||
                    QNetworkInterface::allInterfaces().at(i).flags().testFlag(QNetworkInterface::IsLoopBack))
            {
                netInterfaces.removeLast();
                netInterfacesStringList.removeLast();
            }
        }
    }

    for (int i = 0; i < netInterfacesStringList.size(); i++)
    {
        if (activeNetInterfaces.contains(netInterfacesStringList.at(i)))
        {
            QString active = netInterfacesStringList.at(i);
            active.append(" (акт.)");
            netInterfacesStringList.removeAt(i);
            netInterfacesStringList.insert(i, active);
        }
        else
        {
            QString nactive = netInterfacesStringList.at(i);
            nactive.append(" (неакт.)");
            netInterfacesStringList.removeAt(i);
            netInterfacesStringList.insert(i, nactive);
        }
    }
    //qDebug() << netIfacesStringList;
}
void SettingsWindow::on_netIfcomboBox_currentIndexChanged(int index)
{
    QSettings settings("settings.ini", QSettings::IniFormat);
    settings.setValue("network", netInterfaces.at(index).name());
    ui->ipAddressLineEdit->setText(netInterfaces.at(index).addressEntries().at(0).ip().toString());
}

quint16 SettingsWindow::getObjectHeight() const
{
    return objectHeight;
}

void SettingsWindow::on_journalDirChangeButton_clicked()
{
    QFileDialog *dialog = new QFileDialog(this);
    dialog->setModal(true);
    dialog->setFileMode(QFileDialog::DirectoryOnly);
    dialog->show();

    QStringList dirName;
    if (dialog->exec())
    {
        dirName = dialog->selectedFiles();
        QString saveDirName = QString(dirName.at(0));

        ui->journalDirLineEdit->setText(saveDirName);

        QSettings settings("settings.ini", QSettings::IniFormat);
        settings.setValue("journal_dir", saveDirName);
        journalDir = saveDirName;
    }
    dialog->close();
}

void SettingsWindow::on_configFileChangeButton_clicked()
{
    QFileDialog *dialog = new QFileDialog(this);
    dialog->setModal(true);
    dialog->setFileMode(QFileDialog::AnyFile);
    dialog->setNameFilter("Файлы конфигурации (*.xml)");
    dialog->show();

    QStringList confName;
    if (dialog->exec())
    {
        confName = dialog->selectedFiles();
        QString saveConfName = QString(confName.at(0));

        QSettings settings("settings.ini", QSettings::IniFormat);

        ui->configFileLineEdit->setText(saveConfName);
        settings.setValue("config_file", saveConfName);
        configFileName = saveConfName;
    }
    dialog->close();

    this->close();

    emit configFileChanged();
}

QString SettingsWindow::getConfigFileName() const
{
    return configFileName;
}

void SettingsWindow::setConfigFileName(const QString &value)
{
    configFileName = value;
    ui->configFileLineEdit->setText(value);
}

QString SettingsWindow::getJournalDir() const
{
    return journalDir;
}

void SettingsWindow::setJournalDir(const QString &value)
{
    journalDir = value;
    ui->journalDirLineEdit->setText(value);
}

void SettingsWindow::on_reinitCheckBox_toggled(bool checked)
{
    //qDebug() << "reinit socket toggled " << checked;
    QSettings settings("settings.ini", QSettings::IniFormat);
    settings.setValue("reinit_socket", checked);
}

void SettingsWindow::on_packetOutTimeout_valueChanged(double arg1)
{
    QSettings settings("settings.ini", QSettings::IniFormat);
    settings.setValue("diagnostic_emit_timeout", arg1 * 1000);
}

void SettingsWindow::on_hSysLineEdit_textChanged(const QString &arg1)
{
    QSettings settings("settings.ini", QSettings::IniFormat);
    settings.setValue("hsys", arg1);
}

void SettingsWindow::on_hSubsysLineEdit_textChanged(const QString &arg1)
{
    QSettings settings("settings.ini", QSettings::IniFormat);
    settings.setValue("hsubsys", arg1);
}

void SettingsWindow::on_sysLineEdit_textChanged(const QString &arg1)
{
    QSettings settings("settings.ini", QSettings::IniFormat);
    settings.setValue("sys", arg1);
}

void SettingsWindow::on_subsysLineEdit_textChanged(const QString &arg1)
{
    QSettings settings("settings.ini", QSettings::IniFormat);
    settings.setValue("subsys", arg1);
}

void SettingsWindow::on_manualIpAddress_toggled(bool checked)
{
    if (checked)
    {
        ui->ipAddressLineEdit->setDisabled(false);
        ui->netIfcomboBox->setDisabled(true);
        QSettings settings("settings.ini", QSettings::IniFormat);
        settings.setValue("getipmode", "MANUAL");
    }
}

void SettingsWindow::on_autoIpAddress_toggled(bool checked)
{
    if (checked)
    {
        ui->ipAddressLineEdit->setDisabled(true);
        ui->netIfcomboBox->setDisabled(false);
        QSettings settings("settings.ini", QSettings::IniFormat);
        settings.setValue("getipmode", "AUTO");
    }
}

void SettingsWindow::on_ipAddressLineEdit_editingFinished()
{
    QSettings settings("settings.ini", QSettings::IniFormat);
    settings.setValue("ip", ui->ipAddressLineEdit->text());
}
