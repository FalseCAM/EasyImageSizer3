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

#ifndef EASYIMAGESIZER_H
#define EASYIMAGESIZER_H

#include <QObject>
#include <QString>
#include <QWidget>
#include <QCoreApplication>
#include <QIcon>
#include "easyimagesizer3/easyimagesizer3_global.h"
#include "easyimagesizer3/easyimagesizer3plugin.h"

class EASYIMAGESIZER3SHARED_EXPORT EasyImageSizer3: public QObject {
Q_OBJECT
public:
	EasyImageSizer3();
	~EasyImageSizer3();

	// Library Constants
	static QString applicationName() {
		return QString("EasyImageSizer3");
	}
	static QString applicationDomain() {
		return QString("http://easyimagesizer.sourceforge.net/");
	}
	static QString applicationVersion() {
		return QString("3.0.1");
	}
	static QIcon applicationIcon() {
		return QIcon(":/icon/eis");
	}
	static QString organizationName() {
		return QString("FalseCAM");
	}
	static QString organizationDomain() {
		return QString("www.falsecam.net");
	}
	static QString author() {
		return QString("FalseCAM");
	}

	void configPlugins(QWidget* parent);
	QList<EasyImageSizer3Plugin*> getPlugins();
	QStringList convert(QStringList images, QString folder, QString format,
			int quality);

	static void about(QWidget* parent);

private:
	void checkForExifTool();

signals:
	void progress(int);
public slots:
};

#endif // EASYIMAGESIZER_H
