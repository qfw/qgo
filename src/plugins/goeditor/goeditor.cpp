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

#include "goeditor.h"
#include "goautocompleter.h"
#include "godocument.h"
#include "goeditorconstants.h"

#include <texteditor/texteditoractionhandler.h>
#include <texteditor/texteditorconstants.h>

using namespace TextEditor;

namespace GoEditor {
namespace Internal {

/*****************************************************************************
 * GoEditor
 *****************************************************************************/

GoEditor::GoEditor()
{
    setContext(Core::Context(Constants::GOEDITOR_ID,
                             TextEditor::Constants::C_TEXTEDITOR));
}

/*****************************************************************************
 * GoEditorFactory
 *****************************************************************************/

GoEditorFactory::GoEditorFactory()
{
    setId(Constants::GOEDITOR_ID);
    setDisplayName(tr(Constants::GOEDITOR_DISPLAY_NAME));
    addMimeType(QLatin1String(Constants::GO_MIMETYPE));

    setDocumentCreator([]() { return new GoDocument; });
    setEditorWidgetCreator([]() { return new GoEditorWidget; });
    setEditorCreator([]() { return new GoEditor; });
    setAutoCompleterCreator([]() { return new GoAutoCompleter; });

    setCommentStyle(Utils::CommentDefinition::CppStyle);
    setCodeFoldingSupported(true);
    setMarksVisible(true);
    setParenthesesMatchingEnabled(true);

    setEditorActionHandlers(TextEditorActionHandler::Format
                            | TextEditorActionHandler::UnCommentSelection
                            | TextEditor::TextEditorActionHandler::UnCollapseAll);

/*    new TextEditor::TextEditorActionHandler(this,
                              Constants::GOEDITOR_ID,
                              TextEditor::TextEditorActionHandler::Format
                              | TextEditor::TextEditorActionHandler::UnCommentSelection
                              | TextEditor::TextEditorActionHandler::UnCollapseAll);*/

    // TODO: Add TextEditor::TextEditorActionHandler::FollowSymbolUnderCursor
}

/*****************************************************************************
 * GoEditorWidget
 *****************************************************************************/

GoEditorWidget::GoEditorWidget()
{
}

} // namespace Internal
} // namespace GoEditor
