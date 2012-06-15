/*
 EasyImageSizer - Exif

 EasyImageSizer plugin to Overlay an image to a QImage

 Copyright (C) 2010 - 2011  FalseCAM

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

#ifndef EXIF_H
#define EXIF_H

#include <QDateTime>
#include "easyimagesizer3/easyimagesizer3plugin.h"
#include "easyimagesizer3/eisimage.h"

namespace Ui {
class Exif;
}

class Exif: public QObject, public EasyImageSizer3Plugin {
    Q_OBJECT
    Q_INTERFACES(EasyImageSizer3Plugin)

public:
    Exif();
	~Exif();
	QString getName();
	QString getTitle();
	QString getVersion();
	QString getAuthor();
	QString getDescription();
	QIcon getIcon();
    QWidget* createWidget();
    QObject* getObject(){ return this; }
	void convert(EisImage *image);

private slots:
	void loadState();
	void saveState();
	QString exifToString(QString origImage);
	QDateTime getDateTime(QString file);

private:
    Ui::Exif *ui;
    QWidget *widget;
signals:
	void progress(int);
};

#endif // EXIF_H
