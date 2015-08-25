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

#include "gocompilerplugin.h"
#include "gocompilerconstants.h"
#include "gopropertyprovider.h"
#include "gotoolchainfactory.h"
#include "nodemodulessettingspage.h"

#include <coreplugin/icore.h>
#include <projectexplorer/kitmanager.h>

#include <QSettings>
#include <QStringList>

#include <QtDebug>

namespace GoCompiler {
namespace Internal {

/*****************************************************************************
 * GoCompilerPluginPrivate
 *****************************************************************************/

class GoCompilerPluginPrivate
{
public:
    GoCompilerPluginPrivate() {}

    // Declare private members here
};

/*****************************************************************************
 * GoCompilerPlugin
 *****************************************************************************/

GoCompilerPlugin::GoCompilerPlugin() :
    d(new GoCompilerPluginPrivate)
{
}

GoCompilerPlugin::~GoCompilerPlugin()
{
    delete d;
}

ExtensionSystem::IPlugin::ShutdownFlag GoCompilerPlugin::aboutToShutdown()
{
    writeSettings();
    return SynchronousShutdown;
}

void GoCompilerPlugin::extensionsInitialized()
{
    /*connect(ProjectExplorer::KitManager::instance(), &ProjectExplorer::KitManager::kitsLoaded, this,
            [this]() { m_kitsToBeSetupForQbs = ProjectExplorer::KitManager::kits(); } );
    connect(ProjectExplorer::KitManager::instance(), &ProjectExplorer::KitManager::kitAdded, this,
            &QbsManager::addProfileFromKit);
    connect(ProjectExplorer::KitManager::instance(), &ProjectExplorer::KitManager::kitUpdated, this,
            &QbsManager::handleKitUpdate);
    connect(ProjectExplorer::KitManager::instance(), &ProjectExplorer::KitManager::kitRemoved, this,
            &QbsManager::handleKitRemoval);*/
}

bool GoCompilerPlugin::initialize(const QStringList & /*arguments*/, QString * /*errorString*/)
{
    addAutoReleasedObject(new GoToolChainFactory);
    addAutoReleasedObject(new GoPropertyProvider);
    addAutoReleasedObject(new NodeModulesSettingsPage);

    readSettings();

    return true;
}

void GoCompilerPlugin::readSettings()
{
    QSettings *settings = Core::ICore::settings();
    settings->beginGroup(QLatin1String(Constants::GOCOMPILER_GROUP));

    // Use settings->value() method in order to restore setting

    settings->endGroup();
}

void GoCompilerPlugin::writeSettings()
{
    QSettings *settings = Core::ICore::settings();
    settings->beginGroup(QLatin1String(Constants::GOCOMPILER_GROUP));

    // Use settings->setValue() method in order to save setting

    settings->endGroup();
}

} // namespace Internal
} // namespace GoCompiler
