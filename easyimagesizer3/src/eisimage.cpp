/* (C) 2011 FalseCAM
 This file is part of EasyImageSizer.

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

#include "easyimagesizer3/eisimage.h"
#include <QFileInfo>
#include <QDir>
#include <QDebug>
#include <QProcess>

EisImage::EisImage() {
    this->copyMetaData = true;
}

EisImage::EisImage(QString file, int index = 0) {
    this->file = file;
    this->name = QFileInfo(file).baseName();
    this->image = new QImage(file);
    this->index = index;
    readMetadata();
}

EisImage::~EisImage() {
    if (this->image != 0)
        delete this->image;
}

void EisImage::readMetadata() {
    // reads exif data from file, if no exists it creates new empty one
    try{
        Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(
                    this->file.toStdString().c_str());
        image->readMetadata();

        this->exifData = image->exifData();
    } catch (Exiv2::Error& e) {
        qCritical("[EisImage] %s", e.what());
    }
}

Exiv2::ExifData* EisImage::getExifData(){
    return &this->exifData;
}

void EisImage::setImage(QImage *img) {
    if (this->image != img){
        if (this->image != 0)
            delete this->image;
        this->image = img;
    }
}

QImage* EisImage::getImage() {
    return this->image;
}

void EisImage::setName(QString name) {
    this->name = name;
}

QString EisImage::getName() {
    return name;
}

void EisImage::setCopyMetaData(bool copy) {
    this->copyMetaData = copy;
}

int EisImage::getIndex() {
    return index;
}

QString EisImage::getOriginalFile() {
    return this->file;
}

/*
 Saves image to given folder in given format and quality.
 */
QString EisImage::save(QString folder, QString format, int quality) {
    QString filename = folder + "/" + name + "." + format;
    qDebug("[EisImage] Filename is %s, Format is %s", qPrintable(filename),
           qPrintable(format));
    if (!this->image->save(filename, qPrintable(format), quality)) {
        qDebug("[EisImage] %s could not been saved.", qPrintable(filename));
    } else {
        if (this->copyMetaData)
            saveExifData(filename);
    }
    return filename;
}

/*
 Saves image to currents folder subfolder "eis" in given format and quality.
 */
QString EisImage::save(QString format, int quality) {
    QString folder = QFileInfo(file).absolutePath() + "/eis";
    if (!QDir(folder).exists())
        QDir().mkdir(QDir(folder).path());
    return save(folder, format, quality);
}

// Copy exif Data from one file to another
void EisImage::copyExifData(QString srcFile, QString destFile) {

    try {
        Exiv2::Image::AutoPtr srcImage = Exiv2::ImageFactory::open(
                    srcFile.toStdString().c_str());
        srcImage->readMetadata();

        Exiv2::ExifData& exifData = srcImage->exifData();
        Exiv2::Image::AutoPtr destImage = Exiv2::ImageFactory::open(
                    destFile.toStdString().c_str());
        destImage->setExifData(exifData);
        destImage->writeMetadata();
    } catch (Exiv2::Error& e) {
        qCritical("[EisImage] %s", e.what());
    }
}

void EisImage::saveExifData(QString destFile){
    try {
        Exiv2::Image::AutoPtr destImage = Exiv2::ImageFactory::open(
                    destFile.toStdString().c_str());
        destImage->setExifData(this->exifData);
        destImage->writeMetadata();
    } catch (Exiv2::Error& e) {
        qCritical("[EisImage] %s", e.what());
    }
}

void EisImage::setExifKey(QString key, QString value){
    try {
        // Create a ASCII string value (note the use of create)
        Exiv2::Value::AutoPtr v = Exiv2::Value::create(Exiv2::asciiString);
        // Set the value to a string
        v->read(qPrintable(value));
        // Add the value together with its key to the Exif data container
        Exiv2::ExifKey key_(qPrintable(key));
        exifData.add(key_, v.get());
    } catch (Exiv2::Error& e) {
        qCritical("[EisImage] %s", e.what());
    }
}

void EisImage::setExifKeyRational(QString key, QString value){
    try {
        // Create a ASCII string value (note the use of create)
        Exiv2::URationalValue::AutoPtr v(new Exiv2::URationalValue);
        // Set the value to a string
        v->read(qPrintable(value));
        // Add the value together with its key to the Exif data container
        Exiv2::ExifKey key_(qPrintable(key));
        exifData.add(key_, v.get());
    } catch (Exiv2::Error& e) {
        qCritical("[EisImage] %s", e.what());
    }
}

QString EisImage::getExifKey(QString key){
    // returns string represented by exif key
    QString ret;
    try {
        Exiv2::ExifData::iterator pos = exifData.findKey(
                    Exiv2::ExifKey(qPrintable(key)));
        if (pos == exifData.end()) return QString();
        else ret = QString().fromStdString(pos->value().toString());
    } catch (Exiv2::Error& e) {
        qCritical("[EisImage] %s", e.what());
    }
    return ret;
}
