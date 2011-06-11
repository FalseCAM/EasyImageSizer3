/*
    EasyImageSizer - Overlay

    EasyImageSizer plugin to Overlay an image to a QImage

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

#include "overlay.h"
#include "ui_overlay.h"
#include <QtCore>
#include <QFile>
#include <QFileDialog>
#include <QPainter>
#include <QSettings>
#include <QTranslator>

Overlay::Overlay(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Overlay)
{
    ui->setupUi(this);

    // loads last state of gui
    loadState();

    connect(ui->overlayButton, SIGNAL(clicked()), this, SLOT(setOverlayImage()));
}

Overlay::~Overlay()
{
    saveState();
    delete ui;
}

QString Overlay::getName(){
    return QString("Overlay");
}

QString Overlay::getTitle(){
    return QString(tr("Overlay"));
}

QString Overlay::getVersion(){
    return QString("0.2");
}

QString Overlay::getAuthor(){
    return QString("FalseCAM");
}

QString Overlay::getDescription(){
    return QString(tr("Plugin to overlay an image (copyright) over another image"));
}

QIcon Overlay::getIcon(){
    return QIcon("");
}

void Overlay::convert(EisImage *image){
    if(ui->overlayCheckBox->isChecked())
    {
        emit progress(50);
        if( QFile(ui->overlayImage->text()).exists()){
            image->setImage(addOverlay(image->getImage(), ui->overlayImage->text()));
        }
    }
    emit progress(100);
}

// Set Overlay Image to data
void Overlay::setOverlayImage(){
    // opens a Dialog to choose save directory
    QString file = QFileDialog::getOpenFileName(this, tr("Please select a file"), ui->overlayImage->text(),
                                                "All Supported Image Files (*.bmp *.png *.tif *.tiff);;"
                                                "All Files (*.*)"
                                                );
    // checks for dir exists
    if( QFile(file).exists() )
    {
        // save chosen dir
        this->ui->overlayImage->setText(file);
        ui->image->setPixmap(QPixmap(this->ui->overlayImage->text()));
    }
}

// add Overlay
QImage Overlay::addOverlay(QImage image, QString overlay){
    if(overlay.isEmpty()) return image;
    QImage result(image);
    QPainter painter(&result);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.fillRect(image.rect(), Qt::transparent);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawImage(0, 0, image);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawImage(0, 0, QImage(overlay).scaled(image.width(), image.height()));
    painter.end();
    return result;
}

void Overlay::loadState(){
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.beginGroup("plugin");
    settings.beginGroup(QString(getName()));
    ui->overlayCheckBox->setChecked(settings.value("OverlayUse").toBool());
    ui->overlayImage->setText(settings.value("OverlayImage").toString());
    if(!ui->overlayImage->text().isEmpty())
        ui->image->setPixmap(QPixmap(this->ui->overlayImage->text()));
}

void Overlay::saveState(){
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.beginGroup("plugin");
    settings.beginGroup(QString(getName()));
    settings.setValue("OverlayUse", ui->overlayCheckBox->isChecked());
    settings.setValue("OverlayImage", ui->overlayImage->text());
}

Q_EXPORT_PLUGIN2(overlay, Overlay);
