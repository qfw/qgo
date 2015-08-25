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

#ifndef TEMPLATEPLUGIN_H
#define TEMPLATEPLUGIN_H

#include "template_global.h"

#include <extensionsystem/iplugin.h>

namespace Template {
namespace Internal {

class TemplatePluginPrivate;

class TemplatePlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "Template.json")

public:
    TemplatePlugin();
    ~TemplatePlugin();

    bool initialize(const QStringList &arguments, QString *errorString);
    void extensionsInitialized();
    ShutdownFlag aboutToShutdown();

    void readSettings();
    void writeSettings();

private:
    TemplatePluginPrivate *d;
};

} // namespace Internal
} // namespace Template

#endif // #ifndef TEMPLATEPLUGIN_H

