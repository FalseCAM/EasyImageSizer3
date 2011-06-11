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

#include "easyimagesizer3/pluginsdialog.h"
#include "../ui_pluginsdialog.h"
#include <QCheckBox>
#include "easyimagesizer3/pluginloader.h"
#include "easyimagesizer3/easyimagesizer3.h"

PluginsDialog::PluginsDialog(QWidget *parent) :
	QDialog(parent), ui(new Ui::PluginsDialog) {
	ui->setupUi(this);
	loadPlugins();
	connect(ui->listWidget, SIGNAL(itemChanged(QListWidgetItem*)), this,
			SLOT(itemChanged(QListWidgetItem*)));
	connect(ui->upButton, SIGNAL(clicked()), this, SLOT(moveUp()));
	connect(ui->downButton, SIGNAL(clicked()), this, SLOT(moveDown()));

	ui->upButton->setIcon(this->style()->standardIcon(QStyle::SP_ArrowUp));
	ui->downButton->setIcon(this->style()->standardIcon(QStyle::SP_ArrowDown));
}

PluginsDialog::~PluginsDialog() {
	delete ui;
}

/*
 Loads available Plugins
 */
void PluginsDialog::loadPlugins() {
	ui->listWidget->clear();
	foreach(EasyImageSizer3Plugin *plugin, PluginLoader::getInstance().getPlugins())
		{
			if (plugin) {
				QListWidgetItem *listWidgetItem = new QListWidgetItem(
						plugin->getName() + " " + plugin->getVersion(),
						ui->listWidget);
				listWidgetItem->setToolTip(
						plugin->getTitle() + " Version " + plugin->getVersion()
								+ " by " + plugin->getAuthor() + "<br />"
								+ plugin->getDescription());
				listWidgetItem->setIcon(plugin->getIcon());
				listWidgetItem->setCheckState(Qt::Unchecked);

			}
		}
	foreach(EasyImageSizer3Plugin *plugin, PluginLoader::getInstance().getActivatedPlugins())
		{
			if (plugin) {
				activatePlugin(plugin->getName());
			}
		}
}
/*
 Sets PluginItem width name of plugin checked in list widget.
 */
void PluginsDialog::activatePlugin(QString name) {
	qDebug("PluginsDialog: activating plugin %s", qPrintable(name));
	for (int i = 0; i < PluginLoader::getInstance().getPlugins().size(); i++) {
		if (PluginLoader::getInstance().getPlugins().at(i)->getName() == name) {
			ui->listWidget->item(i)->setCheckState(Qt::Checked);
		}
	}
}

void PluginsDialog::itemChanged(QListWidgetItem* item) {
	if (item->checkState() == Qt::Checked) {
		PluginLoader::getInstance().activatePlugin(item->text());
	} else {
		PluginLoader::getInstance().deactivatePlugin(item->text());
	}
}

void PluginsDialog::moveUp() {
	if (ui->listWidget->currentRow() > 0) {
		QListWidgetItem *listWidgetItem = ui->listWidget->takeItem(
				ui->listWidget->currentRow());
		ui->listWidget->insertItem(ui->listWidget->currentRow() - 1,
				listWidgetItem);
		ui->listWidget->setCurrentRow(ui->listWidget->currentRow() - 2);
		setActivated();
	}
}

void PluginsDialog::moveDown() {
	if (ui->listWidget->currentRow() < ui->listWidget->count()) {
		QListWidgetItem *listWidgetItem = ui->listWidget->takeItem(
				ui->listWidget->currentRow());
		ui->listWidget->insertItem(ui->listWidget->currentRow() + 1,
				listWidgetItem);
		ui->listWidget->setCurrentRow(ui->listWidget->currentRow() + 1);
		setActivated();
	}
}

/*
 renews the activated_plugins_list.
 */
void PluginsDialog::setActivated() {
	PluginLoader::getInstance().clearActivated();
	for (int i = 0; i < ui->listWidget->count(); i++) {
		itemChanged(ui->listWidget->item(i));
	}
}

