/*
 EasyImageSizer - Blur

 EasyImageSizer plugin to Blur an image to a QImage

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

#include "blur.h"
#include "ui_blur.h"
#include <QtCore>
#include <QFile>
#include <QFileDialog>
#include <QPainter>
#include <QSettings>
#include <QTranslator>
#include <QRect>
#include <QMimeData>

Blur::Blur() : ui(new Ui::Blur) {
    widget = new QWidget();
    ui->setupUi(widget);
    image = new PreviewImageLabel(widget);
    ui->gridLayout_image->addWidget(image);

    resetPreviewImage();
    // Signal - Slots

    connect(image, SIGNAL(imageDroped(QString)), this, SLOT(setPreviewImage(QString)));
    connect(ui->resetPreviewButton, SIGNAL(clicked()), this, SLOT(resetPreviewImage()));
    connect(this->ui->blurCheckBox, SIGNAL(toggled(bool)), this, SLOT(updateImage()));
    connect(this->ui->invertCheckBox, SIGNAL(toggled(bool)), this, SLOT(updateImage()));
    connect(this->ui->radiusSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateImage()));

    connect(this->ui->posXSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateImage()));
    connect(this->ui->posYSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateImage()));
    connect(this->ui->widthSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateImage()));
    connect(this->ui->heightSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateImage()));
	// loads last state of gui
    loadState();
    updateImage();
}

Blur::~Blur() {
	saveState();
    widget->close();
	delete ui;
    delete widget;
    delete image;
    delete previewImage;
}

QString Blur::getName() {
	return QString("Blur");
}

QString Blur::getTitle() {
    return QString(tr("Blur"));
}

QString Blur::getVersion() {
    return QString("0.1");
}

QString Blur::getAuthor() {
	return QString("FalseCAM");
}

QString Blur::getDescription() {
	return QString(
            tr("Plugin to blur an image."));
}

QIcon Blur::getIcon() {
	return QIcon("");
}

QWidget * Blur::createWidget(){
    return widget;
}

void Blur::convert(EisImage *eis_image) {
    QImage * image = eis_image->getImage();

    emit progress(30);
    if(ui->blurCheckBox->isChecked()){
        blur(image);
    }
    emit progress(100);
}

void Blur::updateImage(){
    QImage image = QImage(*previewImage);

    if(ui->blurCheckBox->isChecked()){
        blur(&image);
    }
    this->image->setPixmap(QPixmap::fromImage(image));
}

void Blur::setPreviewImage(QString img){
    if(QString::compare(img, this->previewImageSrc) != 0){
        this->previewImageSrc = img;
        if(this->previewImage)
            delete this->previewImage;
        this->previewImage = new QImage(QImage(img).scaled(256,256));
        updateImage();
    }
}

void Blur::resetPreviewImage(){
    setPreviewImage(QString(":/plugins/blur/eis-large.png"));
}

void Blur::loadState() {
	QSettings settings(QCoreApplication::organizationName(),
			QCoreApplication::applicationName());
	settings.beginGroup("plugin");
    settings.beginGroup(QString(getName()));
    ui->blurCheckBox->setChecked(settings.value("BlurUse").toBool());
    ui->posXSpinBox->setValue(settings.value("PosX").toInt());
    ui->posYSpinBox->setValue(settings.value("PosY").toInt());
    ui->widthSpinBox->setValue(settings.value("Width").toInt());
    ui->heightSpinBox->setValue(settings.value("Height").toInt());
    ui->radiusSpinBox->setValue(settings.value("Radius", 7).toInt());
}

void Blur::saveState() {
	QSettings settings(QCoreApplication::organizationName(),
			QCoreApplication::applicationName());
	settings.beginGroup("plugin");
    settings.beginGroup(QString(getName()));
    settings.setValue("BlurUse", ui->blurCheckBox->isChecked());
    settings.setValue("PosX", ui->posXSpinBox->value());
    settings.setValue("PosY", ui->posYSpinBox->value());
    settings.setValue("Width", ui->widthSpinBox->value());
    settings.setValue("Height", ui->heightSpinBox->value());
    settings.setValue("Radius", ui->radiusSpinBox->value());
}



// Function blurs a rect of image by percentage
void Blur::blur(QImage *image){
    // uses percentage

    int fullw = image->width();
    int fullh = image->height();
    int x = ui->posXSpinBox->value() * fullw / 100;
    int y = ui->posYSpinBox->value() * fullh / 100;
    int w = ui->widthSpinBox->value() * fullw / 100;
    int h = ui->heightSpinBox->value() * fullh / 100;
    if(x+w > fullw) w = fullw-x;
    if(y+h > fullh) h = fullh-y;

    if(! ui->invertCheckBox->isChecked()){
        *image = blurred(*image, QRect(x, y, w, h), ui->radiusSpinBox->value());
    }else{

        // blur top part
        *image = blurred(*image, QRect(0, 0, fullw, y), ui->radiusSpinBox->value());
        // blur bot part
        *image = blurred(*image, QRect(0, y+h, fullw, fullh-y-h), ui->radiusSpinBox->value());

        // blur left part
        *image = blurred(*image, QRect(0, y, x, h), ui->radiusSpinBox->value());
        // blur right part
        *image = blurred(*image, QRect(x+w, y, fullw -x -w ,h), ui->radiusSpinBox->value());
    }
}


// Code from: http://qt.gitorious.org/qt/qt/blobs/7d5b560f71e0f11c20b7ebef11f3095e760ca32c/src/gui/image/qpixmapfilter.cpp

// Blur the image according to the blur radius

// Based on exponential blur algorithm by Jani Huhtanen

// (maximum radius is set to 16)

QImage Blur::blurred(QImage& image, const QRect& rect, int radius, bool alphaOnly)

{

    int tab[] = { 14, 10, 8, 6, 5, 5, 4, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2 };

    int alpha = (radius < 1)  ? 16 : (radius > 17) ? 1 : tab[radius-1];

    QImage result = image.convertToFormat(QImage::Format_ARGB32_Premultiplied);

    int r1 = rect.top();

    int r2 = rect.bottom();

    int c1 = rect.left();

    int c2 = rect.right();

    int bpl = result.bytesPerLine();

    int rgba[4];

    unsigned char* p;

    int i1 = 0;

    int i2 = 3;

    if (alphaOnly)

        i1 = i2 = (QSysInfo::ByteOrder == QSysInfo::BigEndian ? 0 : 3);

    for (int col = c1; col <= c2; col++) {

        p = result.scanLine(r1) + col * 4;

        for (int i = i1; i <= i2; i++)

            rgba[i] = p[i] << 4;

        p += bpl;

        for (int j = r1; j < r2; j++, p += bpl)

            for (int i = i1; i <= i2; i++)

                p[i] = (rgba[i] += ((p[i] << 4) - rgba[i]) * alpha / 16) >> 4;

    }

    for (int row = r1; row <= r2; row++) {

        p = result.scanLine(row) + c1 * 4;

        for (int i = i1; i <= i2; i++)

            rgba[i] = p[i] << 4;

        p += 4;

        for (int j = c1; j < c2; j++, p += 4)

            for (int i = i1; i <= i2; i++)

                p[i] = (rgba[i] += ((p[i] << 4) - rgba[i]) * alpha / 16) >> 4;

    }

    for (int col = c1; col <= c2; col++) {

        p = result.scanLine(r2) + col * 4;

        for (int i = i1; i <= i2; i++)

            rgba[i] = p[i] << 4;

        p -= bpl;

        for (int j = r1; j < r2; j++, p -= bpl)

            for (int i = i1; i <= i2; i++)

                p[i] = (rgba[i] += ((p[i] << 4) - rgba[i]) * alpha / 16) >> 4;

    }

    for (int row = r1; row <= r2; row++) {

        p = result.scanLine(row) + c2 * 4;

        for (int i = i1; i <= i2; i++)

            rgba[i] = p[i] << 4;

        p -= 4;

        for (int j = c1; j < c2; j++, p -= 4)

            for (int i = i1; i <= i2; i++)

                p[i] = (rgba[i] += ((p[i] << 4) - rgba[i]) * alpha / 16) >> 4;

    }

    return result;

}

Q_EXPORT_PLUGIN2(blur, Blur);
