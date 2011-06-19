/********************************************************************************
** Form generated from reading UI file 'exif.ui'
**
** Created: Sun 19. Jun 10:19:39 2011
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EXIF_H
#define UI_EXIF_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLineEdit>
#include <QtGui/QRadioButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Exif
{
public:
    QGridLayout *gridLayout;
    QLineEdit *lineEdit;
    QRadioButton *prependRadio;
    QRadioButton *appendRadio;
    QRadioButton *nothingRadio;

    void setupUi(QWidget *Exif)
    {
        if (Exif->objectName().isEmpty())
            Exif->setObjectName(QString::fromUtf8("Exif"));
        Exif->resize(380, 240);
        gridLayout = new QGridLayout(Exif);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        lineEdit = new QLineEdit(Exif);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        gridLayout->addWidget(lineEdit, 3, 0, 1, 1);

        prependRadio = new QRadioButton(Exif);
        prependRadio->setObjectName(QString::fromUtf8("prependRadio"));

        gridLayout->addWidget(prependRadio, 1, 0, 1, 1);

        appendRadio = new QRadioButton(Exif);
        appendRadio->setObjectName(QString::fromUtf8("appendRadio"));

        gridLayout->addWidget(appendRadio, 2, 0, 1, 1);

        nothingRadio = new QRadioButton(Exif);
        nothingRadio->setObjectName(QString::fromUtf8("nothingRadio"));
        nothingRadio->setChecked(true);

        gridLayout->addWidget(nothingRadio, 0, 0, 1, 1);


        retranslateUi(Exif);

        QMetaObject::connectSlotsByName(Exif);
    } // setupUi

    void retranslateUi(QWidget *Exif)
    {
        Exif->setWindowTitle(QApplication::translate("Exif", "Form", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        lineEdit->setToolTip(QApplication::translate("Exif", "Example: yyyyMMdd_hhmmss_", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        lineEdit->setText(QApplication::translate("Exif", "yyyyMMdd_hhmmss_", 0, QApplication::UnicodeUTF8));
        prependRadio->setText(QApplication::translate("Exif", "prepend", 0, QApplication::UnicodeUTF8));
        appendRadio->setText(QApplication::translate("Exif", "append", 0, QApplication::UnicodeUTF8));
        nothingRadio->setText(QApplication::translate("Exif", "change nothing", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Exif: public Ui_Exif {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EXIF_H
