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

#include "gotoken.h"

#include <QtCore/QHash>
#include <QtCore/QVector>

#include <QtDebug>

namespace GoEditor {
namespace Internal {

GoToken::GoToken(int start, int count, TokenKind kind, const QChar *text) :
    m_start(start),
    m_count(count),
    m_kind(kind)
{
    if (text) {
        m_text = QString(&text[m_start], m_count);
        switch (kind) {
        case T_KEYWORD:
            parseKeyword();
            break;
        case T_PARENTHESIS:
            parseParenthesis();
            break;
        default:
            break;
        }
    }
}

void GoToken::parseKeyword()
{
    switch (m_text.length()) {
    case 2:
        switch (m_text.at(0).toLatin1()) {
        case 'i':
            if (m_text == QLatin1String("if"))
                m_kind = T_IF;
            break;
        }
        break;
    case 3:
        switch (m_text.at(0).toLatin1()) {
        case 'f':
            if (m_text == QLatin1String("for"))
                m_kind = T_FOR;
            break;
        case 'v':

            if (m_text == QLatin1String("var"))
                m_kind = T_VAR;
            break;
        }
        break;
    case 4:
        switch (m_text.at(0).toLatin1()) {
        case 'c':
            if (m_text == QLatin1String("case"))
                m_kind = T_CASE;
            break;
        case 'f':
            if (m_text == QLatin1String("func"))
                m_kind = T_FUNC;
            break;
        case 't':
            if (m_text == QLatin1String("type"))
                m_kind = T_TYPE;
            break;
        }
        break;
    case 5:
        switch (m_text.at(0).toLatin1()) {
        case 'c':
            if (m_text == QLatin1String("const"))
                m_kind = T_CONST;
            break;
        }
        break;
    case 6:
        switch (m_text.at(0).toLatin1()) {
        case 'i':
            if (m_text == QLatin1String("import"))
                m_kind = T_IMPORT;
            break;
        case 's':
            if (m_text == QLatin1String("select"))
                m_kind = T_SELECT;
            else if (m_text == QLatin1String("struct"))
                m_kind = T_STRUCT;
            else if (m_text == QLatin1String("switch"))
                m_kind = T_SWITCH;
            break;
        }
        break;
    case 7:
        switch (m_text.at(0).toLatin1()) {
        case 'd':
            if (m_text == QLatin1String("default"))
                m_kind = T_DEFAULT;
            break;
        case 'p':
            if (m_text == QLatin1String("package"))
                m_kind = T_PACKAGE;
            break;
        }
        break;
    case 9:
        switch (m_text.at(0).toLatin1()) {
        case 'i':
            if (m_text == QLatin1String("interface"))
                m_kind = T_INTERFACE;
            break;
        }
        break;
    }
}

void GoToken::parseParenthesis()
{
    switch (m_text.at(0).toLatin1()) {
    case '{':
        m_kind = T_LBRACE;
        break;
    case '}':
        m_kind = T_RBRACE;
        break;
    case '[':
        m_kind = T_LBRACKET;
        break;
    case ']':
        m_kind = T_RBRACKET;
        break;
    case '(':
        m_kind = T_LPAREN;
        break;
    case ')':
        m_kind = T_RPAREN;
        break;
    }
}

} // namespace Internal
} // namespace GoEditor
