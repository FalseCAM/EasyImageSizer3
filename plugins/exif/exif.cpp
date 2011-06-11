/*
 EasyImageSizer - Exif

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

#include "exif.h"
#include "ui_exif.h"
#include <QtCore>

#include <exiv2/exiv2.hpp>

Exif::Exif(QWidget *parent) :
	QWidget(parent), ui(new Ui::Exif) {
	ui->setupUi(this);
	loadState();
}

Exif::~Exif() {
	saveState();
	delete ui;
}

QString Exif::getName() {
	return QString("Exif");
}

QString Exif::getTitle() {
	return QString(tr("Rename by Exifdata"));
}

QString Exif::getVersion() {
	return QString("0.3");
}

QString Exif::getAuthor() {
	return QString("FalseCAM");
}

QString Exif::getDescription() {
	return QString(tr("Plugin to rename Images by Exifdata"));
}

QIcon Exif::getIcon() {
	return QIcon("");
}

void Exif::convert(EisImage *image) {
	if (ui->prependRadio->isChecked()) {
		image->setName(
				exifToString(image->getOriginalFile()) + image->getName());
	} else if (ui->appendRadio->isChecked()) {
		image->setName(
				image->getName() + exifToString(image->getOriginalFile()));
	} else {
	}
}

QString Exif::exifToString(QString origImage) {
	return getDateTime(origImage).toString(ui->lineEdit->text());
}

QDateTime Exif::getDateTime(QString file) {
	emit progress(1);
	QDateTime DateTime;

	Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(
			file.toStdString().c_str());
	image->readMetadata();

	Exiv2::ExifData& exifData = image->exifData();

	Exiv2::ExifData::iterator pos = exifData.findKey(
			Exiv2::ExifKey("Exif.Photo.DateTimeOriginal"));

	DateTime = QDateTime::fromString(
			QString().fromStdString(pos->value().toString()),
			"yyyy:MM:dd hh:mm:ss");

	// Check if there is a correct Date
	if (!DateTime.isValid())
		return QDateTime(QDate(1900, 1, 1), QTime(0, 0, 0));emit
	progress(100);
	return DateTime;
}

// loads last state of gui
void Exif::loadState() {
	QSettings settings(QCoreApplication::organizationName(),
			QCoreApplication::applicationName());
	settings.beginGroup("plugin");
	settings.beginGroup(QString(getName()));
	ui->prependRadio->setChecked(settings.value("Prepend", false).toBool());
	ui->appendRadio->setChecked(settings.value("Append", false).toBool());
	ui->lineEdit->setText(settings.value("String", "").toString());
}

// saves state of gui
void Exif::saveState() {
	QSettings settings(QCoreApplication::organizationName(),
			QCoreApplication::applicationName());
	settings.beginGroup("plugin");
	settings.beginGroup(QString(getName()));
	settings.setValue("Prepend", ui->prependRadio->isChecked());
	settings.setValue("Append", ui->appendRadio->isChecked());
	settings.setValue("String", ui->lineEdit->text());

}

Q_EXPORT_PLUGIN2(exif, Exif)
;
