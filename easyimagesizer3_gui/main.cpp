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

#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setApplicationName("EasyImageSizer3");
    QCoreApplication::setApplicationVersion("3.0.7");
    QCoreApplication::setOrganizationName("FalseCAM");
    QCoreApplication::setOrganizationDomain("www.falsecam.net");
    MainWindow w;
    w.show();
    return a.exec();
}
