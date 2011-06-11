/* (C) 2011 FalseCAM
    This file is part of EasyImageSizer-GUI.

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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringList>
#include "easyimagesizer3/easyimagesizer3.h"
#include "form.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    EasyImageSizer3 *easyImageSizer;
    Form *form;

    // List to store url of images
    QStringList imagesList;
    // QString to store last opened files dir
    QString lastOpenFileDir;

    void loadPluginWidgets();
    // connects signals to slots
    void initSignalSlots();
    void loadConfig();
    void saveConfig();

protected:
    // Drag n Drop
    void dragEnterEvent( QDragEnterEvent* event );
    void dropEvent( QDropEvent* event );

signals:
    void image_list_Changed(int);

public slots:
    // Add Images
    void addImage();
    void addDir();
    void addRDir();
    void addImages(QStringList files);
    void addDir(QString dirString);
    void addRDir(QString dirString);
    bool fileAlreadyAdded(QString file);
    void imageListChanged(int index);
    void clearList();
    void removeImage();
    void changeShowImages();
    void configPlugins();
    void retranslateUi();

private slots:
    // Tips on startup
    void showTips();
    void on_actionConfig_triggered();
    void on_actionTips_triggered();
    void on_actionAbout_EasyImageSizer_GUI_triggered();
    void on_actionAbout_EasyImageSizer_triggered();
    void on_actionAbout_QT_triggered();

    void on_AddImageButton_clicked();
    void on_AddDirButton_clicked();
    void on_ClearImageListButton_clicked();
    void on_RemoveImageButton_clicked();
    void on_commandLinkButton_clicked();
};

#endif // MAINWINDOW_H
