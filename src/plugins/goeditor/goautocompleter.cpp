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

#include "goautocompleter.h"

#include <QtDebug>

namespace GoEditor {
namespace Internal {

GoAutoCompleter::GoAutoCompleter() :
    TextEditor::AutoCompleter()
{
}

bool GoAutoCompleter::contextAllowsAutoParentheses(const QTextCursor & /*cursor*/,
                                  const QString & /*textToInsert*/) const
{
    return false;
}
bool GoAutoCompleter::contextAllowsElectricCharacters(const QTextCursor & /*cursor*/) const
{
    return true;
}

} // namespace Internal
} // namespace GoEditor
