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

#ifndef GOOUTPUTPARSER_H
#define GOOUTPUTPARSER_H

#include <projectexplorer/ioutputparser.h>

#include <projectexplorer/task.h>

#include <QRegExp>

namespace GoCompiler {
namespace Internal {

class GoOutputParser : public ProjectExplorer::IOutputParser
{
    Q_OBJECT

public:
    GoOutputParser();

    void stdError(const QString &line);
    void stdOutput(const QString &line);

protected:
    void newTask(const ProjectExplorer::Task &task);
    void doFlush();

//    void amendDescription(const QString &desc, bool monospaced);

private:
    QRegExp m_regExp;

    ProjectExplorer::Task m_currentTask;
};

} // namespace Internal
} // namespace GoCompiler

#endif // ifndef GOOUTPUTPARSER_H
