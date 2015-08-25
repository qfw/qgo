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

#ifndef GOINDENTER_H
#define GOINDENTER_H

#include "gotoken.h"

#include <texteditor/indenter.h>

#include <QtCore/QSet>

namespace GoEditor {
namespace Internal {

class GoIndenter : public TextEditor::Indenter
{
public:
    GoIndenter();

    bool isElectricCharacter(const QChar &ch) const;
    void indentBlock(QTextDocument *doc,
                     const QTextBlock &block,
                     const QChar &typedChar,
                     const TextEditor::TabSettings &tabSettings);
};

} // namespace Internal
} // namespace GoEditor

#endif // #ifndef GOINDENTER_H

