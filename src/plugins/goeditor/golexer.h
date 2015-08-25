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

#ifndef GOLEXER_H
#define GOLEXER_H

#include "gotoken.h"

#include <texteditor/texteditorconstants.h>

#include <QtCore/QSet>

#include <QtGui/QTextBlock>

namespace GoEditor {
namespace Internal {

class LexerState {
public:
    enum MultiLine {
        NoMultiLine = 0,
        MultiLineInterpretedString,
        MultiLineRawString,
        MultiLineComment
    };
    LexerState(int state){ m_state = state < 0 ? 0 : state; }
    int state() const { return m_state; }
    void setState(int state) { m_state = state; }

    MultiLine multiLine() const { return m_fields.multiLine; }
    void setMultiLine(MultiLine multiLine) { m_fields.multiLine = multiLine; }

    int braceDepth() const { return m_fields.braceDepth; }
    void setBraceDepth(int braceDepth) { m_fields.braceDepth = braceDepth; }

private:
    struct Fields {
        MultiLine multiLine : 4;
        int braceDepth      : 8;
    };
    union {
        Fields m_fields;
        int m_state;
    };
};

class GoLexer
{
public:
    GoLexer();
    static GoLexer *instance();

    QList<GoToken> tokenize(const QString &text, int prevBlockState);
    QList<GoToken> tokenize(const QTextBlock &block);
    LexerState lexerState() const { return m_lexerState; }

    const QStringList &builtins() const { return m_builtins; }

private:
    QChar ch(int offset = 0); // returns current char
    QChar chpp(); // returns current char with position increment

    void skipWhiteSpace();
    GoToken parseDefault();
    GoToken parseString();
    GoToken parseRune();
    GoToken parseSingleLineComment();
    GoToken parseMultiLineComment();
    GoToken parseIdentifier();
    GoToken parseNumber(QChar first);

private:
    static GoLexer *m_instance;

    LexerState m_lexerState;
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

#endif // #ifndef GOLEXER_H

