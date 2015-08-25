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

#ifndef GOCOMPILERPLUGIN_H
#define GOCOMPILERPLUGIN_H

#include "gocompiler_global.h"

#include <extensionsystem/iplugin.h>

namespace GoCompiler {
namespace Internal {

class GoCompilerPluginPrivate;

class GoCompilerPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "GoCompiler.json")

public:
    GoCompilerPlugin();
    ~GoCompilerPlugin();

    bool initialize(const QStringList &arguments, QString *errorString);
    void extensionsInitialized();
    ShutdownFlag aboutToShutdown();

    void readSettings();
    void writeSettings();

private:
    GoCompilerPluginPrivate *d;
};

} // namespace Internal
} // namespace GoCompiler

#endif // #ifndef GOCOMPILERPLUGIN_H

