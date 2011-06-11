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

#ifndef PLUGINLOADER_H
#define PLUGINLOADER_H

#include <QObject>
#include <QList>
#include "easyimagesizer3/easyimagesizer3plugin.h"

/*
 PluginLoader class loads and manages plugins to use in PhotosDD.
 This class is implemented as Singleton.
 */

class PluginLoader: public QObject {
Q_OBJECT
private:
	static PluginLoader* instance;
	QList<EasyImageSizer3Plugin *> pluginList;
	QList<EasyImageSizer3Plugin *> activatedPluginList;
	PluginLoader(const PluginLoader&) {
	}
	PluginLoader(QObject *parent);

public:
	//explicit PluginLoader(QObject *parent = 0){}
	~PluginLoader();

signals:
	void pluginStateChanged();

public slots:

	static PluginLoader& getInstance();
	void addPlugin(EasyImageSizer3Plugin *plugin);
	QList<EasyImageSizer3Plugin *> getPlugins();
	QList<EasyImageSizer3Plugin *> getActivatedPlugins();
	void clearActivated();
	void activatePlugin(QString name);
	void activatePlugins(QStringList list);
	void deactivatePlugin(QString name);
	static void destroy();
	void loadPlugins();
	void unloadPlugins();
	void loadConfig();
	void saveConfig();

};

#endif // PLUGINLOADER_H
