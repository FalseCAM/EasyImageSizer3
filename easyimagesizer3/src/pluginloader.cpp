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

#include <QObject>
#include <QDebug>
#include <QDir>
#include <QPluginLoader>
#include <QCoreApplication>
#include <QSettings>
#include <QMessageBox>
#include "easyimagesizer3/pluginloader.h"
#include "easyimagesizer3/easyimagesizer3plugin.h"
#include "easyimagesizer3/easyimagesizer3.h"

PluginLoader* PluginLoader::instance = 0;

PluginLoader::PluginLoader(QObject *parent = 0) {
	loadPlugins();
	loadConfig();
}

PluginLoader::~PluginLoader() {
	saveConfig();
	unloadPlugins();
}

PluginLoader& PluginLoader::getInstance() {
	if (!instance)
		instance = new PluginLoader(0);
	return *instance;
}

void PluginLoader::destroy() {
	if (instance)
		delete instance;
	instance = 0;
}

void PluginLoader::addPlugin(EasyImageSizer3Plugin *plugin) {
	if (plugin) {
		foreach(EasyImageSizer3Plugin * plugin_, pluginList)
			{
				if (plugin_->getName() == plugin->getName())
					return;
			}
		pluginList.append(plugin);
	}
}

void PluginLoader::clearActivated() {
	activatedPluginList.clear();
}

/*
 Activates plugin by name and version.
 Activated plugins are used.
 */
void PluginLoader::activatePlugin(QString name) {
	qDebug("[PluginLoader] activates plugin: %s", qPrintable(name));
	foreach(EasyImageSizer3Plugin * plugin, pluginList)
		{
			if (!activatedPluginList.contains(plugin)) {
				if (QString(plugin->getName() + " " + plugin->getVersion())
						== name || QString(plugin->getName()) == name) {
					activatedPluginList.append(plugin);
					emit(pluginStateChanged());
				}
			}
		}
}

/*
 Deactivates plugin by name and version.
 */
void PluginLoader::deactivatePlugin(QString name) {
	foreach(EasyImageSizer3Plugin * plugin, activatedPluginList)
		{
			if (QString(plugin->getName() + " " + plugin->getVersion()) == name
					|| QString(plugin->getName()) == name) {
				activatedPluginList.removeOne(plugin);
				emit(pluginStateChanged());
			}
		}
}

void PluginLoader::activatePlugins(QStringList list) {
	for (int i = 0; i < list.size(); i++) {
		activatePlugin(list.at(i));
	}
}

/*
 Loads configuration of activated plugins
 */
void PluginLoader::loadConfig() {
	// standardly load file plugin
	activatePlugin("File");
	// load from settings.
	QSettings settings(EasyImageSizer3::organizationName(),
			EasyImageSizer3::applicationName());
	settings.beginGroup(EasyImageSizer3::applicationName());
	settings.beginGroup("plugins");
	qDebug("[PluginLoader] settings path: %s / %s / %s / %s",
			qPrintable(EasyImageSizer3::organizationName()),
			qPrintable(EasyImageSizer3::applicationName()),
			qPrintable(EasyImageSizer3::applicationName()),
			qPrintable(settings.group()));

	QStringList list;
	list = settings.value("ActivatedPlugins", QStringList()).toStringList();
	activatePlugins(list);
	qDebug("[PluginLoader] loaded config for %s",
			qPrintable(EasyImageSizer3::applicationName()));
}

/*
 Saves configuration of activated plugins
 Windows saves to regestry.
 Unix to config file in home folder
 */
void PluginLoader::saveConfig() {
	QSettings settings(EasyImageSizer3::organizationName(),
			EasyImageSizer3::applicationName());
	settings.beginGroup(EasyImageSizer3::applicationName());
	settings.beginGroup("plugins");
	qDebug("[PluginLoader] settings path: %s / %s / %s / %s",
			qPrintable(EasyImageSizer3::organizationName()),
			qPrintable(EasyImageSizer3::applicationName()),
			qPrintable(EasyImageSizer3::applicationName()),
			qPrintable(settings.group()));

	QStringList stringList; //Temp is the QList<int>
	for (int i = 0; i < this->getActivatedPlugins().size(); i++) {
		stringList.append(
				this->getActivatedPlugins().at(i)->getName() + " "
						+ this->getActivatedPlugins().at(i)->getVersion());
	}
	QVariant list;
	list.setValue(stringList);
	settings.setValue("ActivatedPlugins", list);
	qDebug("[PluginLoader] saved config for %s",
			qPrintable(EasyImageSizer3::applicationName()));
}

/*
 Returns all loaded plugins
 */
QList<EasyImageSizer3Plugin *> PluginLoader::getPlugins() {
	return pluginList;
}

/*
 Returns all plugins that are activated by user / config
 */
QList<EasyImageSizer3Plugin *> PluginLoader::getActivatedPlugins() {
	return activatedPluginList;
}

/*
 Reads all files in plugin dir and loads containing plugins.
 */
void PluginLoader::loadPlugins() {
	// Loads Plugins in plugins dir
	QDir pluginsDir(QCoreApplication::applicationDirPath());
	loadPlugins(pluginsDir.currentPath());
#if defined(Q_OS_WIN)
	if (pluginsDir.dirName().toLower() == "debug"
			|| pluginsDir.dirName().toLower() == "release") {
		pluginsDir.cdUp();
		pluginsDir.cdUp();
	}
#elif defined(Q_OS_MAC)
	if (pluginsDir.dirName() == "MacOS") {
		pluginsDir.cdUp();
		pluginsDir.cdUp();
		pluginsDir.cdUp();
	}
#elif defined(Q_OS_LINUX)
	pluginsDir.cdUp();
	pluginsDir.cd("share");
#endif
	loadPlugins(pluginsDir.currentPath());
}

void PluginLoader::loadPlugins(QString dir) {
	QDir pluginsDir(dir);
	if (pluginsDir.exists(dir)) {
		pluginsDir.cd("plugins");
	}

	foreach (QString fileName, pluginsDir.entryList(QDir::Files))
		{
			QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
			QObject *plugin = pluginLoader.instance();
			if (plugin) {
				// loads Plugins
				EasyImageSizer3Plugin *plugin_ = qobject_cast<
						EasyImageSizer3Plugin *> (plugin);
				if (plugin_) {
					addPlugin(plugin_);
				} else {
					delete plugin_;
				}
			}
		}
}

/*
 Removes all plugins from memory.
 */
void PluginLoader::unloadPlugins() {
	qDebug("[PluginLoader] unloads Plugins");
	foreach(EasyImageSizer3Plugin * plugin, pluginList)
		{
			activatedPluginList.removeOne(plugin);
			pluginList.removeOne(plugin);
			delete plugin;
		}
}
