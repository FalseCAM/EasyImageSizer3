/*
    EasyImageSizer - Rename

    EasyImageSizer plugin rename images

    Copyright (C) 2010 - 2011 FalseCAM

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "rename.h"
#include "ui_rename.h"
#include <QtCore>
#include <QSettings>

Rename::Rename(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::Rename)
{
    ui->setupUi(this);
    loadState();
}

Rename::~Rename()
{
    saveState();
    delete ui;
}

QString Rename::getName(){
    return QString("Rename");
}

QString Rename::getTitle(){
    return QString(tr("Rename"));
}

QString Rename::getVersion(){
    return QString("0.3");
}

QString Rename::getAuthor(){
    return QString("FalseCAM");
}

QString Rename::getDescription(){
    return QString(tr("Plugin to Rename Images"));
}

QIcon Rename::getIcon(){
    return QIcon();
}

void Rename::convert(EisImage *image){
    emit progress(50);
    QString fileName = image->getName();
    if(ui->counterCheckBox->isChecked() && !(ui->prefixLineEdit->text().isEmpty() && ui->suffixLineEdit->text().isEmpty())){
        fileName = QString(ui->prefixLineEdit->text());
        if(ui->counterCheckBox->isChecked())
            fileName.append(QString("%1").arg(ui->startSpinBox->value()+image->getIndex(), ui->decimalSpinBox->value(), 10, QChar('0')) );
        fileName.append(ui->suffixLineEdit->text());
    }
    image->setName(fileName);
    emit progress(100);
}

// loads last state of gui
void Rename::loadState(){
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.beginGroup("plugin");
    settings.beginGroup(QString(getName()));
    ui->renameCheckBox->setChecked(settings.value("RenameUse", false).toBool());
    ui->counterCheckBox->setChecked(settings.value("CounterUse", false).toBool());
    ui->prefixLineEdit->setText(settings.value("Prefix", "").toString());
    ui->suffixLineEdit->setText(settings.value("Suffix", "").toString());
    ui->startSpinBox->setValue(settings.value("Start", 1).toInt());
    ui->decimalSpinBox->setValue(settings.value("Decimals", 0).toInt());
}

// saves state of gui
void Rename::saveState(){
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.beginGroup("plugin");
    settings.beginGroup(QString(getName()));
    settings.setValue("RenameUse", ui->renameCheckBox->isChecked());
    settings.setValue("CounterUse", ui->counterCheckBox->isChecked());
    settings.setValue("Prefix", ui->prefixLineEdit->text());
    settings.setValue("Suffix", ui->suffixLineEdit->text());
    settings.setValue("Start", ui->startSpinBox->value());
    settings.setValue("Decimals", ui->decimalSpinBox->value());
}

Q_EXPORT_PLUGIN2(rename, Rename);
