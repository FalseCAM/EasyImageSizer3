/*
 EasyImageSizer - Rotate

 EasyImageSizer plugin to rotate an image

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

#include <QtCore>
#include <QSettings>
#include <QTranslator>
#include "rotate.h"
#include "ui_rotate.h"

Rotate::Rotate(QWidget *parent) :
	QWidget(parent), ui(new Ui::Rotate) {
	ui->setupUi(this);

	loadState();
}

Rotate::~Rotate() {
	saveState();
	delete ui;
}

QString Rotate::getName() {
	return QString("Rotate");
}

QString Rotate::getTitle() {
	return QString(tr("Rotate"));
}

QString Rotate::getVersion() {
	return QString("0.4");
}

QString Rotate::getAuthor() {
	return QString("FalseCAM");
}

QString Rotate::getDescription() {
	return QString(tr("Plugin to rotate images"));
}

QIcon Rotate::getIcon() {
	return QIcon("");
}

void Rotate::convert(EisImage *image) {
	emit progress(1);
	if (!ui->no_rotation->isChecked()) {
		emit progress(50);
		//  rotate matrix
		QMatrix m;
		if (ui->clockwise_rotation->isChecked())
			m.rotate(90.0);
		if (ui->clockwise180_rotation->isChecked())
			m.rotate(180.0);
		if (ui->counter_clockwise_rotation->isChecked())
			m.rotate(270.0);
		if (ui->angle_rotation->isChecked())
			m.rotate(ui->degreeBox->value());
		// transform image
		image->setImage(new QImage(image->getImage()->transformed(m)));
	}
	emit progress(100);
}

void Rotate::loadState() {
	QSettings settings(QCoreApplication::organizationName(),
			QCoreApplication::applicationName());
	settings.beginGroup("plugin");
	ui->degreeBox->setValue(settings.value("Degree", 0).toInt());
}

void Rotate::saveState() {
	QSettings settings(QCoreApplication::organizationName(),
			QCoreApplication::applicationName());
	settings.beginGroup("plugin");
	settings.beginGroup(QString(getName()));
	settings.setValue("Degree", ui->degreeBox->value());
}

Q_EXPORT_PLUGIN2(rotate, Rotate)
;
