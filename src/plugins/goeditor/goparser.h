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

#ifndef GOPARSER_H
#define GOPARSER_H

#include "gotoken.h"

#include <texteditor/texteditorconstants.h>

#include <QtCore/QSet>

#include <QtGui/QTextBlock>

namespace GoEditor {
namespace Internal {

enum MultiLine {
    NoMultiLine = 0,
    MultiLineInterpretedString,
    MultiLineRawString,
    MultiLineComment
};

struct BlockState {
    MultiLine multiLine : 4;
    int indentation : 16;
};

class GoParser
{
public:
    GoParser();
    static GoParser *instance();

    QList<GoToken> tokenize(const QString &text, int prevBlockState);
    QList<GoToken> tokenize(const QTextBlock &block);
    int state() const { return m_state.i; }
    int indentation() const { return m_state.s.indentation; }
    void includeSpaces() { m_includeSpaces = true; }
    void excludeSpaces()  { m_includeSpaces = false; }
    void includeComments() { m_includeComments = true; }
    void excludeComments() { m_includeComments = false; }
    const QStringList &builtins() const { return m_builtins; }

private:
    GoToken parseDefault();
    GoToken parseString();
    GoToken parseSingleLineComment();
    GoToken parseMultiLineComment();
    GoToken parseWhiteSpace();
    GoToken parseIdentifier();
    GoToken tryParseComment();

private:
    union {
        int i;
        BlockState s;
    } m_state;
    static GoParser *m_instance;
    bool m_includeSpaces;
    bool m_includeComments;
    const QChar *m_text;
    int m_length;
    int m_tokenPosition;
    int m_position;
    QSet<QString> m_keywords;
    QSet<QString> m_types;
    QSet<QString> m_operators;
    QStringList m_builtins;
};

} // namespace Internal
} // namespace GoEditor

#endif // #ifndef GOPARSER_H

