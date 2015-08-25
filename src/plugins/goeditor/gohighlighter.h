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

#ifndef GOHIGHLIGHTER_H
#define GOHIGHLIGHTER_H

#include <texteditor/syntaxhighlighter.h>

#include <texteditor/textdocumentlayout.h>

#include "gotoken.h"

namespace GoEditor {
namespace Internal {

class GoHighlighter : public TextEditor::SyntaxHighlighter
{
    Q_OBJECT
public:
    explicit GoHighlighter(QTextDocument *parent = 0);
    explicit GoHighlighter(TextEditor::TextDocument *parent);
    ~GoHighlighter();

protected:
    void highlightBlock(const QString &text);

private:
    void init();
    void onBlockBegin();
    void onBlockEnd();
    int styleCategory(TextEditor::TextStyle style);
    int tokenCategory(const GoToken &tk);
};

} // namespace Internal
} // namespace GoEditor

#endif // #ifndef GOHIGHLIGHTER_H

