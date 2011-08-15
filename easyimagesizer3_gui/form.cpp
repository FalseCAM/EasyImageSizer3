/* (C) 2011 FalseCAM
    This file is part of EasyImageSizer-GUI.

    EasyImageSizer is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    EasyImageSizer is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with EasyImageSizer.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "form.h"
#include "ui_form.h"
#include <QImageWriter>
#include <QDebug>
#include <QFileDialog>
#include <QSettings>

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
    initSaveFormats();
    loadConfig();
}

Form::~Form()
{
    saveConfig();
    delete ui;
}

QString Form::getTitle(){
    return "";
}

void Form::initSaveFormats(){
    foreach(const QByteArray &fmt, QImageWriter::supportedImageFormats()){
        ui->saveFormatsComboBox->addItem(QString(fmt));
    }
}

QString Form::getImageFormat(){
    return this->ui->saveFormatsComboBox->currentText();
}

/*
  Returns a value between 0 and 100.
  This is a quality value to save images in.
  Value is -1 if nothing should be changed.
  */
int Form::getImageQuality(){
    if(ui->changeQualityCheckBox->isChecked())
        return this->ui->qualitySpinBox->value();
    else
        return -1;
}

bool Form::getCopyMetaData(){
	return ui->copyMetadataCheckBox->isChecked();
}

// set Save Dir
void Form::setSaveToDir()
{
    // opens a Dialog to choose save directory
    QString dir = QFileDialog::getExistingDirectory(
                this,
                "Please select a directory.",
                this->lastOpenFileDir );
    // checks for dir exists
    if( QDir(dir).exists() )
    {
        // saves last chosen savedir
        this->lastOpenFileDir = dir;
        // save chosen dir
        ui->saveToLineEdit->setText(dir);
    }
}

QString Form::getSaveFolder(){
    return this->ui->saveToLineEdit->text();
}

bool Form::getSaveTo(){
    return this->ui->saveToCheckBox->isChecked();
}

void Form::on_pushButton_clicked()
{
    this->setSaveToDir();
}

// saves state of selections into config file
void Form::loadConfig(){
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    ui->changeQualityCheckBox->setChecked(settings.value("changeQuality", false).toBool());
    ui->qualitySpinBox->setValue(settings.value("quality", 80).toInt());
    ui->saveFormatsComboBox->setCurrentIndex(
                ui->saveFormatsComboBox->findText(settings.value("format", "jpg").toString()));
    ui->saveToCheckBox->setChecked(settings.value("saveTo", false).toBool());
    ui->saveToLineEdit->setText(settings.value("saveToDir", QDir::homePath()).toString());
}

// loads state of selections
void Form::saveConfig(){
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.setValue("changeQuality", ui->changeQualityCheckBox->isChecked());
    settings.setValue("quality", ui->qualitySpinBox->value());
    settings.setValue("format", ui->saveFormatsComboBox->currentText());
    settings.setValue("saveTo", ui->saveToCheckBox->isChecked());
    settings.setValue("saveToDir", ui->saveToLineEdit->text());
}

void Form::retranslateUi(){
    this->ui->retranslateUi(this);
}
