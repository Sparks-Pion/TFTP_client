/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QWidget *widget_1;
    QHBoxLayout *horizontalLayout;
    QLabel *label_2;
    QLineEdit *IPeditor;
    QLabel *label;
    QLineEdit *portEditor;
    QPushButton *testBtn;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_5;
    QLineEdit *savepathEditor;
    QPushButton *fileBtn;
    QWidget *widget_3;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_6;
    QLineEdit *filenameEditor;
    QWidget *widget_4;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_4;
    QComboBox *modeBox;
    QLabel *label_3;
    QLineEdit *sizeEditor;
    QLabel *label_7;
    QLineEdit *timeoutEditor;
    QLabel *label_8;
    QLineEdit *retryEditor;
    QWidget *widget_5;
    QHBoxLayout *horizontalLayout_5;
    QPushButton *uploadBtn;
    QPushButton *downloadBtn;
    QWidget *widget_6;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_9;
    QComboBox *logModeBox;
    QSpacerItem *horizontalSpacer;
    QPushButton *outputBtn;
    QPlainTextEdit *logBrowser;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(648, 561);
        QFont font;
        font.setFamily(QString::fromUtf8("Fira Code"));
        font.setPointSize(9);
        MainWindow->setFont(font);
        MainWindow->setStyleSheet(QString::fromUtf8("QWidget{font-family: \"Fira Code\";}"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        widget_1 = new QWidget(centralwidget);
        widget_1->setObjectName(QString::fromUtf8("widget_1"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(widget_1->sizePolicy().hasHeightForWidth());
        widget_1->setSizePolicy(sizePolicy);
        widget_1->setMaximumSize(QSize(16777215, 50));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Fira Code"));
        font1.setPointSize(14);
        widget_1->setFont(font1);
        horizontalLayout = new QHBoxLayout(widget_1);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(widget_1);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Fira Code"));
        font2.setPointSize(12);
        label_2->setFont(font2);

        horizontalLayout->addWidget(label_2);

        IPeditor = new QLineEdit(widget_1);
        IPeditor->setObjectName(QString::fromUtf8("IPeditor"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(IPeditor->sizePolicy().hasHeightForWidth());
        IPeditor->setSizePolicy(sizePolicy1);
        IPeditor->setFont(font2);

        horizontalLayout->addWidget(IPeditor);

        label = new QLabel(widget_1);
        label->setObjectName(QString::fromUtf8("label"));
        label->setFont(font2);

        horizontalLayout->addWidget(label);

        portEditor = new QLineEdit(widget_1);
        portEditor->setObjectName(QString::fromUtf8("portEditor"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(portEditor->sizePolicy().hasHeightForWidth());
        portEditor->setSizePolicy(sizePolicy2);
        portEditor->setMaximumSize(QSize(100, 16777215));
        portEditor->setFont(font2);

        horizontalLayout->addWidget(portEditor);

        testBtn = new QPushButton(widget_1);
        testBtn->setObjectName(QString::fromUtf8("testBtn"));
        sizePolicy2.setHeightForWidth(testBtn->sizePolicy().hasHeightForWidth());
        testBtn->setSizePolicy(sizePolicy2);
        testBtn->setFont(font2);

        horizontalLayout->addWidget(testBtn);


        verticalLayout->addWidget(widget_1);

        widget_2 = new QWidget(centralwidget);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        sizePolicy.setHeightForWidth(widget_2->sizePolicy().hasHeightForWidth());
        widget_2->setSizePolicy(sizePolicy);
        widget_2->setMaximumSize(QSize(16777215, 50));
        horizontalLayout_2 = new QHBoxLayout(widget_2);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        label_5 = new QLabel(widget_2);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setFont(font2);

        horizontalLayout_2->addWidget(label_5);

        savepathEditor = new QLineEdit(widget_2);
        savepathEditor->setObjectName(QString::fromUtf8("savepathEditor"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(savepathEditor->sizePolicy().hasHeightForWidth());
        savepathEditor->setSizePolicy(sizePolicy3);
        savepathEditor->setMinimumSize(QSize(0, 35));
        savepathEditor->setMaximumSize(QSize(16777215, 16777215));

        horizontalLayout_2->addWidget(savepathEditor);

        fileBtn = new QPushButton(widget_2);
        fileBtn->setObjectName(QString::fromUtf8("fileBtn"));
        sizePolicy2.setHeightForWidth(fileBtn->sizePolicy().hasHeightForWidth());
        fileBtn->setSizePolicy(sizePolicy2);
        fileBtn->setFont(font2);

        horizontalLayout_2->addWidget(fileBtn);


        verticalLayout->addWidget(widget_2);

        widget_3 = new QWidget(centralwidget);
        widget_3->setObjectName(QString::fromUtf8("widget_3"));
        sizePolicy.setHeightForWidth(widget_3->sizePolicy().hasHeightForWidth());
        widget_3->setSizePolicy(sizePolicy);
        widget_3->setMaximumSize(QSize(16777215, 50));
        horizontalLayout_4 = new QHBoxLayout(widget_3);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        label_6 = new QLabel(widget_3);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setFont(font2);

        horizontalLayout_4->addWidget(label_6);

        filenameEditor = new QLineEdit(widget_3);
        filenameEditor->setObjectName(QString::fromUtf8("filenameEditor"));
        sizePolicy3.setHeightForWidth(filenameEditor->sizePolicy().hasHeightForWidth());
        filenameEditor->setSizePolicy(sizePolicy3);
        filenameEditor->setMinimumSize(QSize(0, 35));
        filenameEditor->setMaximumSize(QSize(16777215, 16777215));

        horizontalLayout_4->addWidget(filenameEditor);


        verticalLayout->addWidget(widget_3);

        widget_4 = new QWidget(centralwidget);
        widget_4->setObjectName(QString::fromUtf8("widget_4"));
        sizePolicy.setHeightForWidth(widget_4->sizePolicy().hasHeightForWidth());
        widget_4->setSizePolicy(sizePolicy);
        widget_4->setMaximumSize(QSize(16777215, 50));
        horizontalLayout_3 = new QHBoxLayout(widget_4);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        label_4 = new QLabel(widget_4);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setFont(font2);

        horizontalLayout_3->addWidget(label_4);

        modeBox = new QComboBox(widget_4);
        modeBox->addItem(QString());
        modeBox->addItem(QString());
        modeBox->setObjectName(QString::fromUtf8("modeBox"));
        sizePolicy2.setHeightForWidth(modeBox->sizePolicy().hasHeightForWidth());
        modeBox->setSizePolicy(sizePolicy2);
        modeBox->setFont(font2);

        horizontalLayout_3->addWidget(modeBox);

        label_3 = new QLabel(widget_4);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setFont(font2);

        horizontalLayout_3->addWidget(label_3);

        sizeEditor = new QLineEdit(widget_4);
        sizeEditor->setObjectName(QString::fromUtf8("sizeEditor"));
        sizePolicy1.setHeightForWidth(sizeEditor->sizePolicy().hasHeightForWidth());
        sizeEditor->setSizePolicy(sizePolicy1);
        sizeEditor->setMaximumSize(QSize(16777215, 16777215));
        sizeEditor->setFont(font2);

        horizontalLayout_3->addWidget(sizeEditor);

        label_7 = new QLabel(widget_4);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setFont(font2);

        horizontalLayout_3->addWidget(label_7);

        timeoutEditor = new QLineEdit(widget_4);
        timeoutEditor->setObjectName(QString::fromUtf8("timeoutEditor"));
        sizePolicy1.setHeightForWidth(timeoutEditor->sizePolicy().hasHeightForWidth());
        timeoutEditor->setSizePolicy(sizePolicy1);
        timeoutEditor->setMaximumSize(QSize(16777215, 16777215));
        timeoutEditor->setFont(font2);

        horizontalLayout_3->addWidget(timeoutEditor);

        label_8 = new QLabel(widget_4);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setFont(font2);

        horizontalLayout_3->addWidget(label_8);

        retryEditor = new QLineEdit(widget_4);
        retryEditor->setObjectName(QString::fromUtf8("retryEditor"));
        sizePolicy1.setHeightForWidth(retryEditor->sizePolicy().hasHeightForWidth());
        retryEditor->setSizePolicy(sizePolicy1);
        retryEditor->setMaximumSize(QSize(16777215, 16777215));
        retryEditor->setFont(font2);

        horizontalLayout_3->addWidget(retryEditor);


        verticalLayout->addWidget(widget_4);

        widget_5 = new QWidget(centralwidget);
        widget_5->setObjectName(QString::fromUtf8("widget_5"));
        sizePolicy.setHeightForWidth(widget_5->sizePolicy().hasHeightForWidth());
        widget_5->setSizePolicy(sizePolicy);
        widget_5->setMaximumSize(QSize(16777215, 50));
        horizontalLayout_5 = new QHBoxLayout(widget_5);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(0, 0, 0, 0);
        uploadBtn = new QPushButton(widget_5);
        uploadBtn->setObjectName(QString::fromUtf8("uploadBtn"));
        sizePolicy2.setHeightForWidth(uploadBtn->sizePolicy().hasHeightForWidth());
        uploadBtn->setSizePolicy(sizePolicy2);
        uploadBtn->setFont(font2);

        horizontalLayout_5->addWidget(uploadBtn);

        downloadBtn = new QPushButton(widget_5);
        downloadBtn->setObjectName(QString::fromUtf8("downloadBtn"));
        sizePolicy2.setHeightForWidth(downloadBtn->sizePolicy().hasHeightForWidth());
        downloadBtn->setSizePolicy(sizePolicy2);
        downloadBtn->setFont(font2);

        horizontalLayout_5->addWidget(downloadBtn);


        verticalLayout->addWidget(widget_5);

        widget_6 = new QWidget(centralwidget);
        widget_6->setObjectName(QString::fromUtf8("widget_6"));
        sizePolicy.setHeightForWidth(widget_6->sizePolicy().hasHeightForWidth());
        widget_6->setSizePolicy(sizePolicy);
        widget_6->setMaximumSize(QSize(16777215, 50));
        horizontalLayout_6 = new QHBoxLayout(widget_6);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        horizontalLayout_6->setContentsMargins(0, 0, 0, 0);
        label_9 = new QLabel(widget_6);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setFont(font2);

        horizontalLayout_6->addWidget(label_9);

        logModeBox = new QComboBox(widget_6);
        logModeBox->addItem(QString());
        logModeBox->addItem(QString());
        logModeBox->addItem(QString());
        logModeBox->setObjectName(QString::fromUtf8("logModeBox"));
        sizePolicy1.setHeightForWidth(logModeBox->sizePolicy().hasHeightForWidth());
        logModeBox->setSizePolicy(sizePolicy1);
        logModeBox->setFont(font2);

        horizontalLayout_6->addWidget(logModeBox);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer);

        outputBtn = new QPushButton(widget_6);
        outputBtn->setObjectName(QString::fromUtf8("outputBtn"));
        sizePolicy2.setHeightForWidth(outputBtn->sizePolicy().hasHeightForWidth());
        outputBtn->setSizePolicy(sizePolicy2);
        outputBtn->setFont(font2);

        horizontalLayout_6->addWidget(outputBtn);


        verticalLayout->addWidget(widget_6);

        logBrowser = new QPlainTextEdit(centralwidget);
        logBrowser->setObjectName(QString::fromUtf8("logBrowser"));

        verticalLayout->addWidget(logBrowser);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "\350\277\236\346\216\245\345\234\260\345\235\200:", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "\347\253\257\345\217\243:", nullptr));
        testBtn->setText(QCoreApplication::translate("MainWindow", "\350\277\236\346\216\245\346\265\213\350\257\225", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "\346\234\254\345\234\260\346\226\207\344\273\266\350\267\257\345\276\204:", nullptr));
        fileBtn->setText(QCoreApplication::translate("MainWindow", "\346\265\217\350\247\210\346\234\254\345\234\260", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "\346\234\215\345\212\241\345\231\250\346\226\207\344\273\266\350\267\257\345\276\204:", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "\344\274\240\350\276\223\346\250\241\345\274\217:", nullptr));
        modeBox->setItemText(0, QCoreApplication::translate("MainWindow", "octet", nullptr));
        modeBox->setItemText(1, QCoreApplication::translate("MainWindow", "netascii", nullptr));

        label_3->setText(QCoreApplication::translate("MainWindow", "\346\225\260\346\215\256\345\214\205\345\244\247\345\260\217:", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "\346\227\266\351\231\220(s):", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "\351\207\215\344\274\240\346\254\241\346\225\260:", nullptr));
        uploadBtn->setText(QCoreApplication::translate("MainWindow", "\344\270\212\344\274\240", nullptr));
        downloadBtn->setText(QCoreApplication::translate("MainWindow", "\344\270\213\350\275\275", nullptr));
        label_9->setText(QCoreApplication::translate("MainWindow", "\346\227\245\345\277\227\347\255\211\347\272\247:", nullptr));
        logModeBox->setItemText(0, QCoreApplication::translate("MainWindow", "WARNING", nullptr));
        logModeBox->setItemText(1, QCoreApplication::translate("MainWindow", "INFO", nullptr));
        logModeBox->setItemText(2, QCoreApplication::translate("MainWindow", "DEBUG", nullptr));

        outputBtn->setText(QCoreApplication::translate("MainWindow", "\345\257\274\345\207\272", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
