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

#ifndef GODOCUMENT_H
#define GODOCUMENT_H

#include <texteditor/textdocument.h>

namespace GoEditor {
namespace Internal {

class GoCompletionAssistProvider;

class GoDocument : public TextEditor::TextDocument
{
    Q_OBJECT
public:
    explicit GoDocument();
    ~GoDocument();

    TextEditor::CompletionAssistProvider *completionAssistProvider() const Q_DECL_OVERRIDE;

private:
    GoCompletionAssistProvider *m_completionAssistProvider;
};

} // namespace Internal
} // namespace GoEditor

#endif // #ifndef GODOCUMENT_H

