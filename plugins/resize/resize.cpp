/*
 EasyImageSizer - Resize

 EasyImageSizer plugin to Resize a QImage

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
#include <QCoreApplication>
#include <QSettings>
#include "resize.h"
#include "ui_resize.h"

Resize::Resize(QWidget *parent) :
	QWidget(parent), ui(new Ui::Resize) {
	ui->setupUi(this);

	loadState();

	connect(ui->SizePercentRadioButton, SIGNAL(toggled(bool)), this,
			SLOT(changeSizePercentUse()));
	connect(ui->SizePixelRadioButton, SIGNAL(toggled(bool)), this,
			SLOT(changeSizePixelUse()));
	connect(ui->PixelComboBox, SIGNAL(activated(QString)), this,
			SLOT(changeSizePixel()));
	connect(ui->SizeRatioComboBox, SIGNAL(activated(QString)), this,
			SLOT(changeSizeRatio()));
}

Resize::~Resize() {
	saveState();
	delete ui;
}

QString Resize::getName() {
	return QString("Resize");
}

QString Resize::getTitle() {
	return QString(tr("Resize"));
}

QString Resize::getVersion() {
	return QString("0.3");
}

QString Resize::getAuthor() {
	return QString("FalseCAM");
}

QString Resize::getDescription() {
	return QString(tr("Plugin to resize images"));
}

QIcon Resize::getIcon() {
	return QIcon("");
}

void Resize::convert(EisImage *image) {
	emit progress(1);
	// scale image by percentage
	if (ui->SizePercentRadioButton->isChecked()) {
		image->setImage(
				new QImage(
						image->getImage()->scaled(
								image->getImage()->width()
										* ui->SizeXPercentSpinBox->value()
										/ 100,
								image->getImage()->height()
										* ui->SizeYPercentSpinBox->value()
										/ 100, Qt::IgnoreAspectRatio)));
	} else if (ui->SizePixelRadioButton->isChecked()) {
		image->setImage(
				new QImage(
						image->getImage()->scaled(
								ui->SizeXPixelSpinBox->value(),
								ui->SizeYPixelSpinBox->value(),
								Qt::IgnoreAspectRatio)));
	} else {
	}
	emit progress(100);
}

void Resize::changeSizePercentUse() {
	if (ui->SizePercentRadioButton->isChecked()) {
		ui->SizeXPercentHorizontalSlider->setEnabled(true);
		ui->SizeXPercentSpinBox->setEnabled(true);
		ui->SizeYPercentHorizontalSlider->setEnabled(true);
		ui->SizeYPercentSpinBox->setEnabled(true);
	} else {
		ui->SizeXPercentHorizontalSlider->setEnabled(false);
		ui->SizeXPercentSpinBox->setEnabled(false);
		ui->SizeYPercentHorizontalSlider->setEnabled(false);
		ui->SizeYPercentSpinBox->setEnabled(false);
	}
}

void Resize::changeSizePixelUse() {
	if (ui->SizePixelRadioButton->isChecked()) {
		ui->SizeXPixelSpinBox->setEnabled(true);
		ui->SizeYPixelSpinBox->setEnabled(true);
	} else {
		ui->SizeXPixelSpinBox->setEnabled(false);
		ui->SizeYPixelSpinBox->setEnabled(false);
	}
}

void Resize::changeSizePixel() {
	ui->SizePixelRadioButton->setChecked(true);
	if (ui->PixelComboBox->currentText() == "VGA (640x480)") {
		ui->SizeXPixelSpinBox->setValue(640);
		ui->SizeYPixelSpinBox->setValue(480);
	} else if (ui->PixelComboBox->currentText() == "XGA (1024x768)") {
		ui->SizeXPixelSpinBox->setValue(1024);
		ui->SizeYPixelSpinBox->setValue(768);
	} else if (ui->PixelComboBox->currentText() == "NTSC (576x486)") {
		ui->SizeXPixelSpinBox->setValue(576);
		ui->SizeYPixelSpinBox->setValue(486);
	} else if (ui->PixelComboBox->currentText() == "PAL (720x486)") {
		ui->SizeXPixelSpinBox->setValue(720);
		ui->SizeYPixelSpinBox->setValue(486);
	} else if (ui->PixelComboBox->currentText() == "HD (1920x1080)") {
		ui->SizeXPixelSpinBox->setValue(1920);
		ui->SizeYPixelSpinBox->setValue(1080);
	} else if (ui->PixelComboBox->currentText() == "Flickr large (768x1024)") {
		ui->SizeXPixelSpinBox->setValue(768);
		ui->SizeYPixelSpinBox->setValue(1024);
	} else if (ui->PixelComboBox->currentText() == "iPhone landscape (480x320)") {
		ui->SizeXPixelSpinBox->setValue(480);
		ui->SizeYPixelSpinBox->setValue(320);
	} else if (ui->PixelComboBox->currentText() == "160x160") {
		ui->SizeXPixelSpinBox->setValue(160);
		ui->SizeYPixelSpinBox->setValue(160);
	} else if (ui->PixelComboBox->currentText() == "HD 720 (1280x720)") {
		ui->SizeXPixelSpinBox->setValue(1280);
		ui->SizeYPixelSpinBox->setValue(720);
	} else if (ui->PixelComboBox->currentText() == "SVGA (800x600)") {
		ui->SizeXPixelSpinBox->setValue(800);
		ui->SizeYPixelSpinBox->setValue(600);
	} else if (ui->PixelComboBox->currentText() == "PSP (480x272)") {
		ui->SizeXPixelSpinBox->setValue(480);
		ui->SizeYPixelSpinBox->setValue(272);
	} else if (ui->PixelComboBox->currentText() == "2K (2048x1080)") {
		ui->SizeXPixelSpinBox->setValue(2048);
		ui->SizeYPixelSpinBox->setValue(1080);
	}
}

void Resize::changeSizeRatio() {
	ui->SizePercentRadioButton->setChecked(true);
	if (ui->SizeRatioComboBox->currentText() == "1:1") {
		ui->SizeYPercentSpinBox->setValue(
				ui->SizeXPercentSpinBox->value() * (1.0 / 1));
	} else if (ui->SizeRatioComboBox->currentText() == "5:4") {
		ui->SizeYPercentSpinBox->setValue(
				ui->SizeXPercentSpinBox->value() * (4.0 / 5));
	} else if (ui->SizeRatioComboBox->currentText() == "4:3") {
		ui->SizeYPercentSpinBox->setValue(
				ui->SizeXPercentSpinBox->value() * (3.0 / 4));
	} else if (ui->SizeRatioComboBox->currentText() == "16:9") {
		ui->SizeYPercentSpinBox->setValue(
				ui->SizeXPercentSpinBox->value() * (9.0 / 16));
	}
}

void Resize::loadState() {
	QSettings settings(QCoreApplication::organizationName(),
			QCoreApplication::applicationName());
	settings.beginGroup("plugin");
	settings.beginGroup(QString(getName()));
	ui->SizePixelRadioButton->setChecked(
			settings.value("PixelUse", false).toBool());
	ui->SizePercentRadioButton->setChecked(
			settings.value("PercentUse", false).toBool());
	ui->SizeXPixelSpinBox->setValue(settings.value("PixelX", 1920).toInt());
	ui->SizeYPixelSpinBox->setValue(settings.value("PixelY", 1080).toInt());
	ui->SizeXPercentSpinBox->setValue(settings.value("PercentX", 100).toInt());
	ui->SizeXPercentSpinBox->setValue(settings.value("PercentY", 100).toInt());
	changeSizePercentUse();
	changeSizePixelUse();
}

void Resize::saveState() {
	QSettings settings(QCoreApplication::organizationName(),
			QCoreApplication::applicationName());
	settings.beginGroup("plugin");
	settings.beginGroup(QString(getName()));
	settings.setValue("PixelUse", ui->SizePixelRadioButton->isChecked());
	settings.setValue("PercentUse", ui->SizePercentRadioButton->isChecked());
	settings.setValue("PixelX", ui->SizeXPixelSpinBox->value());
	settings.setValue("PixelY", ui->SizeYPixelSpinBox->value());
	settings.setValue("PercentX", ui->SizeXPercentSpinBox->value());
	settings.setValue("PercentY", ui->SizeYPercentSpinBox->value());
}

Q_EXPORT_PLUGIN2(resize, Resize)
;
