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

#ifndef EISPLUGIN_H
#define EISPLUGIN_H

#include <QWidget>
#include <QIcon>
#include <QString>
#include "easyimagesizer3/eisimage.h"

class EasyImageSizer3Plugin{
public:
	virtual ~EasyImageSizer3Plugin() {
	}
	virtual QString getName() = 0;
	virtual QString getTitle() = 0;
	virtual QString getVersion() = 0;
	virtual QString getAuthor() = 0;
	virtual QString getDescription() = 0;
	virtual QIcon getIcon() = 0;
    virtual QWidget* createWidget() = 0;
    virtual QObject* getObject() = 0;
    virtual void convert(EisImage *image) = 0;
};

Q_DECLARE_INTERFACE(EasyImageSizer3Plugin,
        "EasyImageSizer3.EasyImageSizer3Plugin/0.1")
;

#endif // PLUGIN_H
