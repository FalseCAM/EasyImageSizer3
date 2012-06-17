/*
 EasyImageSizer - Color

 EasyImageSizer plugin to Color an image to a QImage

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

#include "color.h"
#include "ui_color.h"
#include <QtCore>
#include <QFile>
#include <QFileDialog>
#include <QPainter>
#include <QSettings>
#include <QTranslator>

Color::Color() : ui(new Ui::Color) {
    widget = new QWidget();
    ui->setupUi(widget);
    // Signal - Slots
    connect(this->ui->grayscaleCheckBox, SIGNAL(toggled(bool)), this, SLOT(updateImage()));
    connect(this->ui->invertCheckBox, SIGNAL(toggled(bool)), this, SLOT(updateImage()));
    connect(this->ui->sepiaCheckBox, SIGNAL(toggled(bool)), this, SLOT(updateImage()));
    connect(this->ui->swapCheckBox, SIGNAL(toggled(bool)), this, SLOT(updateImage()));
	// loads last state of gui
    loadState();
    updateImage();
}

Color::~Color() {
	saveState();
    widget->close();
	delete ui;
    delete widget;
}

QString Color::getName() {
	return QString("Color");
}

QString Color::getTitle() {
    return QString(tr("Color"));
}

QString Color::getVersion() {
    return QString("0.1");
}

QString Color::getAuthor() {
	return QString("FalseCAM");
}

QString Color::getDescription() {
	return QString(
            tr("Plugin to invert, grayscale or swap colors of an image."));
}

QIcon Color::getIcon() {
	return QIcon("");
}

QWidget * Color::createWidget(){
    return widget;
}

void Color::convert(EisImage *eis_image) {
    QImage * image = eis_image->getImage();

    if(ui->grayscaleCheckBox->isChecked()){
        grayscale(image);
    }
    emit progress(30);
    if(ui->invertCheckBox->isChecked()){
        image->invertPixels();
    }
    emit progress(50);
    if(ui->swapCheckBox->isChecked()){
        *image = image->rgbSwapped();
    }
    emit progress(70);
    if(ui->sepiaCheckBox->isChecked()){
        sepia(image);
    }
	emit progress(100);
}

void Color::updateImage(){
    QImage image(":/plugins/color/eis-large.png");

    // Code from http://docs.huihoo.com/qt/4.7/tools-plugandpaintplugins-basictools.html
    if(ui->grayscaleCheckBox->isChecked()){
        grayscale(&image);
    }
    if(ui->invertCheckBox->isChecked()){
        image.invertPixels();
    }
    if(ui->swapCheckBox->isChecked()){
        image = image.rgbSwapped();
    }
    if(ui->sepiaCheckBox->isChecked()){
        sepia(&image);
    }
    ui->image->setPixmap(QPixmap::fromImage(image));
}

void Color::loadState() {
	QSettings settings(QCoreApplication::organizationName(),
			QCoreApplication::applicationName());
	settings.beginGroup("plugin");
    settings.beginGroup(QString(getName()));
    ui->grayscaleCheckBox->setChecked(settings.value("GrayscaleUse").toBool());
    ui->invertCheckBox->setChecked(settings.value("InvertUse").toBool());
    ui->swapCheckBox->setChecked(settings.value("SepiaUse").toBool());
    ui->swapCheckBox->setChecked(settings.value("SwapUse").toBool());
}

void Color::saveState() {
	QSettings settings(QCoreApplication::organizationName(),
			QCoreApplication::applicationName());
	settings.beginGroup("plugin");
    settings.beginGroup(QString(getName()));
    settings.setValue("GrayscaleUse", ui->grayscaleCheckBox->isChecked());
    settings.setValue("InvertUse", ui->invertCheckBox->isChecked());
    settings.setValue("SepiaUse", ui->swapCheckBox->isChecked());
    settings.setValue("SwapUse", ui->swapCheckBox->isChecked());
}

void Color::grayscale(QImage *image){
    for (int y = 0; y < image->height(); ++y) {
        for (int x = 0; x < image->width(); ++x) {
            int pixel = image->pixel(x, y);
            int gray = qGray(pixel);
            int alpha = qAlpha(pixel);
            image->setPixel(x, y, qRgba(gray, gray, gray, alpha));
        }
    }
}

void Color::sepia(QImage *image){

    //compute the hsl/hsv coordinates of sepia color
    int sepiaH, sepiaS, sepiaL;
    QColor(162,128,101).getHsv( &sepiaH, &sepiaS, &sepiaL );


    //iterate over each selected scanline
    int x, y, pixelLuminance;
    QRgb* rgb;
    QColor sepiaColor;
    uchar* scanLine;

    for( y=0; y<image->height(); y++)
    {
        //iterate over each selected pixel in scanline
        scanLine = image->scanLine(y);
        for( x=0; x<image->width(); x++)
        {
            //compute gray value based on the display luminance of color coordinates
            rgb = ((QRgb*)scanLine+x);
            pixelLuminance = (int) (0.2125*qRed(*rgb) + 0.7154*qGreen(*rgb) + 0.0721*qBlue(*rgb));
            //compute and set sepia color
            sepiaColor.setHsv( sepiaH, sepiaS, pixelLuminance );
            *rgb = sepiaColor.rgb();
        }

    }
}

Q_EXPORT_PLUGIN2(color, Color);
