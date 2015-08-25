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

#include "goparser.h"

#include <QtCore/QList>

#include <QtCore/QString>

#include <QtDebug>

const char * const GO_KEYWORDS[] {
    "break",
    "case",
    "chan",
    "const",
    "continue",
    "default",
    "defer",
    "else",
    "fallthrough",
    "false",
    "for",
    "func",
    "go",
    "goto",
    "if",
    "import",
    "interface",
    "iota",
    "map",
    "nil",
    "package",
    "range",
    "return",
    "select",
    "struct",
    "switch",
    "true",
    "type",
    "var",
};

const char * const GO_TYPES[] {
    "bool",
    "byte",
    "complex64",
    "complex128",
    "error",
    "float32",
    "float64",
    "int",
    "int8",
    "int16",
    "int32",
    "int64",
    "rune",
    "string",
    "uint",
    "uintptr",
    "uint8",
    "uint16",
    "uint32",
    "uint64",
};

const char * const GO_OPERATORS[] {
    "append",
    "cap",
    "close",
    "complex",
    "copy",
    "delete",
    "imag",
    "len",
    "make",
    "new",
    "panic",
    "print",
    "println",
    "real",
    "recover",
};

namespace GoEditor {
namespace Internal {

GoParser *GoParser::m_instance = 0;

GoParser::GoParser() :
    m_includeSpaces(true),
    m_includeComments(true),
    m_text(0),
    m_length(0),
    m_position(0)
{
    m_state.i = 0;

    m_instance = this;
    for (size_t i = 0; i < sizeof(GO_KEYWORDS)/sizeof(char *); ++i) {
        m_keywords.insert(QLatin1String(GO_KEYWORDS[i]));
        m_builtins.append(QLatin1String(GO_KEYWORDS[i]));
    }

    for (size_t i = 0; i < sizeof(GO_TYPES)/sizeof(char *); ++i) {
        m_types.insert(QLatin1String(GO_TYPES[i]));
        m_builtins.append(QLatin1String(GO_TYPES[i]));
    }

    for (size_t i = 0; i < sizeof(GO_OPERATORS)/sizeof(char *); ++i) {
        m_operators.insert(QLatin1String(GO_OPERATORS[i]));
        m_builtins.append(QLatin1String(GO_OPERATORS[i]));
    }
    qSort(m_builtins);
}

GoParser *GoParser::instance()
{
    return m_instance;
}

GoToken GoParser::parseDefault()
{
    QChar c = m_text[m_position++];
    GoToken token;

    if (c.isSpace()) // White space
        token = parseWhiteSpace();
    else if (c.isLetter() || c == QLatin1Char('_')) // Identifier
        token = parseIdentifier();
    else {
        switch (c.toLatin1()) {
        case '"':
            m_state.s.multiLine = MultiLineInterpretedString;
            token = parseString();
            break;
        case '`':
            m_state.s.multiLine = MultiLineRawString;
            token = parseString();
            break;
        case '/':
            token = tryParseComment();
            break;
        case '{':
            token = GoToken(m_tokenPosition, 1, T_LBRACE);
            ++m_state.s.indentation;
            break;
        case '}':
            token = GoToken(m_tokenPosition, 1, T_RBRACE);
            --m_state.s.indentation;
            break;
        case '(':
            token = GoToken(m_tokenPosition, 1, T_LPAREN);
            ++m_state.s.indentation;
            break;
        case ')':
            token = GoToken(m_tokenPosition, 1, T_RPAREN);
            --m_state.s.indentation;
            break;
        }
    }

    return token;
}

GoToken GoParser::parseIdentifier()
{
    QChar c;
    while (m_position < m_length) {
        c = m_text[m_position];
        if (!c.isLetterOrNumber() && c != QLatin1Char('_'))
            break;
        ++m_position;
    }

    const int tokenStart = m_tokenPosition;
    const int tokenCount = m_position - m_tokenPosition;
    QString identifier(&m_text[tokenStart], tokenCount);

    TokenKind kind = T_IDENTIFIER;
    if (m_keywords.contains(identifier))
        kind = T_KEYWORD;
    else if (m_types.contains(identifier))
        kind = T_PRIMITIVE;
    else if (m_operators.contains(identifier))
        kind = T_OPERATOR;

    return GoToken(tokenStart, tokenCount, kind);
}

GoToken GoParser::parseMultiLineComment()
{
    QChar c;
    bool expectCommentClosing = false;
    while (m_position < m_length) {
        c = m_text[m_position++];

        if (expectCommentClosing && c == QLatin1Char('/')) { // Multiline comment ended
            m_state.s.multiLine = NoMultiLine;
            break;
        }

        if (c == QLatin1Char('*'))
            expectCommentClosing = true;
        else
            expectCommentClosing = false;
    }
    if (!m_includeComments)
        return GoToken();
    return GoToken(m_tokenPosition, m_position - m_tokenPosition, T_COMMENT);
}

GoToken GoParser::parseSingleLineComment()
{
    QChar c;
    while (m_position < m_length) {
        c = m_text[m_position++];
        if (c == QLatin1Char('\n')) // TODO: Is it possible?
            break;
    }

    if (!m_includeComments)
        return GoToken();
    return GoToken(m_tokenPosition, m_position - m_tokenPosition, T_COMMENT);
}

GoToken GoParser::parseString()
{
    const MultiLine ml = m_state.s.multiLine;
    const QChar quoteChar = (ml == MultiLineRawString) ? QLatin1Char('`') : QLatin1Char('"');

    QChar c, prev;
    while (m_position < m_length) {
        prev = m_text[m_position - 1];
        c = m_text[m_position++];
        if (c == quoteChar) {
            bool ended = true;
            if (ml == MultiLineInterpretedString)
                ended = (prev != QLatin1Char('\\')); // \" sequence is not end of interpreted string
            if (ended) {
                m_state.s.multiLine = NoMultiLine;
                break;
            }
        }
    }

    return GoToken(m_tokenPosition, m_position - m_tokenPosition, T_STRING);
}

GoToken GoParser::parseWhiteSpace()
{
    QChar c;
    while (m_position < m_length) {
        c = m_text[m_position];
        if (!c.isSpace())
            break;
        ++m_position;
    }
    if (!m_includeSpaces)
        return GoToken();
    return GoToken(m_tokenPosition, m_position - m_tokenPosition, T_WHITESPACE);
}

QList<GoToken> GoParser::tokenize(const QString &text, int prevBlockState)
{
    QList<GoToken> tokens;

    m_text = text.constData();
    m_tokenPosition = m_position = 0;
    m_length = text.length();
    if (prevBlockState < 0)
        prevBlockState = 0;
    m_state.i = prevBlockState;
    GoToken token;

    while (m_position < m_length) {
        switch (m_state.s.multiLine) {
        case MultiLineInterpretedString:
        case MultiLineRawString:
            token = parseString();
            break;
        case MultiLineComment:
            token = parseMultiLineComment();
            break;
        default:
            token = parseDefault();
            break;
        }
        if (token.isValid())
            tokens.append(token);
        m_tokenPosition = m_position;
    }

    return tokens;
}

QList<GoToken> GoParser::tokenize(const QTextBlock &block)
{
    const int prevState = block.previous().isValid() ? block.previous().userState() : -1;
    return tokenize(block.text(), prevState);
}

GoToken GoParser::tryParseComment()
{
    if (m_position < m_length) {
        QChar c = m_text[m_position++];
        if (c == QLatin1Char('/')) { // Single line comment begin: //
            return parseSingleLineComment();
        } else if (c == QLatin1Char('*')) { // Multiline comment begin: /*
            m_state.s.multiLine = MultiLineComment;
            return parseMultiLineComment();
        }
    }
    return GoToken(); // It's not a comment
}

} // namespace Internal
} // namespace GoEditor
