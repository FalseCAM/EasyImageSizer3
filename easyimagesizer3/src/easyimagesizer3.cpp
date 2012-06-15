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
#include <QFutureWatcher>
#include <QFutureSynchronizer>
#include <QtConcurrentRun>
#include <QtConcurrentMap>
#include <QString>
#include "easyimagesizer3/pluginsdialog.h"
#include "easyimagesizer3/pluginloader.h"

EasyImageSizer3::EasyImageSizer3() {
    progressPos = 0;
    progressMax = 0;
}

EasyImageSizer3::~EasyImageSizer3() {
	PluginLoader::getInstance().destroy();
}

/*
 About box to show information about EasyImageSizer library
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
                    + ("<p align=\"left\">(C) 2012 ")
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

void EasyImageSizer3::progressChanged(int state){
    qDebug("[EasyImageSizer3] progress: %s", qPrintable(QString().number(state)));
    emit progress(state);
}

void EasyImageSizer3::progressChanged(){
    progressPos ++;
    progressChanged(progressPos*100 / progressMax);
}

/*
 Converts given images and returns a list of converted images.
 */
QStringList EasyImageSizer3::convert(QStringList images, QString folder,
		QString format, int quality, bool copyMetaData) {
    this->images = images;
    this->folder = folder;
    this->format = format;
    this->quality = quality;
    this->copyMetaData = copyMetaData;
    progressPos = 0;
    progressMax = images.size();
    return images;
}

void EasyImageSizer3::run(){
#ifndef QT_NO_CONCURRENT
    QFutureSynchronizer<void> synchronizer;
#endif
    for (int i = 0; i < images.size(); i++) {

        // Parallel convertion with some threads
#ifndef QT_NO_CONCURRENT
        QFuture<void> future;
        if(copyMetaData){
            future = QtConcurrent::run(convertMeta, images, i, folder, format, quality);
        }else{
            future = QtConcurrent::run(convertNoMeta, images, i, folder, format, quality);
        }
        synchronizer.addFuture(future);
        if(synchronizer.futures().size() > QThread::idealThreadCount()*3){
            synchronizer.waitForFinished();
            synchronizer.clearFutures();
        }
#else
        convert(images, i , folder, format, quality, copyMetaData);
#endif
        progressChanged();
	}
#ifndef QT_NO_CONCURRENT
    synchronizer.waitForFinished();
#endif
}

void EasyImageSizer3::convert(QStringList images, int index, QString folder, QString format, int quality, bool copyMetaData){
    qDebug("[EasyImageSizer] converts %s, format: %s",
            qPrintable(images.at(index)), qPrintable(format));
    EisImage *img = new EisImage(images.at(index), index);
    QList<EasyImageSizer3Plugin*> plugins = EasyImageSizer3::getPlugins();
    for (int i = 0; i < plugins.count(); i++) {
        qDebug("[EasyImageSizer] converts plugin %s", qPrintable(plugins.at(i)->getName()));
        plugins.at(i)->convert(img);
    }
    img->setCopyMetaData(copyMetaData);
    if (folder.isEmpty())
        img->save(format, quality);
    else
        img->save(folder, format, quality);
    delete img;
}

void EasyImageSizer3::convertMeta(QStringList images, int index, QString folder, QString format, int quality){
    convert(images, index, folder, format, quality, true);
}

void EasyImageSizer3::convertNoMeta(QStringList images, int index, QString folder, QString format, int quality){
    convert(images, index, folder, format, quality, false);
}
