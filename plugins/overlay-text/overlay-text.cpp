/*
 EasyImageSizer - OverlayText

 EasyImageSizer plugin to Overlay a Text over a QImage

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

#include "overlay-text.h"
#include "ui_overlay-text.h"

#include <QtCore>
#include <QFontDialog>
#include <QColorDialog>
#include <QSettings>

OverlayText::OverlayText(QWidget *parent) :
	QWidget(parent), ui(new Ui::OverlayText) {
	font = new QFont();
	color = new QColor();
	pixmap = new QPixmap(32, 32);
	scene = new QGraphicsScene();
	scenePixmap = new QPixmap(100, 100);
	ui->setupUi(this);
	ui->preview->setScene(scene);

	connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(setFont()));
	connect(ui->colorPushButton, SIGNAL(clicked()), this, SLOT(setColor()));

	connect(ui->Text, SIGNAL(editingFinished()), this, SLOT(makePreview()));
	connect(ui->xSpinBox, SIGNAL(valueChanged(int)), this, SLOT(makePreview()));
	connect(ui->ySpinBox, SIGNAL(valueChanged(int)), this, SLOT(makePreview()));

	// loads last state of gui
	loadState();
	ui->fontComboBox->setCurrentFont(*font);
	pixmap->fill(*color);
	ui->colorPushButton->setIcon(QIcon(*pixmap));
	makePreview();

}

OverlayText::~OverlayText() {
	saveState();
	delete ui;
}

QString OverlayText::getName() {
	return QString("OverlayText");
}

QString OverlayText::getTitle() {
	return QString(tr("OverlayText"));
}

QString OverlayText::getVersion() {
	return QString("0.3");
}

QString OverlayText::getAuthor() {
	return QString("FalseCAM");
}

QString OverlayText::getDescription() {
	return QString(tr("Plugin to overlay a Text (copyright) over an image"));
}

QIcon OverlayText::getIcon() {
	return QIcon("");
}

void OverlayText::convert(EisImage *image) {
	if (ui->overlayCheckBox->isChecked()) {
		emit progress(50);
		addOverlay(image->getImage(), ui->Text->text());
	}
	emit progress(100);
}

// add Overlay
void OverlayText::addOverlay(QImage *image, QString overlay) {
	if (overlay.isEmpty())
		return;
	QPainter painter(image);
	painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
	int x = (int) ((double) image->width() * ((double) ui->xSpinBox->value()
			/ 100.0));
	int y = (int) ((double) image->height() * ((double) ui->ySpinBox->value()
			/ 100.0));
	painter.setPen(*color);
	QFont tempFont = *font;
	tempFont.setPointSize((int) (tempFont.pointSize() * image->width() / 100));
	painter.setFont(tempFont);
	painter.drawText(x, y, overlay);
	painter.end();
}
// shows a dialog to select font of text
void OverlayText::setFont() {
	bool ok;
	QFont temp = QFontDialog::getFont(&ok, *font, this);
	if (ok) {
		delete font;
		this->font = new QFont(temp);
		ui->fontComboBox->setCurrentFont(temp);
	}
	makePreview();
}

// shows a dialog to select color of text
void OverlayText::setColor() {
	QColor temp = QColorDialog::getColor(*color, this);

	delete color;
	color = new QColor(temp);
	pixmap->fill(*color);
	ui->colorPushButton->setIcon(QIcon(*pixmap));
	makePreview();
}

// makes a preview of text with position and size
void OverlayText::makePreview() {
	QPainter painter(scenePixmap);
	painter.fillRect(0, 0, scenePixmap->width(), scenePixmap->height(),
			Qt::gray);
	painter.beginNativePainting();
	int x = (int) ((double) scenePixmap->width()
			* ((double) ui->xSpinBox->value() / 100.0));
	int y = (int) ((double) scenePixmap->width()
			* ((double) ui->ySpinBox->value() / 100.0));
	painter.setPen(*color);
	painter.setFont(*font);
	painter.drawText(x, y, ui->Text->text());
	painter.endNativePainting();
	scene->clear();
	scene->addPixmap(scenePixmap->scaled(100, 100));
}

void OverlayText::loadState() {
	QSettings settings(QCoreApplication::organizationName(),
			QCoreApplication::applicationName());
	settings.beginGroup("plugin");
	settings.beginGroup(QString(getName()));
	ui->overlayCheckBox->setChecked(settings.value("OverlayTextUse").toBool());
	ui->Text->setText(settings.value("OverlayText").toString());
	ui->xSpinBox->setValue(settings.value("OverlayX").toInt());
	ui->ySpinBox->setValue(settings.value("OverlayY").toInt());
	font->setPointSize(settings.value("OverlayFontSize").toInt());
	font->setFamily(settings.value("OverlayFontFamily").toString());
	color->setRed(settings.value("OverlayColorR").toInt());
	color->setGreen(settings.value("OverlayG").toInt());
	color->setBlue(settings.value("OverlayColorB").toInt());
}

void OverlayText::saveState() {
	QSettings settings(QCoreApplication::organizationName(),
			QCoreApplication::applicationName());
	settings.beginGroup("plugin");
	settings.beginGroup(QString(getName()));
	settings.setValue("OverlayTextUse", ui->overlayCheckBox->isChecked());
	settings.setValue("OverlayText", ui->Text->text());
	settings.setValue("OverlayX", ui->xSpinBox->value());
	settings.setValue("OverlayY", ui->ySpinBox->value());
	settings.setValue("OverlayFontSize", font->pointSize());
	settings.setValue("OverlayFontFamily", font->family());
	int r, g, b;
	color->getRgb(&r, &g, &b);
	settings.setValue("OverlayColorR", r);
	settings.setValue("OverlayColorG", g);
	settings.setValue("OverlayColorB", b);
}

Q_EXPORT_PLUGIN2(overlay_text, OverlayText)
;
