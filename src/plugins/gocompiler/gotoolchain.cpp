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

#include "gotoolchain.h"
#include "gocompilerconstants.h"
#include "gooutputparser.h"
#include "gotoolchainfactory.h"
#include "gotoolchainconfigwidget.h"

#include <projectexplorer/gcctoolchain.h>
#include <projectexplorer/headerpath.h>
#include <utils/environment.h>
#include <utils/synchronousprocess.h>

#include <QtCore/QFileInfo>
#include <QtCore/QProcess>

#include <QtDebug>

static const char COMPILER_COMMAND_KEY[] = "GoCompiler.GoToolChain.Path";
static const char TARGET_ABI_KEY[] = "GoCompiler.GoToolChain.TargetAbi";
static const char SUPPORTED_ABI_KEY[] = "GoCompiler.GoToolChain.SupportedAbis";

using namespace ProjectExplorer;
using namespace Utils;

namespace GoCompiler {
namespace Internal {

GoToolChain::GoToolChain(const QString &id, Detection d) :
    ToolChain(id, d)
{
}

GoToolChain::GoToolChain(const GoToolChain &tc) :
    ToolChain(tc),
    m_compilerCommand(tc.compilerCommand()),
    m_targetAbi(tc.targetAbi()),
    m_supportedAbis(tc.supportedAbis())
{
}

GoToolChain::GoToolChain(Detection d) :
    ToolChain(QLatin1String(Constants::GO_TOOLCHAIN_ID), d)
{
}

void GoToolChain::addToEnvironment(Utils::Environment &env) const
{
    GccToolChain::addCommandPathToEnvironment(m_compilerCommand, env);
}

ToolChain *GoToolChain::clone() const
{
    return new GoToolChain(*this);
}

FileName GoToolChain::compilerCommand() const
{
    return m_compilerCommand;
}

ToolChain::CompilerFlags GoToolChain::compilerFlags(const QStringList &cxxflags) const
{
    // TODO
    return 0;
}

ToolChainConfigWidget *GoToolChain::configurationWidget()
{
    return new GoToolChainConfigWidget(this);
}

QList<Abi> GoToolChain::detectSupportedAbis() const
{
    Environment env = Environment::systemEnvironment();
    addToEnvironment(env);
    return guessGoAbi(m_compilerCommand, env.toStringList());
}

QList<ProjectExplorer::Abi> GoToolChain::guessGoAbi(const Utils::FileName &path, const QStringList &env)
{
    QList<Abi> abiList;
    if (path.isEmpty())
        return abiList;

    QStringList arguments(QLatin1String("version"));
    QString version = QString::fromLocal8Bit(runGo(path, arguments, env)).trimmed();
    if (version.isEmpty())
        return QList<Abi>(); // no need to continue if running failed once...

    QStringList details = version.split(QRegExp(QLatin1String("[ /-]")));

    Abi::Architecture arch = Abi::UnknownArchitecture;
    Abi::OS os = Abi::UnknownOS;
    Abi::OSFlavor flavor = Abi::UnknownFlavor;
    Abi::BinaryFormat format = Abi::UnknownFormat;
    int width = 0;
    foreach (QString detail, details) {
        if (detail == QLatin1String("go") || detail == QLatin1String("version"))
            continue;
        if (detail == QLatin1String("linux")) {
            os = Abi::LinuxOS;
            if (flavor == Abi::UnknownFlavor)
                flavor = Abi::GenericLinuxFlavor;
            format = Abi::ElfFormat;
        }
        if (detail.contains(QLatin1String("darwin"))) {
            os = Abi::MacOS;
            flavor = Abi::GenericMacFlavor;
            format = Abi::MachOFormat;
        }
        if (detail.contains(QLatin1String("windows"))) {
            arch = Abi::X86Architecture;
            os = Abi::WindowsOS;
            flavor = Abi::WindowsMSysFlavor;
            format = Abi::PEFormat;
        }
        if (detail.contains(QLatin1String("386"))) {
            arch = Abi::X86Architecture;
            width = 32;
        }
        if (detail.contains(QLatin1String("amd64"))) {
            arch = Abi::X86Architecture;
            width = 64;
        }
    }

    abiList.append(Abi(arch, os, flavor, format, width));

    // TODO: Add all cross-compile Abi's according to GOROOT/bin platform subdirectories
    // TODO: Default Abi should be selected according to GOOS/GOARCH from 'go env'

    return abiList;
}

/*
virtual CompilerFlags defaultCompilerFlags() const;
virtual QString defaultDisplayName() const;
virtual QList<Abi> detectSupportedAbis() const;
virtual QString detectVersion() const;
*/

bool GoToolChain::fromMap(const QVariantMap &data)
{
    if (!ToolChain::fromMap(data))
        return false;

    m_compilerCommand = FileName::fromString(data.value(QLatin1String(COMPILER_COMMAND_KEY)).toString());
    m_targetAbi = Abi(data.value(QLatin1String(TARGET_ABI_KEY)).toString());
    QStringList abiList = data.value(QLatin1String(SUPPORTED_ABI_KEY)).toStringList();
    m_supportedAbis.clear();
    foreach (const QString &a, abiList) {
        Abi abi(a);
        if (!abi.isValid())
            continue;
        m_supportedAbis.append(abi);
    }
    return true;
}

bool GoToolChain::isValid() const
{
    if (m_compilerCommand.isNull())
        return false;

    QFileInfo fi = compilerCommand().toFileInfo();
    return fi.isExecutable();
}

QString GoToolChain::makeCommand(const Utils::Environment &environment) const
{
    // TODO
    return QString();
}

bool GoToolChain::operator ==(const ToolChain &tc) const
{
    if (!ToolChain::operator ==(tc))
        return false;

    const GoToolChain *goTc = static_cast<const GoToolChain *>(&tc);
    return (m_compilerCommand == goTc->m_compilerCommand)
            && (m_targetAbi == goTc->m_targetAbi);
}

IOutputParser *GoToolChain::outputParser() const
{
    return new GoOutputParser;
}

QByteArray GoToolChain::predefinedMacros(const QStringList &cxxflags) const
{
    // TODO
    return QByteArray();
}

QByteArray GoToolChain::runGo(const FileName &goCommand, const QStringList &arguments, const QStringList &env)
{
    if (goCommand.isEmpty() || !goCommand.toFileInfo().isExecutable())
        return QByteArray();

    QProcess goProc;
    goProc.setEnvironment(env);
    goProc.start(goCommand.toString(), arguments);
    if (!goProc.waitForStarted()) {
        qWarning("%s: Cannot start '%s': %s", Q_FUNC_INFO, qPrintable(goCommand.toUserOutput()),
                 qPrintable(goProc.errorString()));
        return QByteArray();
    }
    goProc.closeWriteChannel();
    if (!goProc.waitForFinished(10000)) {
        SynchronousProcess::stopProcess(goProc);
        qWarning("%s: Timeout running '%s'.", Q_FUNC_INFO, qPrintable(goCommand.toUserOutput()));
        return QByteArray();
    }
    if (goProc.exitStatus() != QProcess::NormalExit) {
        qWarning("%s: '%s' crashed.", Q_FUNC_INFO, qPrintable(goCommand.toUserOutput()));
        return QByteArray();
    }

    const QByteArray stdErr = goProc.readAllStandardError();
    if (goProc.exitCode() != 0) {
        qWarning().nospace()
                << Q_FUNC_INFO << ": " << goCommand.toUserOutput() << ' '
                << arguments.join(QLatin1String(" ")) << " returned exit code "
                << goProc.exitCode() << ": " << stdErr;
        return QByteArray();
    }

    QByteArray data = goProc.readAllStandardOutput();
    if (!data.isEmpty() && !data.endsWith('\n'))
        data.append('\n');
    data.append(stdErr);
    return data;
}

void GoToolChain::setCompilerCommand(const FileName &path)
{
    if (path == m_compilerCommand)
        return;

    m_compilerCommand = path;

    Abi currentAbi = m_targetAbi;
    m_supportedAbis = detectSupportedAbis();

    m_targetAbi = Abi();
    if (!m_supportedAbis.isEmpty()) {
        if (m_supportedAbis.contains(currentAbi))
            m_targetAbi = currentAbi;
        else
            m_targetAbi = m_supportedAbis.at(0);
    }

    toolChainUpdated();
}

void GoToolChain::setTargetAbi(const Abi &abi)
{
    if (abi == m_targetAbi)
        return;

    m_targetAbi = abi;
    toolChainUpdated();
}

QList<FileName> GoToolChain::suggestedMkspecList() const
{
    // TODO
    return QList<FileName>();
}

QList<Abi> GoToolChain::supportedAbis() const
{
    return m_supportedAbis;
}

QList<HeaderPath> GoToolChain::systemHeaderPaths(const QStringList &cxxflags, const Utils::FileName &sysRoot) const
{
    // TODO
    return QList<HeaderPath>();
}

Abi GoToolChain::targetAbi() const
{
    return m_targetAbi;
}

QVariantMap GoToolChain::toMap() const
{
    QVariantMap data = ToolChain::toMap();
    data.insert(QLatin1String(COMPILER_COMMAND_KEY), m_compilerCommand.toString());
    data.insert(QLatin1String(TARGET_ABI_KEY), m_targetAbi.toString());
    QStringList abiList;
    foreach (const Abi &a, m_supportedAbis)
        abiList.append(a.toString());
    data.insert(QLatin1String(SUPPORTED_ABI_KEY), abiList);
    return data;
}

QString GoToolChain::type() const
{
    return QLatin1String("go");
}

QString GoToolChain::typeDisplayName() const
{
    return GoToolChainFactory::tr("Go");
}

ToolChain::WarningFlags GoToolChain::warningFlags(const QStringList &cflags) const
{
    // TODO
    return 0;
}

} // namespace Internal
} // namespace GoCompiler
