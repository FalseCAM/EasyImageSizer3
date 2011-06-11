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

#ifndef PLUGINSDIALOG_H
#define PLUGINSDIALOG_H

#include <QDialog>
#include <QListWidgetItem>

namespace Ui {
class PluginsDialog;
}

class PluginsDialog: public QDialog {
Q_OBJECT

public:
	explicit PluginsDialog(QWidget *parent = 0);
	~PluginsDialog();
	void loadPlugins();
private:

	Ui::PluginsDialog *ui;
private slots:
	void activatePlugin(QString name);
	void itemChanged(QListWidgetItem*);
	void moveUp();
	void moveDown();
	void setActivated();
};

#endif // PLUGINSDIALOG_H
