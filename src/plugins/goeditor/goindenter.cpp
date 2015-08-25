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

#include "goindenter.h"
#include "gocodeformatter.h"
//#include "golexer.h"

#include <texteditor/tabsettings.h>

#include <QtDebug>

//#include <QtGui/QTextDocument>

namespace GoEditor {
namespace Internal {

GoIndenter::GoIndenter() :
    TextEditor::Indenter()
{

}

void GoIndenter::indentBlock(QTextDocument * /*doc*/, const QTextBlock &block,
                             const QChar &typedChar, const TextEditor::TabSettings &tabSettings)
{
    GoCodeFormatter codeFormatter(tabSettings);

    codeFormatter.updateStateUntil(block);
    int indentation;
    int padding;
    codeFormatter.calcIndentation(block, &indentation, &padding);

    if (isElectricCharacter(typedChar)) {
        int defaultIndentation;
        int defaultPadding;
        codeFormatter.calcIndentation(block, &defaultIndentation, &defaultPadding, true);
        if (tabSettings.indentationColumn(block.text()) != defaultIndentation + defaultPadding)
            return;
    }

    tabSettings.indentLine(block, indentation + padding, padding);
}

bool GoIndenter::isElectricCharacter(const QChar &ch) const
{
    switch (ch.toLatin1()) {
    case '{':
    case '}':
    case '(':
    case ')':
    case ':': // "case ... :" or "Label:"
        return true;
    }

    return false;
}

} // namespace Internal
} // namespace GoEditor
