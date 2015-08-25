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

#ifndef GOTOOLCHAINCONFIGWIDGET_H
#define GOTOOLCHAINCONFIGWIDGET_H

#include <projectexplorer/toolchainconfigwidget.h>

namespace ProjectExplorer { class AbiWidget; }
namespace Utils { class PathChooser; }

namespace GoCompiler {
namespace Internal {

class GoToolChainConfigWidget : public ProjectExplorer::ToolChainConfigWidget
{
public:
    GoToolChainConfigWidget(ProjectExplorer::ToolChain *tc);

protected:
    void applyImpl();
    void discardImpl();
    bool isDirtyImpl() const;
    void makeReadOnlyImpl();

private Q_SLOTS:
    void handleCompilerCommandChange();

private:
    void setFromToolchain();

private:
    Utils::PathChooser *m_compilerCommand;
    ProjectExplorer::AbiWidget *m_abiWidget;

    bool m_isReadOnly;
};

} // namespace Internal
} // namespace GoCompiler

#endif // #ifndef GOTOOLCHAINCONFIGWIDGET_H
