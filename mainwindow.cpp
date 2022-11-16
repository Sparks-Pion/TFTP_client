// You may need to build the project (run Qt uic code generator) to get "ui_mainwindow.h" resolved
//#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QMetaType>
#include <QtConcurrent>
#include <QRegularExpression>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::MainWindow) {

    ui->setupUi(this);

    // init UI
    resize(600, 800);
    setWindowTitle("TFTP Client");
    ui->logBrowser->setReadOnly(true);
    ui->IPeditor->setPlaceholderText(TFTP::Default_IP);
    ui->portEditor->setPlaceholderText(QString::number(TFTP::TFTP_DEFAULT_PORT));
    ui->sizeEditor->setPlaceholderText(QString::number(TFTP::TFTP_DEFAULT_BLOCK_SIZE));
    ui->timeoutEditor->setPlaceholderText(QString::number(TFTP::TFTP_DEFAULT_TIMEOUT));
    ui->retryEditor->setPlaceholderText(QString::number(TFTP::TFTP_DEFAULT_RETRIES));
    ui->logModeBox->setCurrentIndex(1);

    // press enter to check IP and port
    connect(ui->IPeditor, &QLineEdit::returnPressed, this, &MainWindow::getIP);
    connect(ui->portEditor, &QLineEdit::returnPressed, this, &MainWindow::getIP);

    // get file path interface
    connect(ui->fileBtn, &QPushButton::clicked, this, [=]() {
        QString path = QFileDialog::getOpenFileName(this, "Select a file", QDir::currentPath());
        if (path.isEmpty()) return;
        ui->savepathEditor->setText(path);
    });

    // select log level
    connect(ui->logModeBox, &QComboBox::currentTextChanged, this, [=](const QString &text) {
        if (text == "DEBUG") {
            logLevel = TFTP::TFTP_LOG_DEBUG;
        } else if (text == "INFO") {
            logLevel = TFTP::TFTP_LOG_INFO;
        } else if (text == "WARNING") {
            logLevel = TFTP::TFTP_LOG_WARNING;
        } else if (text == "ERROR") {
            logLevel = TFTP::TFTP_LOG_ERROR;
        }
    });

    // output log
    connect(ui->outputBtn,&QPushButton::clicked,this,[=](){
        QString path = QFileDialog::getSaveFileName(this, "Select a file", QDir::currentPath());
        if (path.isEmpty()) return;
        QFile file(path);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::warning(this, "Warning", "Failed to open file");
            return;
        }
        QTextStream out(&file);
        out << ui->logBrowser->toPlainText();
        file.close();
    });

    // test connection interface
    connect(ui->testBtn, &QPushButton::clicked, this, [=]() {
        log("********** Testing connection **********");
        if (!getIP()) return;
        QtConcurrent::run([&]() {
            TFTP::TFTPClient client(logLevel);
            connect(&client, &TFTP::TFTPClient::log, this, &MainWindow::log);
            if(!client.setServer_ipv4(ip, port.toInt())) return;
            if(!client.test_ipv4()) return;
        });
    });

    // download interface
    connect(ui->downloadBtn, &QPushButton::clicked, this, [=]() {
        log("********** Downloading **********");
        if (!getIP()) return;
        if (!getInfo()) return;
        if (QFile::exists(savepath)) {
            QMessageBox::StandardButton btn = QMessageBox::question(this, "File exists", "File exists, overwrite?");
            if (btn == QMessageBox::No) return;
            log("[WARNING] file exists, overwrite (" + savepath + ")");
        }
        QtConcurrent::run([&]() {
            TFTP::TFTPClient client(logLevel);
            connect(&client, &TFTP::TFTPClient::log, this, &MainWindow::log);
            if(!client.setServer_ipv4(ip, port.toInt())) return;
            if(!client.download_ipv4(filename, savepath, mode, blksize, timeout, retries)) return;
        });
    });

    // upload interface
    connect(ui->uploadBtn, &QPushButton::clicked, this, [=]() {
        log("********** Uploading **********");
        if (!getIP()) return;
        if (!getInfo()) return;
        if (!QFile::exists(savepath)) {
            log("[ERROR] file not exists (" + savepath + ")");
            return;
        }
        QtConcurrent::run([&]() {
            TFTP::TFTPClient client(logLevel);
            connect(&client, &TFTP::TFTPClient::log, this, &MainWindow::log);
            if(!client.setServer_ipv4(ip, port.toInt())) return;
            if(!client.upload_ipv4(filename, savepath, mode, blksize, timeout, retries)) return;
        });
    });

}

MainWindow::~MainWindow() {
    delete ui;
}

auto ipv4 = QRegularExpression(
        R"(\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\b)");

//auto ipv6 = QRegularExpression(
//        R"(\b(?:(?:[0-9a-fA-F]{1,4}:){7}[0-9a-fA-F]{1,4}|(?=(?:[0-9a-fA-F]{0,4}:){0,7}[0-9a-fA-F]{0,4}(?![:.\w]))(([0-9a-fA-F]{1,4}:){1,7}|:)((:[0-9a-fA-F]{1,4}){1,7}|:))\b)");

bool MainWindow::getIP() {
    ip = ui->IPeditor->text();
    if (ip.isEmpty()) {
        ip = TFTP::Default_IP;
    } else {
        QRegularExpressionMatch match;
        match = ipv4.match(ip);
        if (match.hasMatch()) {
            ip = match.captured(0);
//        } else {
//            match = ipv6.match(ip);
//            if (match.hasMatch()) {
//                ip = match.captured(0);
//            }
        } else {
            log("[ERROR] Invalid IP address!");
            return false;
        }
    }

    port = ui->portEditor->text();
    if (port.isEmpty()) {
        port = TFTP::Default_Port;
    } else {
        bool ok;
        int portNum = port.toInt(&ok);
        if (!ok || portNum < 0 || portNum > 65535) {
            log("[ERROR] Invalid port number!");
            return false;
        }
    }
//    log("[INFO] Server IP: " + ip + ":" + port);
    return true;
}

bool MainWindow::getInfo() {
    filename = ui->filenameEditor->text();
    savepath = ui->savepathEditor->text();
    if (filename.isEmpty() || savepath.isEmpty()) {
        log("[ERROR] filename or savepath is empty");
        return false;
    }
    if (!QDir().mkpath(QFileInfo(savepath).absolutePath())) {
        log("[ERROR] failed to create directory");
        return false;
    }
    if(ui->modeBox->currentIndex() == 0) {
        mode = TFTP::TFTP_MODE_OCTET;
    } else {
        mode = TFTP::TFTP_MODE_NETASCII;
    }
    if(ui->sizeEditor->text().isEmpty()) {
        blksize = TFTP::TFTP_DEFAULT_BLOCK_SIZE;
    } else {
        bool ok;
        blksize = ui->sizeEditor->text().toInt(&ok);
        if (!ok || blksize <= 0 ) {
            log("[ERROR] Invalid block size!");
            return false;
        }
    }
    if(ui->timeoutEditor->text().isEmpty()) {
        timeout = TFTP::TFTP_DEFAULT_TIMEOUT;
    } else {
        bool ok;
        timeout = ui->timeoutEditor->text().toInt(&ok);
        if (!ok || timeout <= 0 ) {
            log("[ERROR] Invalid timeout!");
            return false;
        }
    }
    if(ui->retryEditor->text().isEmpty()) {
        retries = TFTP::TFTP_DEFAULT_RETRIES;
    } else {
        bool ok;
        retries = ui->retryEditor->text().toInt(&ok);
        if (!ok || retries <= 0 ) {
            log("[ERROR] Invalid retries!");
            return false;
        }
    }
//    log("[INFO] get filename: " + filename);
//    log("[INFO] get savepath: " + savepath);
//    log("[INFO] get mode: " + QString(TFTP::TFTP_MODE_STR[mode]));
//    log("[INFO] get blksize: " + QString::number(blksize));
//    log("[INFO] get timeout: " + QString::number(timeout));
//    log("[INFO] get retries: " + QString::number(retries));
    return true;
}
