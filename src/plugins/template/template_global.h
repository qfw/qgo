/****************************************************************************
**
** Copyright (C) 2014 Noviga.
** Contact: http://noviga.com
**
** This file is part of QGo.
**
** QGo is free software: you can redistribute it and/or modify it
** under the terms of the GNU General Public License version 3 as published
** by the Free Software Foundation, and appearing in the file GPL3.txt
** included in the packaging of this file.
**
** QGo is distributed in the hope that it will be useful, but
** WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
** Public License for more details.
**
****************************************************************************/

#ifndef TEMPLATE_GLOBAL_H
#define TEMPLATE_GLOBAL_H

#include <QtGlobal>

#if defined(TEMPLATE_LIBRARY)
#  define TEMPLATE_EXPORT Q_DECL_EXPORT
#else
#  define TEMPLATE_EXPORT Q_DECL_IMPORT
#endif

#endif // #ifndef TEMPLATE_GLOBAL_H

