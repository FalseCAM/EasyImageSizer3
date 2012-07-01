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

#ifndef BLUR_H
#define BLUR_H

#include <QWidget>
#include "easyimagesizer3/easyimagesizer3plugin.h"
#include "easyimagesizer3/eisimage.h"
#include "previewimagelabel.h"

namespace Ui {
class Blur;
}

class Blur: public QObject, public EasyImageSizer3Plugin {
Q_OBJECT
Q_INTERFACES( EasyImageSizer3Plugin)

public:
    Blur();
	~Blur();
	QString getName();
	QString getTitle();
	QString getVersion();
	QString getAuthor();
	QString getDescription();
	QIcon getIcon();
    QWidget* createWidget();
    QObject* getObject(){ return this; }
    void convert(EisImage *eis_image);

private slots:
	void loadState();
	void saveState();
    void updateImage();
    void setPreviewImage(QString img);
    void resetPreviewImage();
private:
    PreviewImageLabel *image;

    QString previewImageSrc;
    QImage *previewImage;

    void blur(QImage* image);
    static QImage blurred(QImage& image, const QRect& rect, int radius, bool alphaOnly = false);
    Ui::Blur *ui;
    QWidget *widget;

signals:
	void progress(int);
};

#endif // BLUR_H
