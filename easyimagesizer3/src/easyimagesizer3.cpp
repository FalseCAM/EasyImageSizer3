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

#include "easyimagesizer3/easyimagesizer3.h"
#include <QMessageBox>
#include <QTranslator>
#include <QDebug>
#include <QProcess>
#include <QString>
#include "easyimagesizer3/pluginsdialog.h"
#include "easyimagesizer3/pluginloader.h"

EasyImageSizer3::EasyImageSizer3() {
	checkForExifTool();
}

EasyImageSizer3::~EasyImageSizer3() {
	PluginLoader::getInstance().destroy();
}

/*
 About box to show information about PhotosDD library
 */
void EasyImageSizer3::about(QWidget* parent = 0) {
	QMessageBox aboutBox;
	aboutBox.setWindowIcon(EasyImageSizer3::applicationIcon());
	aboutBox.about(
			parent,
			("About ") + EasyImageSizer3::applicationName(),
			("<p align=\"right\"><h1>") + EasyImageSizer3::applicationName()
					+ ("</h1></p>")
					+ ("<p align=\"left\"><font size=\"2\" color=\"grey\">Version ")
					+ EasyImageSizer3::applicationVersion() + ("</font>")
					+ ("<p align=\"left\">(C) 2011 ")
					+ EasyImageSizer3::organizationName() + ("</p>")
					+ ("<p><a href=\"" + EasyImageSizer3::applicationDomain()
							+ "\">") + ("Homepage</a></p>") + ("<br/><br/><p>")
					+ ("English translation: FalseCAM") + ("</p>"));
}

/*
 Shows a dialog to config (activate/deactivate) plugins
 */
void EasyImageSizer3::configPlugins(QWidget* parent = 0) {
	PluginsDialog pluginsDialog(parent);
	pluginsDialog.exec();
}

QList<EasyImageSizer3Plugin*> EasyImageSizer3::getPlugins() {
	return PluginLoader::getInstance().getActivatedPlugins();
}

/*
 Converts given images and returns a list of converted images.
 */
QStringList EasyImageSizer3::convert(QStringList images, QString folder,
		QString format, int quality) {

	for (int i = 0; i < images.size(); i++) {
		qDebug("[EasyImageSizer] converts %s, format: %s",
				qPrintable(images.at(i)), qPrintable(format));
		EisImage *img = new EisImage(images.at(i), i);
		QList<EasyImageSizer3Plugin*> plugins = getPlugins();
		for (int i = 0; i < plugins.count(); i++) {
			plugins.at(i)->convert(img);
		}
		if (folder.isEmpty())
			img->save(format, quality);
		else
			img->save(folder, format, quality);
	}
	return images;
}

// check for exif tool
void EasyImageSizer3::checkForExifTool() {
	QProcess exiftool;
	exiftool.start("exiftool -help");
	if (exiftool.waitForFinished(-1)) {

	} else {
		QMessageBox::warning(
				0,
				"exiftool warning",
				QString(
						"Exiftool command is not installed or in workingdirectory.<br />"
							"Exif data of images wont be copied."));
	}
}
