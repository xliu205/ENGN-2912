/********************************************************************************
** Form generated from reading UI file 'AboutDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ABOUTDIALOG_H
#define UI_ABOUTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_AboutDialog
{
public:
    QVBoxLayout *verticalLayout_1;
    QLabel *title_label;
    QFormLayout *formLayout;
    QLabel *qt_version_title_label;
    QLabel *qt_version_label;
    QLabel *built_date_title_label;
    QLabel *built_date_label;
    QLabel *author_label;
    QPushButton *close_button;

    void setupUi(QDialog *AboutDialog)
    {
        if (AboutDialog->objectName().isEmpty())
            AboutDialog->setObjectName(QString::fromUtf8("AboutDialog"));
        AboutDialog->setWindowModality(Qt::ApplicationModal);
        verticalLayout_1 = new QVBoxLayout(AboutDialog);
        verticalLayout_1->setObjectName(QString::fromUtf8("verticalLayout_1"));
        title_label = new QLabel(AboutDialog);
        title_label->setObjectName(QString::fromUtf8("title_label"));
        QFont font;
        font.setPointSize(16);
        title_label->setFont(font);
        title_label->setAlignment(Qt::AlignCenter);

        verticalLayout_1->addWidget(title_label);

        formLayout = new QFormLayout();
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        qt_version_title_label = new QLabel(AboutDialog);
        qt_version_title_label->setObjectName(QString::fromUtf8("qt_version_title_label"));
        QFont font1;
        font1.setPointSize(10);
        qt_version_title_label->setFont(font1);

        formLayout->setWidget(1, QFormLayout::LabelRole, qt_version_title_label);

        qt_version_label = new QLabel(AboutDialog);
        qt_version_label->setObjectName(QString::fromUtf8("qt_version_label"));
        qt_version_label->setFont(font1);

        formLayout->setWidget(1, QFormLayout::FieldRole, qt_version_label);

        built_date_title_label = new QLabel(AboutDialog);
        built_date_title_label->setObjectName(QString::fromUtf8("built_date_title_label"));
        built_date_title_label->setFont(font1);

        formLayout->setWidget(2, QFormLayout::LabelRole, built_date_title_label);

        built_date_label = new QLabel(AboutDialog);
        built_date_label->setObjectName(QString::fromUtf8("built_date_label"));
        built_date_label->setFont(font1);

        formLayout->setWidget(2, QFormLayout::FieldRole, built_date_label);


        verticalLayout_1->addLayout(formLayout);

        author_label = new QLabel(AboutDialog);
        author_label->setObjectName(QString::fromUtf8("author_label"));
        author_label->setFont(font1);
        author_label->setAlignment(Qt::AlignCenter);

        verticalLayout_1->addWidget(author_label);

        close_button = new QPushButton(AboutDialog);
        close_button->setObjectName(QString::fromUtf8("close_button"));
        close_button->setFont(font1);

        verticalLayout_1->addWidget(close_button);


        retranslateUi(AboutDialog);

        QMetaObject::connectSlotsByName(AboutDialog);
    } // setupUi

    void retranslateUi(QDialog *AboutDialog)
    {
        AboutDialog->setWindowTitle(QCoreApplication::translate("AboutDialog", "QtCalc", nullptr));
        title_label->setText(QCoreApplication::translate("AboutDialog", "ENGN2912B 2017 Qt Calculator", nullptr));
        qt_version_title_label->setText(QCoreApplication::translate("AboutDialog", "Qt Version:", nullptr));
        qt_version_label->setText(QCoreApplication::translate("AboutDialog", "qt ver", nullptr));
        built_date_title_label->setText(QCoreApplication::translate("AboutDialog", "Built on:", nullptr));
        built_date_label->setText(QCoreApplication::translate("AboutDialog", "build date time", nullptr));
        author_label->setText(QCoreApplication::translate("AboutDialog", "(c) 2017 Your Name", nullptr));
        close_button->setText(QCoreApplication::translate("AboutDialog", "Close", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AboutDialog: public Ui_AboutDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABOUTDIALOG_H
