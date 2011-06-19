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

#ifndef OVERLAY_TEXT_H
#define OVERLAY_TEXT_H

#include "easyimagesizer3/easyimagesizer3plugin.h"
#include "easyimagesizer3/eisimage.h"
#include <QFont>
#include <QColor>
#include <QPixmap>
#include <QGraphicsScene>

namespace Ui {
class OverlayText;
}

class OverlayText: public EasyImageSizer3Plugin {
Q_OBJECT
Q_INTERFACES(EasyImageSizer3Plugin)

public:
	OverlayText(QWidget *parent = 0);
	~OverlayText();

	QFont *font;
	QColor *color;
	QPixmap *pixmap;
	QPixmap *scenePixmap;
	QGraphicsScene *scene;

	QString getName();
	QString getTitle();
	QString getVersion();
	QString getAuthor();
	QString getDescription();
	QIcon getIcon();
	void convert(EisImage *image);

private slots:
	void addOverlay(QImage *image, QString overlay);
	void loadState();
	void saveState();
	void setFont();
	void setColor();
	void makePreview();

private:
	Ui::OverlayText *ui;signals:
	void progress(int);
};

#endif // OVERLAY_TEXT_H
