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

#include "godocument.h"
#include "gocompletionassistprovider.h"
#include "goeditorconstants.h"
#include "gohighlighter.h"
#include "goindenter.h"

using namespace TextEditor;

namespace GoEditor {
namespace Internal {

GoDocument::GoDocument()
{
    setId(Constants::GOEDITOR_ID);
    setSyntaxHighlighter(new GoHighlighter);
    setIndenter(new GoIndenter);
    //setCompletionAssistProvider(new GoCompletionAssistProvider); // TODO: Doesn't work. Why?

    m_completionAssistProvider = new GoCompletionAssistProvider;
}

GoDocument::~GoDocument()
{
    delete m_completionAssistProvider;
}

TextEditor::CompletionAssistProvider *GoDocument::completionAssistProvider() const
{
    return m_completionAssistProvider;
}

} // namespace Internal
} // namespace GoEditor
