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

#ifndef GOTOOLCHAIN_H
#define GOTOOLCHAIN_H

#include <projectexplorer/toolchain.h>

#include <projectexplorer/abi.h>
#include <utils/fileutils.h>

namespace GoCompiler {
namespace Internal {

class GoToolChain : public ProjectExplorer::ToolChain
{
public:
    GoToolChain(const QString &id, Detection d);

    static QByteArray runGo(const Utils::FileName &goCommand, const QStringList &arguments, const QStringList &env);
    static QList<ProjectExplorer::Abi> guessGoAbi(const Utils::FileName &path, const QStringList &env);

    QString type() const;
    QString typeDisplayName() const;
    ProjectExplorer::Abi targetAbi() const;
    void setTargetAbi(const ProjectExplorer::Abi &abi);

    QList<ProjectExplorer::Abi> supportedAbis() const;

    bool isValid() const;

    QByteArray predefinedMacros(const QStringList &cxxflags) const;
    CompilerFlags compilerFlags(const QStringList &cxxflags) const;
    WarningFlags warningFlags(const QStringList &cflags) const;

    QList<ProjectExplorer::HeaderPath> systemHeaderPaths(const QStringList &cxxflags, const Utils::FileName &sysRoot) const;
    void addToEnvironment(Utils::Environment &env) const;
    QString makeCommand(const Utils::Environment &environment) const;
    QList<Utils::FileName> suggestedMkspecList() const;
    ProjectExplorer::IOutputParser *outputParser() const;

    QVariantMap toMap() const;
    bool fromMap(const QVariantMap &data);

    ProjectExplorer::ToolChainConfigWidget *configurationWidget();

    bool operator ==(const ProjectExplorer::ToolChain &tc) const;

    Utils::FileName compilerCommand() const;
    void setCompilerCommand(const Utils::FileName &path);

    ProjectExplorer::ToolChain *clone() const;

protected:
    GoToolChain(const GoToolChain &tc);
/*
    virtual QString defaultDisplayName() const;
    virtual CompilerFlags defaultCompilerFlags() const;

    virtual QList<Abi> detectSupportedAbis() const;
    virtual QString detectVersion() const;
*/
    virtual QStringList reinterpretOptions(const QStringList &argument) const { return argument; }

private:
    explicit GoToolChain(Detection d);
    QList<ProjectExplorer::Abi> detectSupportedAbis() const;

private:
    Utils::FileName m_compilerCommand;
    ProjectExplorer::Abi m_targetAbi;
    QList<ProjectExplorer::Abi> m_supportedAbis;

    friend class GoToolChainFactory;
};



} // namespace Internal
} // namespace GoCompiler

#endif // #ifndef GOTOOLCHAIN_H
