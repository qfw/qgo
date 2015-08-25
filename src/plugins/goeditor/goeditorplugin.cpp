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

#include "goeditorplugin.h"
#include "gocompletionassistprovider.h"
#include "goeditor.h"
#include "goeditorconstants.h"
#include "gohighlighter.h"
#include "golexer.h"

#include <coreplugin/icore.h>

#include <utils/mimetypes/mimedatabase.h>

#include <QSettings>
#include <QStringList>

#include <QtDebug>

static const char GOEDITOR_GROUP[] = "GoEditor";

namespace GoEditor {
namespace Internal {

/*****************************************************************************
 * GoEditorPluginPrivate
 *****************************************************************************/

class GoEditorPluginPrivate
{
public:
    GoEditorPluginPrivate() :
        lexer(0)
    {}

    GoLexer *lexer;
};

/*****************************************************************************
 * GoEditorPlugin
 *****************************************************************************/

GoEditorPlugin::GoEditorPlugin() :
    d(new GoEditorPluginPrivate)
{
    // Create your members
}

GoEditorPlugin::~GoEditorPlugin()
{
    delete d->lexer;
    delete d;
}

ExtensionSystem::IPlugin::ShutdownFlag GoEditorPlugin::aboutToShutdown()
{
    writeSettings();
    return SynchronousShutdown;
}

void GoEditorPlugin::extensionsInitialized()
{
    // Retrieve objects from the plugin manager's object pool
    // In the extensionsInitialized function, a plugin can be sure that all
    // plugins that depend on it are completely initialized.
}

bool GoEditorPlugin::initialize(const QStringList & /*arguments*/, QString *errorMessage)
{
    Q_UNUSED(errorMessage)
    d->lexer = new GoLexer;

    Utils::MimeDatabase::addMimeTypes(QLatin1String(Constants::MIME_XML));

    addAutoReleasedObject(new GoEditorFactory);
    addAutoReleasedObject(new GoCompletionAssistProvider);

    readSettings();

    return true;
}

void GoEditorPlugin::readSettings()
{
    QSettings *settings = Core::ICore::settings();
    settings->beginGroup(QLatin1String(GOEDITOR_GROUP));

    // Use settings->value() method in order to restore setting

    settings->endGroup();
}

void GoEditorPlugin::writeSettings()
{
    QSettings *settings = Core::ICore::settings();
    settings->beginGroup(QLatin1String(GOEDITOR_GROUP));

    // Use settings->setValue() method in order to save setting

    settings->endGroup();
}

} // namespace Internal
} // namespace GoEditor
