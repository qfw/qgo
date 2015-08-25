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

#include "gotoolchainconfigwidget.h"
#include "gotoolchain.h"

#include <projectexplorer/abiwidget.h>
#include <projectexplorer/gcctoolchain.h>
#include <utils/environment.h>
#include <utils/pathchooser.h>

#include <QtCore/QFileInfo>

#include <QtWidgets/QFormLayout>


using namespace ProjectExplorer;
using namespace Utils;

namespace GoCompiler {
namespace Internal {

GoToolChainConfigWidget::GoToolChainConfigWidget(ToolChain *tc) :
    ToolChainConfigWidget(tc),
    m_compilerCommand(new PathChooser),
    m_abiWidget(new AbiWidget),
    m_isReadOnly(false)
{
    m_compilerCommand->setExpectedKind(PathChooser::ExistingCommand);
    const QStringList goVersionArgs = QStringList(QLatin1String("version"));
    m_compilerCommand->setCommandVersionArguments(goVersionArgs);

    m_mainLayout->addRow(tr("&Compiler path:"), m_compilerCommand);
    m_mainLayout->addRow(tr("&ABI:"), m_abiWidget);
    m_abiWidget->setEnabled(false);

    setFromToolchain();

    connect(m_compilerCommand, &PathChooser::changed,
            this, &GoToolChainConfigWidget::handleCompilerCommandChange);
    connect(m_abiWidget, &AbiWidget::abiChanged, this, &ToolChainConfigWidget::dirty);
}

void GoToolChainConfigWidget::applyImpl()
{
    // TODO
    if (toolChain()->isAutoDetected())
        return;

    GoToolChain *tc = static_cast<GoToolChain *>(toolChain());
    Q_ASSERT(tc);
    QString displayName = tc->displayName();
    tc->setCompilerCommand(m_compilerCommand->fileName());
    tc->setTargetAbi(m_abiWidget->currentAbi());
    tc->setDisplayName(displayName); // reset display name
}

void GoToolChainConfigWidget::discardImpl()
{
    // TODO
}

void GoToolChainConfigWidget::handleCompilerCommandChange()
{
    bool haveCompiler = false;
    Abi currentAbi = m_abiWidget->currentAbi();
    bool customAbi = m_abiWidget->isCustomAbi();
    FileName path = m_compilerCommand->fileName();
    QList<Abi> abiList;

    if (!path.isEmpty()) {
        QFileInfo fi(path.toFileInfo());
        haveCompiler = fi.isExecutable() && fi.isFile();
    }
    if (haveCompiler) {
        Environment env = Environment::systemEnvironment();
        GccToolChain::addCommandPathToEnvironment(path, env);
        abiList = GoToolChain::guessGoAbi(path, env.toStringList());
    }
    m_abiWidget->setEnabled(haveCompiler);

    // Find a good ABI for the new compiler:
    Abi newAbi;
    if (customAbi)
        newAbi = currentAbi;
    else if (abiList.contains(currentAbi))
        newAbi = currentAbi;

    m_abiWidget->setAbis(abiList, newAbi);
    emit dirty();
}

bool GoToolChainConfigWidget::isDirtyImpl() const
{
    GoToolChain *tc = static_cast<GoToolChain *>(toolChain());
    Q_ASSERT(tc);
    return m_compilerCommand->fileName() != tc->compilerCommand()
            || m_abiWidget->currentAbi() != tc->targetAbi();
}

void GoToolChainConfigWidget::makeReadOnlyImpl()
{
    m_compilerCommand->setReadOnly(true);
    m_abiWidget->setEnabled(false);
    m_isReadOnly = true;
}

void GoToolChainConfigWidget::setFromToolchain()
{
    bool blocked = blockSignals(true);
    GoToolChain *tc = static_cast<GoToolChain *>(toolChain());
    m_compilerCommand->setFileName(tc->compilerCommand());
    m_abiWidget->setAbis(tc->supportedAbis(), tc->targetAbi());
    if (!m_isReadOnly && !m_compilerCommand->path().isEmpty())
        m_abiWidget->setEnabled(true);
    blockSignals(blocked);
}

} // namespace Internal
} // namespace GoCompiler
