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

#include "golexer.h"

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

GoLexer *GoLexer::m_instance = 0;

GoLexer::GoLexer() :
    m_lexerState(0),
    m_text(0),
    m_length(0),
    m_position(0)
{
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

QChar GoLexer::ch(int offset)
{
    if ((m_position + offset) < m_length)
        return m_text[m_position + offset];
    return QLatin1Char('\0');
}

QChar GoLexer::chpp()
{
    if (m_position < m_length)
        return m_text[m_position++];
    return QLatin1Char('\0');
}

GoLexer *GoLexer::instance()
{
    return m_instance;
}

GoToken GoLexer::parseDefault()
{
    QChar c = chpp();
    GoToken token;

    if (c.isSpace()) // White space
        skipWhiteSpace();
    else if (c.isLetter() || c == QLatin1Char('_')) // Identifier
        token = parseIdentifier();
    else if (c.isDigit()) // Number
        token = parseNumber(c);
    else {
        switch (c.toLatin1()) {
        // Strings & runes
        case '"':
            m_lexerState.setMultiLine(LexerState::MultiLineInterpretedString);
            token = parseString();
            break;
        case '`':
            m_lexerState.setMultiLine(LexerState::MultiLineRawString);
            token = parseString();
            break;
        case '\'':
            token = parseRune();
            break;

        // Comments
        case '/':
            switch (ch().toLatin1()) {
            case '/':
                token = parseSingleLineComment();
                break;
            case '*':
                m_lexerState.setMultiLine(LexerState::MultiLineComment);
                token = parseMultiLineComment();
                break;
            default:
                token = GoToken(m_tokenPosition, 1, T_SLASH);
            }
            break;

        // Operations
        case '+':
            if (ch() == QLatin1Char('='))
                token = GoToken(m_tokenPosition, 2, T_PLUS_EQUAL);
            else
                token = GoToken(m_tokenPosition, 1, T_PLUS);
            break;
        case '-':
            if (ch() == QLatin1Char('='))
                token = GoToken(m_tokenPosition, 2, T_MINUS_EQUAL);
            else if (ch() == QLatin1Char('>'))
                token = GoToken(m_tokenPosition, 2, T_RARROW);
            else
                token = GoToken(m_tokenPosition, 1, T_MINUS);
            break;
        case ':':
            if (ch() == QLatin1Char('='))
                token = GoToken(m_tokenPosition, 2, T_COLON_EQUAL);
            else
                token = GoToken(m_tokenPosition, 1, T_COLON );
            break;
        case '=':
            if (ch() == QLatin1Char('='))
                token = GoToken(m_tokenPosition, 2, T_EQUAL_EQUAL);
            else
                token = GoToken(m_tokenPosition, 1, T_EQUAL);
            break;
        case '.':
            if (ch().isDigit())
                parseNumber(c);
            else if (ch() == QLatin1Char('.') && ch(1) == QLatin1Char('.'))
                token = GoToken(m_tokenPosition, 3, T_DOT_DOT_DOT);
            else
                token = GoToken(m_tokenPosition, 1, T_DOT);
            break;
        case '<':
            if (ch() == QLatin1Char('<')) {
                if (ch(1) == QLatin1Char('='))
                    token = GoToken(m_tokenPosition, 3, T_LESS_LESS_EQUAL);
                else
                    token = GoToken(m_tokenPosition, 2, T_LESS_LESS);
            } else if (ch() == QLatin1Char('=')) {
                token = GoToken(m_tokenPosition, 2, T_LESS_EQUAL);
            } else if (ch() == QLatin1Char('-')) {
                token = GoToken(m_tokenPosition, 2, T_LARROW);
            } else {
                token = GoToken(m_tokenPosition, 1, T_LESS);
            }
            break;
        case '>':
            if (ch() == QLatin1Char('>')) {
                if (ch(1) == QLatin1Char('='))
                    token = GoToken(m_tokenPosition, 3, T_GREATER_GREATER_EQUAL);
                else
                    token = GoToken(m_tokenPosition, 2, T_GREATER_GREATER);
            } else if (ch() == QLatin1Char('=')) {
                token = GoToken(m_tokenPosition, 2, T_GREATER_EQUAL);
            } else {
                token = GoToken(m_tokenPosition, 1, T_GREATER);
            }
            break;
        case '!':
            if (ch() == QLatin1Char('='))
                token = GoToken(m_tokenPosition, 2, T_EXCLAM_EQUAL);
            else
                token = GoToken(m_tokenPosition, 1, T_EXCLAM);
            break;
        case '|':
            if (ch() == QLatin1Char('='))
                token = GoToken(m_tokenPosition, 2, T_PIPE_EQUAL);
            else if (ch() == QLatin1Char('|'))
                token = GoToken(m_tokenPosition, 1, T_PIPE_PIPE);
            else
                token = GoToken(m_tokenPosition, 1, T_PIPE);
            break;
        case '*':
            if (ch() == QLatin1Char('='))
                token = GoToken(m_tokenPosition, 2, T_ASTERISK_EQUAL);
            else
                token = GoToken(m_tokenPosition, 1, T_ASTERISK);
            break;
        case '&':
            if (ch() == QLatin1Char('^')) {
                if (ch(1) == QLatin1Char('='))
                    token = GoToken(m_tokenPosition, 3, T_AMPER_CARET_EQUAL);
                else
                    token = GoToken(m_tokenPosition, 2, T_AMPER_CARET);
            } else if (ch() == QLatin1Char('&')) {
                token = GoToken(m_tokenPosition, 2, T_AMPER_AMPER);
            } else {
                token = GoToken(m_tokenPosition, 1, T_AMPER);
            }
            break;
        case ',':
            token = GoToken(m_tokenPosition, 1, T_COMMA);
            break;

        // Misc

        // Delimiters
        case ';':
            token = GoToken(m_tokenPosition, 1, T_SEMICOLON);
            break;
        case '{':
        case '}':
        case '[':
        case ']':
        case '(':
        case ')':
            token = GoToken(m_tokenPosition, 1, T_PARENTHESIS, m_text);
            break;
        default:
            break;
        }
    }

    return token;
}

GoToken GoLexer::parseIdentifier()
{
    QChar c;
    while (m_position < m_length) {
        c = ch();
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

    return GoToken(tokenStart, tokenCount, kind, m_text);
}

GoToken GoLexer::parseMultiLineComment()
{
    QChar c;
    bool expectCommentClosing = false;
    while (m_position < m_length) {
        c = chpp();

        if (expectCommentClosing && c == QLatin1Char('/')) { // Multiline comment ended
            m_lexerState.setMultiLine(LexerState::NoMultiLine);
            break;
        }

        if (c == QLatin1Char('*'))
            expectCommentClosing = true;
        else
            expectCommentClosing = false;
    }
    return GoToken(m_tokenPosition, m_position - m_tokenPosition, T_COMMENT);
}

GoToken GoLexer::parseRune()
{
    QChar c;
    while (m_position < m_length) {
        c = chpp();
        if (c == QLatin1Char('\''))
            break;
    }

    return GoToken(m_tokenPosition, m_position - m_tokenPosition, T_RUNE);
}

GoToken GoLexer::parseSingleLineComment()
{
    QChar c;
    while (m_position < m_length) {
        c = chpp();
        if (c == QLatin1Char('\n')) // TODO: Is it possible?
            break;
    }

    return GoToken(m_tokenPosition, m_position - m_tokenPosition, T_COMMENT);
}

GoToken GoLexer::parseNumber(QChar first)
{
    enum NumberFlag {
        FixedPointType = 0x1,
        FloatPointType = 0x2,
        HexType = 0x4,
        OctalType = 0x8,
    };
    int flags;
    switch (first.toLatin1()) {
    case '0':
        if (ch() == QLatin1Char('x') || ch() == QLatin1Char('X'))
            flags = HexType;
        else if (ch() == QLatin1Char('.'))
            flags = FloatPointType;
        else
            flags = OctalType | FloatPointType;
        break;
    case '.':
        flags = FloatPointType;
        break;
    default:
        flags = FixedPointType | FloatPointType;
        break;
    }

    QChar c;
    while (m_position < m_length) {
        c = ch();
        if (c == QLatin1Char('.'))
            flags = FloatPointType;

        if ((flags & HexType) && !QString(QLatin1String("0123456789abcdefABCDEF")).contains(c))
            break;
        if ((flags & FloatPointType) && !QString(QLatin1String("0123456789eEi")).contains(c))
            break;
        if ((flags & FixedPointType) && !c.isDigit() && c != QLatin1Char('i'))
            break;
        ++m_position;
    }

    return GoToken(m_tokenPosition, m_position - m_tokenPosition, T_NUMBER);
}

GoToken GoLexer::parseString()
{
    const LexerState::MultiLine ml = m_lexerState.multiLine();
    const QChar quoteChar = (ml == LexerState::MultiLineRawString) ? QLatin1Char('`') : QLatin1Char('"');

    QChar c, prev;
    while (m_position < m_length) {
        prev = m_text[m_position - 1];
        c = chpp();
        if (c == quoteChar) {
            bool ended = true;
            if (ml == LexerState::MultiLineInterpretedString)
                ended = (prev != QLatin1Char('\\')); // \" sequence is not end of interpreted string
            if (ended) {
                m_lexerState.setMultiLine(LexerState::NoMultiLine);
                break;
            }
        }
    }

    return GoToken(m_tokenPosition, m_position - m_tokenPosition, T_STRING);
}

void GoLexer::skipWhiteSpace()
{
    QChar c;
    while (m_position < m_length) {
        c = ch();
        if (!c.isSpace())
            break;
        ++m_position;
    }
}

QList<GoToken> GoLexer::tokenize(const QString &text, int prevBlockState)
{
    QList<GoToken> tokens;

    m_text = text.constData();
    m_tokenPosition = m_position = 0;
    m_length = text.length();
    if (prevBlockState < 0)
        prevBlockState = 0;
    m_lexerState.setState(prevBlockState);
    GoToken token;

    while (m_position < m_length) {
        switch (m_lexerState.multiLine()) {
        case LexerState::MultiLineInterpretedString:
        case LexerState::MultiLineRawString:
            token = parseString();
            break;
        case LexerState::MultiLineComment:
            skipWhiteSpace();
            m_tokenPosition = m_position; // Start position should point first non-space character
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

QList<GoToken> GoLexer::tokenize(const QTextBlock &block)
{
    const int prevState = block.previous().isValid() ? block.previous().userState() : -1;
    return tokenize(block.text(), prevState);
}

} // namespace Internal
} // namespace GoEditor
