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

#ifndef EASYIMAGESIZER3_GLOBAL_H
#define EASYIMAGESIZER3_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(EASYIMAGESIZER3_LIBRARY)
#  define EASYIMAGESIZER3SHARED_EXPORT Q_DECL_EXPORT
#else
#  define EASYIMAGESIZER3SHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // EASYIMAGESIZER3_GLOBAL_H
