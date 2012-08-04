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

#ifndef EISIMAGE_H
#define EISIMAGE_H

#include <QObject>
#include "easyimagesizer3/easyimagesizer3_global.h"
#include <QString>
#include <QImage>
#include <exiv2/exiv2.hpp>

class EASYIMAGESIZER3SHARED_EXPORT EisImage: public QObject {
Q_OBJECT
public:
	explicit EisImage(QObject *parent = 0) {
	}
	EisImage();
	EisImage(QString file, int index);
	~EisImage();

	void setImage(QImage* img);
	QImage *getImage();
	void setName(QString name);
	QString getName();
    // Exif
    Exiv2::ExifData *getExifData();
	void setCopyMetaData(bool copy);
    void setExifKey(QString key, QString value);
    void setExifKeyRational(QString key, QString value);
    QString getExifKey(QString key);

	int getIndex();
	QString getOriginalFile();
	QString save(QString folder, QString format, int quality);
	QString save(QString format, int quality);

private:
	QString file;
	QString name;
	QImage *image;
	bool copyMetaData;
    Exiv2::ExifData exifData;
	int index;

    // Exif
	void readMetadata();
	void copyExifData(QString srcFile, QString destFile);
    void saveExifData(QString destFile);

signals:

public slots:

};

#endif // EISIMAGE_H
