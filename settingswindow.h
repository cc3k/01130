#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QDialog>
#include <QNetworkInterface>
namespace Ui {
class SettingsWindow;
}

class SettingsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsWindow(QWidget *parent = 0);
    ~SettingsWindow();

    QStringList IPSelection;

    void setObjectHeight(quint16 height);
    quint16 getObjectHeight() const;

    bool getJournalWrite() const;
    void setJournalWrite(bool value);

    bool getBoldFont() const;
    double getDiagnosticTimeout() const;
    void setDiagnosticTimeout(double value);
    void setBoldFont(bool value);

	void netIfListUpdate();

    QString getJournalDir() const;
    void setJournalDir(const QString &value);

    QString getConfigFileName() const;
    void setConfigFileName(const QString &value);

signals:
    void objectHeightChanged(quint16 height);
    void fontBoldChanged(bool bold);
    void journalWriteChanged(bool write);
    void diagnosticTimeoutChanged();
    void configFileChanged();

private slots:
    void on_objectHeight_valueChanged(int arg1);
    void on_boldFont_toggled(bool checked);
    void on_writeLog_toggled(bool checked);
    void on_okButton_clicked();
    void on_cancelButton_clicked();
	void on_netIfcomboBox_currentIndexChanged(int index);

    void on_journalDirChangeButton_clicked();

    void on_configFileChangeButton_clicked();

    void on_reinitCheckBox_toggled(bool checked);

    void on_packetOutTimeout_valueChanged(double arg1);

    void on_hSysLineEdit_textChanged(const QString &arg1);

    void on_hSubsysLineEdit_textChanged(const QString &arg1);

    void on_sysLineEdit_textChanged(const QString &arg1);

    void on_subsysLineEdit_textChanged(const QString &arg1);

    void on_manualIpAddress_toggled(bool checked);

    void on_autoIpAddress_toggled(bool checked);

    void on_ipAddressLineEdit_editingFinished();

private:
    Ui::SettingsWindow *ui;

    double diagnostic_timeout;
    quint16 objectHeight;
    bool journalWrite;
    bool boldFont;
    double diagnosticTimeout;
	QList<QNetworkInterface> netInterfaces;
	QStringList netInterfacesStringList;
    int index;

    QString journalDir;
    QString configFileName;

    QString ip;
    QString ipSelection;
    bool reinitSocket;
    QString ifaceIpAddress;
    quint8 hSys;
    quint8 hSubsys;
    quint8 sys;
    quint8 subsys;
    double diagEmitTimeout;
};

#endif // SETTINGSWINDOW_H
