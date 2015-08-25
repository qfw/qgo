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

#ifndef GOTOKEN_H
#define GOTOKEN_H

#include <texteditor/texteditorconstants.h>

#include <QtCore/QString>

namespace GoEditor {
namespace Internal {

enum TokenKind {
    T_INVALID = 0,
    T_IDENTIFIER,
    T_NUMBER,

    T_WHITESPACE,   // Spaces, tabs, returns
    T_COMMENT,      // Single line (//) or multiline (/* */) comment
    T_RUNE,         // 'rune'
    T_STRING,       // "interpreted string" or `raw string`

    T_KEYWORD,
    T_CASE,         // "case" (after "switch" or "select")
    T_CONST,        // "const" declaration
    T_DEFAULT,      // "default" (after "switch" or "select")
    T_FOR,          // "for" statement
    T_FUNC,         // "func" declaration
    T_IF,           // "if" statement
    T_IMPORT,       // "import" statement
    T_INTERFACE,    // "interface" declaration
    T_PACKAGE,      // "package" clause
    T_SELECT,       // "select" statement
    T_STRUCT,       // "struct" declaration
    T_SWITCH,       // "switch" statement
    T_TYPE,         // "type" declaration
    T_VAR,          // "var" declaration
    T_LAST_KEYWORD,

    T_PRIMITIVE,
    T_LAST_PRIMITIVE,

    T_OPERATOR,
    T_LAST_OPERATOR,

    T_OPERATION,
    // Math operations
    T_PLUS,                     // +
    T_PLUS_EQUAL,               // +=
    T_MINUS,                    // -
    T_MINUS_EQUAL,              // -=
    T_ASTERISK,                 // *
    T_ASTERISK_EQUAL,           // *=
    T_SLASH,                    // /
    T_SLASH_EQUAL,              // /=
    T_PERCENT,                  // %
    T_PERCENT_EQUAL,            // %=
    // Bitwise operations
    T_AMPER,                    // &
    T_AMPER_EQUAL,              // &=
    T_PIPE,                     // |
    T_PIPE_EQUAL,               // |=
    T_CARET,                    // ^
    T_CARET_EQUAL,              // ^=
    T_LESS_LESS,                // <<
    T_LESS_LESS_EQUAL,          // <<=
    T_GREATER_GREATER,          // >>
    T_GREATER_GREATER_EQUAL,    // >>=
    T_AMPER_CARET,              // &^
    T_AMPER_CARET_EQUAL,        // &^=
    // Misc operations
    T_AMPER_AMPER,              // &&
    T_PIPE_PIPE,                // ||
    T_LARROW,                   // <-
    T_RARROW,                   // ->
    T_PLUS_PLUS,                // ++
    T_MINUS_MINUS,              // --
    T_LESS,                     // <
    T_LESS_EQUAL,               // <=
    T_GREATER,                  // >
    T_GREATER_EQUAL,            // >=
    T_EQUAL,                    // =
    T_EQUAL_EQUAL,              // ==
    T_EXCLAM,                   // !
    T_EXCLAM_EQUAL,             // !=
    T_COLON,                    // :
    T_COLON_EQUAL,              // :=
    T_DOT,                      // .
    T_DOT_DOT_DOT,              // ...
    T_SEMICOLON,                // ;
    T_COMMA,                    // ','
    T_LAST_OPERATION,

    T_PARENTHESIS,
    T_LBRACE,                   // '{'
    T_RBRACE,                   // '}'
    T_LBRACKET,                 // '['
    T_RBRACKET,                 // ']'
    T_LPAREN,                   // '('
    T_RPAREN,                   // ')'
    T_LAST_PARENTHESIS,

};

class GoToken
{
public:
    GoToken(int start = 0, int count = 0, TokenKind kind = T_INVALID, const QChar *text = 0);

    bool is(TokenKind kind) const { return (m_kind == kind); }
    bool isValid() const { return (m_kind != T_INVALID); }
    bool isKeyword() const { return (m_kind >= T_KEYWORD) && (m_kind < T_LAST_KEYWORD); }
    bool isPrimitive() const { return (m_kind >= T_PRIMITIVE) && (m_kind < T_LAST_PRIMITIVE); }
    bool isOperator() const { return (m_kind >= T_OPERATOR) && (m_kind < T_LAST_OPERATOR); }
    bool isOperation() const { return (m_kind >= T_OPERATION) && (m_kind < T_LAST_OPERATION); }
    bool isParenthesis() const { return (m_kind >= T_PARENTHESIS) && (m_kind < T_LAST_PARENTHESIS); }

    int start() const { return m_start; }
    int count() const { return m_count; }
    int end() const { return (m_start + m_count); }

    TokenKind kind() const { return m_kind; }
    const QString &text() const { return m_text; }
    QChar ch() const { return m_text.length() ? m_text.at(0) : QLatin1Char('\0'); }

private:
    void parseKeyword();
    void parseParenthesis();

private:
    int m_start;
    int m_count;
    TokenKind m_kind;
    QString m_text;
};

} // namespace Internal
} // namespace GoEditor

#endif // #ifndef GOTOKEN_H

