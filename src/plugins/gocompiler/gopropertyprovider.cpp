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

#include "gopropertyprovider.h"

#include <coreplugin/icore.h>
#include <projectexplorer/kitinformation.h>
#include <projectexplorer/toolchain.h>
//#include <tools/profile.h>

//#include <utils/hostosinfo.h>

//#include <qbs.h>

#include <QtCore/QFileInfo>

using namespace QbsProjectManager;

namespace GoCompiler {
namespace Internal {

bool GoPropertyProvider::canHandle(const ProjectExplorer::Kit *k) const
{
    ProjectExplorer::ToolChain *tc = ProjectExplorer::ToolChainKitInformation::toolChain(k);
    return (tc->type() == QLatin1String("go"));
}

QVariantMap GoPropertyProvider::properties(const ProjectExplorer::Kit *k, const QVariantMap &defaultData) const
{
    QVariantMap data = defaultData;

    ProjectExplorer::ToolChain *tc = ProjectExplorer::ToolChainKitInformation::toolChain(k);
    Utils::FileName gc = tc->compilerCommand();
    const QFileInfo fi = gc.toFileInfo();

    data.insert(QLatin1String("go.compilerPath"), fi.absoluteFilePath());

    /*
    const QString profileName = QString::fromLatin1("qtc_") + k->fileSystemFriendlyName();
    const QString profileBaseDir = QString::fromLocal8Bit("%1/qbs/profiles/%2")
            .arg(Core::ICore::userResourcePath(), profileName);

    data.insert(QLatin1String("preferences.qbsSearchPaths"), profileBaseDir);
    */
    return data;
}

} // namespace Internal
} // namespace GoCompiler
