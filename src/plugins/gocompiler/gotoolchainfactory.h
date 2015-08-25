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

#ifndef GOTOOLCHAINFACTORY_H
#define GOTOOLCHAINFACTORY_H

#include <projectexplorer/toolchain.h>

namespace GoCompiler {
namespace Internal {

class GoToolChainFactory : public ProjectExplorer::ToolChainFactory
{
public:
    GoToolChainFactory();

    QList<ProjectExplorer::ToolChain *> autoDetect();

    bool canCreate();
    ProjectExplorer::ToolChain *create();

    bool canRestore(const QVariantMap &data);
    ProjectExplorer::ToolChain *restore(const QVariantMap &data);

};

} // namespace Internal
} // namespace GoCompiler

#endif // #ifndef GOTOOLCHAINFACTORY_H
