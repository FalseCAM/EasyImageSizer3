/*
 EasyImageSizer - Geotag

 EasyImageSizer plugin rename images

 Copyright (C) 2010 - 2011 FalseCAM

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "geotag.h"
#include "ui_geotag.h"
#include <QtCore>

#include <exiv2/exiv2.hpp>

Geotag::Geotag() :
    ui(new Ui::Geotag) {
    widget = new QWidget();
    ui->setupUi(widget);
    this->map = new Map(widget);
    ui->mapGrid->addWidget(map);
    connect(ui->gotoButton, SIGNAL(clicked()), this, SLOT(on_gotoButton_clicked()));
    connect(map, SIGNAL(latChanged(qreal)), this, SLOT(latChanged(qreal)));
    connect(map, SIGNAL(lngChanged(qreal)), this, SLOT(lngChanged(qreal)));
    // Zoom Buttons
    connect(ui->zoomInButton, SIGNAL(clicked()), map, SLOT(zoomIn()));
    connect(ui->zoomOutButton, SIGNAL(clicked()), map, SLOT(zoomOut()));
	loadState();
}

Geotag::~Geotag() {
	saveState();
    widget->close();
	delete ui;
    delete widget;
    // TODO: solve crashes
    //delete map;
}

QString Geotag::getName() {
	return QString("Geotag");
}

QString Geotag::getTitle() {
    return QString(tr("Change Geotagdata"));
}

QString Geotag::getVersion() {
        return QString("0.1");
}

QString Geotag::getAuthor() {
	return QString("FalseCAM");
}

QString Geotag::getDescription() {
    return QString(tr("Plugin to change Geotagdata in Exif"));
}

QIcon Geotag::getIcon() {
	return QIcon("");
}

QWidget * Geotag::createWidget()
{
    return widget;
}

void Geotag::convert(EisImage *image) {
    if(ui->geotagCheckBox->isChecked()){
        char scratchBuf[100];
        long int deg, min;
        qreal lat = ui->latLineEdit->text().toDouble();
        qreal lng = ui->lngLineEdit->text().toDouble();

        if(lng < 0)
            image->setExifKey("Exif.GPSInfo.GPSLongitudeRef", "W");
        else image->setExifKey("Exif.GPSInfo.GPSLongitudeRef", "E");

        if(lat > 0)
            image->setExifKey("Exif.GPSInfo.GPSLatitudeRef", "N");
        else image->setExifKey("Exif.GPSInfo.GPSLatitudeRef", "S");

        deg = (int)floor(fabs(lng)); // Slice off after decimal.
        min = (int)floor((fabs(lng) - floor(fabs(lng))) * 60000000);
        snprintf(scratchBuf, 100, "%ld/1 %ld/1000000 0/1", deg, min);
        image->setExifKeyRational("Exif.GPSInfo.GPSLongitude", QString(scratchBuf));

        deg = (int)floor(fabs(lat)); // Slice off after decimal.
        min = (int)floor((fabs(lat) - floor(fabs(lat))) * 60000000);
        snprintf(scratchBuf, 100, "%ld/1 %ld/1000000 0/1", deg, min);
        image->setExifKeyRational("Exif.GPSInfo.GPSLatitude", QString(scratchBuf));
    }
}

// loads last state of gui
void Geotag::loadState() {
	QSettings settings(QCoreApplication::organizationName(),
			QCoreApplication::applicationName());
	settings.beginGroup("plugin");
    settings.beginGroup(QString(getName()));
    ui->latLineEdit->setText(settings.value("latitude", "50.0000").toString());
    ui->lngLineEdit->setText(settings.value("longitude", "8.0000").toString());
    on_gotoButton_clicked();
}

// saves state of gui
void Geotag::saveState() {
	QSettings settings(QCoreApplication::organizationName(),
			QCoreApplication::applicationName());
	settings.beginGroup("plugin");
    settings.beginGroup(QString(getName()));
    settings.setValue("latitude", ui->latLineEdit->text());
    settings.setValue("longitude", ui->lngLineEdit->text());
}

void Geotag::latChanged(qreal lat){
    ui->latLineEdit->setText(QString::number(lat));
}

void Geotag::lngChanged(qreal lng){
    ui->lngLineEdit->setText(QString::number(lng));
}

void Geotag::on_gotoButton_clicked()
{
    qreal lat = ui->latLineEdit->text().toDouble();
    qreal lng = ui->lngLineEdit->text().toDouble();
    if(lat < 90 && lat > -90)
        map->setCenter(lat , lng);
    map->invalidate();
}
Q_EXPORT_PLUGIN2(geotag, Geotag);
