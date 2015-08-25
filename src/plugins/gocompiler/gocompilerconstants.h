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

#ifndef GOCOMPILERCONSTANTS_H
#define GOCOMPILERCONSTANTS_H

namespace GoCompiler {
namespace Constants {

const char GO_TOOLCHAIN_ID[] = "GoCompiler.ToolChain.Go";

// For settings page
const char GOCOMPILER_SETTINGS_CATEGORY[]  = "L.GoCompiler";
const char GOCOMPILER_SETTINGS_CATEGORY_ICON[]  = ":/gocompiler/images/gopher.png";
const char GOCOMPILER_SETTINGS_TR_CATEGORY[] = QT_TRANSLATE_NOOP("GoCompiler", "Go");
const char NODE_MODULES_SETTINGS_ID[] = "Q.GoCompiler.NodeModuleSettings";

// For settings file
const char GOCOMPILER_GROUP[] = "GoCompiler";


} // namespace GoCompiler
} // namespace Constants

#endif // #ifndef GOCOMPILERCONSTANTS_H

