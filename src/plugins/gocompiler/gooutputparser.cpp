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

#include "gooutputparser.h"

#include <projectexplorer/projectexplorerconstants.h>
#include <projectexplorer/task.h>
#include <utils/fileutils.h>

using namespace ProjectExplorer;
using namespace Utils;

namespace GoCompiler {
namespace Internal {

GoOutputParser::GoOutputParser()
{
    setObjectName(QLatin1String("GoOutputParser"));

    m_regExp.setPattern(QLatin1String("(.*):(\\d+): (.*)$"));
    m_regExp.setMinimal(true);
}

void GoOutputParser::stdError(const QString &line)
{
    QString lne = rightTrimmed(line);
    if (m_regExp.indexIn(lne) > -1) {
        FileName filename = FileName::fromUserInput(m_regExp.cap(1));
        int lineno = m_regExp.cap(2).toInt();
        Task::TaskType type = Task::Error;
        QString description = m_regExp.cap(3);

        Task task(type, description, filename, lineno, Constants::TASK_CATEGORY_COMPILE);
        newTask(task);

    }
}

void GoOutputParser::stdOutput(const QString &line)
{
    doFlush();
    IOutputParser::stdOutput(line);
}

void GoOutputParser::newTask(const Task &task)
{
    doFlush();
    m_currentTask = task;
}

void GoOutputParser::doFlush()
{
    if (m_currentTask.isNull())
        return;
    Task t = m_currentTask;
    m_currentTask.clear();
    emit addTask(t);
}

} // namespace Internal
} // namespace GoCompiler
