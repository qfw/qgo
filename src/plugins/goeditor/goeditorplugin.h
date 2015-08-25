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

#ifndef GOEDITORPLUGIN_H
#define GOEDITORPLUGIN_H

#include "goeditor_global.h"

#include <extensionsystem/iplugin.h>

namespace GoEditor {
namespace Internal {

class GoEditorPluginPrivate;

class GoEditorPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "GoEditor.json")

public:
    GoEditorPlugin();
    ~GoEditorPlugin();

    bool initialize(const QStringList &arguments, QString *errorMessage);
    void extensionsInitialized();
    ShutdownFlag aboutToShutdown();

    void readSettings();
    void writeSettings();

private:
    GoEditorPluginPrivate *d;
};

} // namespace Internal
} // namespace GoEditor

#endif // #ifndef GOEDITORPLUGIN_H

