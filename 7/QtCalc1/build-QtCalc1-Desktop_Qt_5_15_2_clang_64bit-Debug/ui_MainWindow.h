/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *about_action;
    QWidget *centralwidget;
    QTextEdit *displayTextEdit;
    QPushButton *leftParenButton;
    QPushButton *rightParenButton;
    QPushButton *clearButton;
    QPushButton *divideButton;
    QPushButton *number7Button;
    QPushButton *number8Button;
    QPushButton *number9Button;
    QPushButton *multiplyButton;
    QPushButton *number4Button;
    QPushButton *number5Button;
    QPushButton *number6Button;
    QPushButton *subtractButton;
    QPushButton *number1Button;
    QPushButton *number2Button;
    QPushButton *number3Button;
    QPushButton *addButton;
    QPushButton *changeSignButton;
    QPushButton *number0Button;
    QPushButton *decimalPointButton;
    QPushButton *evaluateButton;
    QMenuBar *menubar;
    QMenu *menuHelp;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(400, 600);
        about_action = new QAction(MainWindow);
        about_action->setObjectName(QString::fromUtf8("about_action"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        displayTextEdit = new QTextEdit(centralwidget);
        displayTextEdit->setObjectName(QString::fromUtf8("displayTextEdit"));
        displayTextEdit->setGeometry(QRect(6, 6, 388, 88));
        QFont font;
        font.setPointSize(36);
        displayTextEdit->setFont(font);
        displayTextEdit->setAlignment(Qt::AlignCenter);
        leftParenButton = new QPushButton(centralwidget);
        leftParenButton->setObjectName(QString::fromUtf8("leftParenButton"));
        leftParenButton->setGeometry(QRect(0, 100, 100, 100));
        leftParenButton->setFont(font);
        rightParenButton = new QPushButton(centralwidget);
        rightParenButton->setObjectName(QString::fromUtf8("rightParenButton"));
        rightParenButton->setGeometry(QRect(100, 100, 100, 100));
        rightParenButton->setFont(font);
        clearButton = new QPushButton(centralwidget);
        clearButton->setObjectName(QString::fromUtf8("clearButton"));
        clearButton->setGeometry(QRect(200, 100, 100, 100));
        clearButton->setFont(font);
        divideButton = new QPushButton(centralwidget);
        divideButton->setObjectName(QString::fromUtf8("divideButton"));
        divideButton->setGeometry(QRect(300, 100, 100, 100));
        divideButton->setFont(font);
        number7Button = new QPushButton(centralwidget);
        number7Button->setObjectName(QString::fromUtf8("number7Button"));
        number7Button->setGeometry(QRect(0, 200, 100, 100));
        number7Button->setFont(font);
        number8Button = new QPushButton(centralwidget);
        number8Button->setObjectName(QString::fromUtf8("number8Button"));
        number8Button->setGeometry(QRect(100, 200, 100, 100));
        number8Button->setFont(font);
        number9Button = new QPushButton(centralwidget);
        number9Button->setObjectName(QString::fromUtf8("number9Button"));
        number9Button->setGeometry(QRect(200, 200, 100, 100));
        number9Button->setFont(font);
        multiplyButton = new QPushButton(centralwidget);
        multiplyButton->setObjectName(QString::fromUtf8("multiplyButton"));
        multiplyButton->setGeometry(QRect(300, 200, 100, 100));
        multiplyButton->setFont(font);
        number4Button = new QPushButton(centralwidget);
        number4Button->setObjectName(QString::fromUtf8("number4Button"));
        number4Button->setGeometry(QRect(0, 300, 100, 100));
        number4Button->setFont(font);
        number5Button = new QPushButton(centralwidget);
        number5Button->setObjectName(QString::fromUtf8("number5Button"));
        number5Button->setGeometry(QRect(100, 300, 100, 100));
        number5Button->setFont(font);
        number6Button = new QPushButton(centralwidget);
        number6Button->setObjectName(QString::fromUtf8("number6Button"));
        number6Button->setGeometry(QRect(200, 300, 100, 100));
        number6Button->setFont(font);
        subtractButton = new QPushButton(centralwidget);
        subtractButton->setObjectName(QString::fromUtf8("subtractButton"));
        subtractButton->setGeometry(QRect(300, 300, 100, 100));
        subtractButton->setFont(font);
        number1Button = new QPushButton(centralwidget);
        number1Button->setObjectName(QString::fromUtf8("number1Button"));
        number1Button->setGeometry(QRect(0, 400, 100, 100));
        number1Button->setFont(font);
        number2Button = new QPushButton(centralwidget);
        number2Button->setObjectName(QString::fromUtf8("number2Button"));
        number2Button->setGeometry(QRect(100, 400, 100, 100));
        number2Button->setFont(font);
        number3Button = new QPushButton(centralwidget);
        number3Button->setObjectName(QString::fromUtf8("number3Button"));
        number3Button->setGeometry(QRect(200, 400, 100, 100));
        number3Button->setFont(font);
        addButton = new QPushButton(centralwidget);
        addButton->setObjectName(QString::fromUtf8("addButton"));
        addButton->setGeometry(QRect(300, 400, 100, 100));
        addButton->setFont(font);
        changeSignButton = new QPushButton(centralwidget);
        changeSignButton->setObjectName(QString::fromUtf8("changeSignButton"));
        changeSignButton->setGeometry(QRect(0, 500, 100, 100));
        changeSignButton->setFont(font);
        number0Button = new QPushButton(centralwidget);
        number0Button->setObjectName(QString::fromUtf8("number0Button"));
        number0Button->setGeometry(QRect(100, 500, 100, 100));
        number0Button->setFont(font);
        decimalPointButton = new QPushButton(centralwidget);
        decimalPointButton->setObjectName(QString::fromUtf8("decimalPointButton"));
        decimalPointButton->setGeometry(QRect(200, 500, 100, 100));
        decimalPointButton->setFont(font);
        evaluateButton = new QPushButton(centralwidget);
        evaluateButton->setObjectName(QString::fromUtf8("evaluateButton"));
        evaluateButton->setGeometry(QRect(300, 500, 100, 100));
        evaluateButton->setFont(font);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 786, 22));
        menuHelp = new QMenu(menubar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        MainWindow->setMenuBar(menubar);

        menubar->addAction(menuHelp->menuAction());
        menuHelp->addAction(about_action);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "ENGN2912B 2017 | QtCalc1", nullptr));
        about_action->setText(QCoreApplication::translate("MainWindow", "About...", nullptr));
        displayTextEdit->setText(QString());
        leftParenButton->setText(QCoreApplication::translate("MainWindow", "(", nullptr));
        rightParenButton->setText(QCoreApplication::translate("MainWindow", ")", nullptr));
        clearButton->setText(QCoreApplication::translate("MainWindow", "C", nullptr));
        divideButton->setText(QCoreApplication::translate("MainWindow", "/", nullptr));
        number7Button->setText(QCoreApplication::translate("MainWindow", "7", nullptr));
        number8Button->setText(QCoreApplication::translate("MainWindow", "8", nullptr));
        number9Button->setText(QCoreApplication::translate("MainWindow", "9", nullptr));
        multiplyButton->setText(QCoreApplication::translate("MainWindow", "*", nullptr));
        number4Button->setText(QCoreApplication::translate("MainWindow", "4", nullptr));
        number5Button->setText(QCoreApplication::translate("MainWindow", "5", nullptr));
        number6Button->setText(QCoreApplication::translate("MainWindow", "6", nullptr));
        subtractButton->setText(QCoreApplication::translate("MainWindow", "-", nullptr));
        number1Button->setText(QCoreApplication::translate("MainWindow", "1", nullptr));
        number2Button->setText(QCoreApplication::translate("MainWindow", "2", nullptr));
        number3Button->setText(QCoreApplication::translate("MainWindow", "3", nullptr));
        addButton->setText(QCoreApplication::translate("MainWindow", "+", nullptr));
        changeSignButton->setText(QCoreApplication::translate("MainWindow", "+/-", nullptr));
        number0Button->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        decimalPointButton->setText(QCoreApplication::translate("MainWindow", ".", nullptr));
        evaluateButton->setText(QCoreApplication::translate("MainWindow", "=", nullptr));
        menuHelp->setTitle(QCoreApplication::translate("MainWindow", "Help", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
