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

#include "gotoolchainfactory.h"
#include "gotoolchain.h"
#include "gocompilerconstants.h"

#include <projectexplorer/abi.h>
#include <projectexplorer/gcctoolchain.h>
#include <utils/environment.h>

#include <QProcess>
#include <QDebug>

using namespace ProjectExplorer;
using namespace Utils;

namespace GoCompiler {
namespace Internal {

GoToolChainFactory::GoToolChainFactory()
{
    setId(Constants::GO_TOOLCHAIN_ID);
    setDisplayName(tr("Go"));
}

QList<ToolChain *> GoToolChainFactory::autoDetect()
{
    QList<ToolChain *> tcs;

    Environment env = Environment::systemEnvironment();

    // Add Go's default istallation directory (if it is not in PATH)
#if defined(Q_OS_UNIX) || defined(Q_OS_OSX)
    env.prependOrSetPath(QLatin1String("/usr/local/go/bin"));
#elif defined(Q_OS_WIN)
    env.prependOrSetPath(QLatin1String("c:\\Go\\bin"));
#endif
    const FileName compilerPath = env.searchInPath(QLatin1String("go"));
    if (compilerPath.isEmpty())
        return tcs;

    GccToolChain::addCommandPathToEnvironment(compilerPath, env);

    const Abi requiredAbi = Abi::hostAbi();
    QList<Abi> abiList = GoToolChain::guessGoAbi(compilerPath, env.toStringList());
    if (!abiList.contains(requiredAbi)) {
        if (requiredAbi.wordWidth() != 64
                || !abiList.contains(Abi(requiredAbi.architecture(), requiredAbi.os(), requiredAbi.osFlavor(),
                                         requiredAbi.binaryFormat(), 32)))
            return tcs;
    }

    foreach (const Abi &abi, abiList) {
        QScopedPointer<GoToolChain> tc(new GoToolChain(ToolChain::AutoDetection));
        if (tc.isNull())
            return tcs;

        tc->setCompilerCommand(compilerPath);
        tc->setTargetAbi(abi);
        tc->setDisplayName(tr("Go (%1bit in %2)")
                           .arg(abi.wordWidth())
                           .arg(tc->compilerCommand().parentDir().toUserOutput()));

        tcs.append(tc.take());
    }

    return tcs;
}

bool GoToolChainFactory::canCreate()
{
    return true;
}

bool GoToolChainFactory::canRestore(const QVariantMap &data)
{
    const QString id = idFromMap(data);
    return id.startsWith(QLatin1String(Constants::GO_TOOLCHAIN_ID) + QLatin1Char(':'));
}

ToolChain *GoToolChainFactory::create()
{
    return new GoToolChain(ToolChain::ManualDetection);
}

ToolChain *GoToolChainFactory::restore(const QVariantMap &data)
{
    GoToolChain *tc = new GoToolChain(ToolChain::ManualDetection);
    QVariantMap updated = data;
    if (tc->fromMap(updated))
        return tc;

    delete tc;
    return 0;
}

} // namespace Internal
} // namespace GoCompiler
