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

#ifndef GOCOMPLETIONASSISTPROVIDER_H
#define GOCOMPLETIONASSISTPROVIDER_H

#include <texteditor/codeassist/completionassistprovider.h>

namespace GoEditor {
namespace Internal {

class GoCompletionAssistProvider : public TextEditor::CompletionAssistProvider
{
    Q_OBJECT
public:
    GoCompletionAssistProvider();

    bool supportsEditor(Core::Id editorId) const;
    int activationCharSequenceLength() const;
    bool isActivationCharSequence(const QString &sequence) const;
    bool isContinuationChar(const QChar &c) const;

    TextEditor::IAssistProcessor *createProcessor() const;

    /*TextEditor::IAssistInterface *createAssistInterface(
            ProjectExplorer::Project *project, TextEditor::BaseTextEditor *editor,
            QTextDocument *document, bool isObjCEnabled, int position,
            TextEditor::AssistReason reason) const = 0;*/


};

} // namespace Internal
} // namespace GoEditor

#endif // #ifndef GOCOMPLETIONASSISTPROVIDER_H

