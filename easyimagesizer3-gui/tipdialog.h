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

#ifndef TIPDIALOG_H
#define TIPDIALOG_H

#include <QDialog>

namespace Ui {
    class TipDialog;
}

class TipDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TipDialog(QWidget *parent = 0);
    ~TipDialog();

    void startUp();

private:
    QStringList tips;
    int tipNumber;
    Ui::TipDialog *ui;
    void nextTip();
    void previousTip();
    void setTip();
    void loadTips();
    void loadConfig();
    void saveConfig();

private slots:
    void on_nextButton_clicked();
    void on_previousButton_clicked();
};

#endif // TIPDIALOG_H
