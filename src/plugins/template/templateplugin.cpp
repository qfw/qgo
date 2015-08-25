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

#include "templateplugin.h"
#include "templateconstants.h"

#include <coreplugin/icore.h>

#include <QSettings>
#include <QStringList>

#include <QtDebug>

static const char TEMPLATE_GROUP[] = "Template";

namespace Template {
namespace Internal {

/*****************************************************************************
 * TemplatePluginPrivate
 *****************************************************************************/

class TemplatePluginPrivate
{
public:
    TemplatePluginPrivate() {}

    // Declare private members here
};

/*****************************************************************************
 * TemplatePlugin
 *****************************************************************************/

TemplatePlugin::TemplatePlugin() :
    d(new TemplatePluginPrivate)
{
    // Create your members
}

TemplatePlugin::~TemplatePlugin()
{
    // Unregister objects from the plugin manager's object pool
    // Delete members
    delete d;
}

ExtensionSystem::IPlugin::ShutdownFlag TemplatePlugin::aboutToShutdown()
{
    // Save settings
    // Disconnect from signals that are not needed during shutdown
    // Hide UI (if you add UI that is not in the main window directly)

    writeSettings();
    return SynchronousShutdown;
}

void TemplatePlugin::extensionsInitialized()
{
    // Retrieve objects from the plugin manager's object pool
    // In the extensionsInitialized function, a plugin can be sure that all
    // plugins that depend on it are completely initialized.
}

bool TemplatePlugin::initialize(const QStringList &arguments, QString *errorString)
{
    // Register objects in the plugin manager's object pool
    // Load settings
    // Add actions to menus
    // Connect to other plugins' signals
    // In the initialize function, a plugin can be sure that the plugins it
    // depends on have initialized their members.

    Q_UNUSED(arguments)
    Q_UNUSED(errorString)

    readSettings();

    return true;
}

void TemplatePlugin::readSettings()
{
    QSettings *settings = Core::ICore::settings();
    settings->beginGroup(QLatin1String(TEMPLATE_GROUP));

    // Use settings->value() method in order to restore setting

    settings->endGroup();
}

void TemplatePlugin::writeSettings()
{
    QSettings *settings = Core::ICore::settings();
    settings->beginGroup(QLatin1String(TEMPLATE_GROUP));

    // Use settings->setValue() method in order to save setting

    settings->endGroup();
}

} // namespace Internal
} // namespace Template
