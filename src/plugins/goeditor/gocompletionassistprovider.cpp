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

#include "gocompletionassistprovider.h"
#include "goeditorconstants.h"
#include "golexer.h"

#include <coreplugin/id.h>
#include <texteditor/codeassist/keywordscompletionassist.h>

#include <QtDebug>

namespace GoEditor {
namespace Internal {

GoCompletionAssistProvider::GoCompletionAssistProvider() :
    TextEditor::CompletionAssistProvider()
{
}

int GoCompletionAssistProvider::activationCharSequenceLength() const
{
    return 3;
}

TextEditor::IAssistProcessor *GoCompletionAssistProvider::createProcessor() const
{
    GoLexer *lexer = GoLexer::instance();
    TextEditor::Keywords keywords = TextEditor::Keywords(lexer->builtins(),
                                                         QStringList(),
                                                         QMap<QString, QStringList>());
    return new TextEditor::KeywordsCompletionAssistProcessor(keywords);
}

bool GoCompletionAssistProvider::isActivationCharSequence(const QString & /*sequence*/) const
{
    return false;
}

bool GoCompletionAssistProvider::isContinuationChar(const QChar & /*c*/) const
{
    return false;
}


bool GoCompletionAssistProvider::supportsEditor(Core::Id editorId) const
{
    return (editorId == Constants::GOEDITOR_ID);
}



} // namespace Internal
} // namespace GoEditor
