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

#ifndef FORM_H
#define FORM_H

#include <QWidget>

namespace Ui {
    class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = 0);
    ~Form();
    QString getTitle();
    QString getImageFormat();
    int getImageQuality();
    QString getSaveFolder();
    bool getSaveTo();


private:
    Ui::Form *ui;
    void initSaveFormats();
    // QString to store last opened files dir
    QString lastOpenFileDir;
    void loadConfig();
    void saveConfig();

private slots:
    void setSaveToDir();
    void on_pushButton_clicked();

public slots:
    void retranslateUi();
};

#endif // FORM_H
