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

#ifndef GOPROPERTYPROVIDER_H
#define GOPROPERTYPROVIDER_H

#include <qbsprojectmanager/propertyprovider.h>

namespace GoCompiler {
namespace Internal {

class GoPropertyProvider : public QbsProjectManager::PropertyProvider
{
    Q_OBJECT
public:
    bool canHandle(const ProjectExplorer::Kit *k) const Q_DECL_OVERRIDE;
    QVariantMap properties(const ProjectExplorer::Kit *k, const QVariantMap &defaultData) const Q_DECL_OVERRIDE;
};

} // namespace Internal
} // namespace GoCompiler

#endif // #ifndef GOPROPERTYPROVIDER_H
