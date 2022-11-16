#ifndef TFTP_CLIENT_MAINWINDOW_H
#define TFTP_CLIENT_MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QMutex>
#include <QTime>

#include "tftp.h"
#include "ui_mainwindow.h"


QT_BEGIN_NAMESPACE
namespace Ui { class mainwindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

private:
    Ui::MainWindow *ui;
    QString ip;
    QString port;
    QString filename;
    QString savepath;
    TFTP::TFTP_MODE mode;
    int blksize;
    int timeout;
    int retries;
    TFTP::TFTP_LOG_LEVEL logLevel = TFTP::TFTP_LOG_INFO;

private slots:

    bool getIP();

    bool getInfo();

    void log(const QString &msg) {
        static QMutex mutex;
        mutex.lock();
        // 获取时间
        QTime time = QTime::currentTime();
        QString str = time.toString("[hh:mm:ss.zzz] ");
        ui->logBrowser->appendPlainText(str + msg);
        mutex.unlock();
    }
};

#endif //TFTP_CLIENT_MAINWINDOW_H
