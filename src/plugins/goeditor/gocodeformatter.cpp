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

#include "gocodeformatter.h"
#include "golexer.h"

#include <QtDebug>

#include <QtGui/QTextDocument>

using namespace TextEditor;

namespace GoEditor {
namespace Internal {

GoCodeFormatter::GoCodeFormatter(const TextEditor::TabSettings &tabSettings) :
    m_tabSettings(tabSettings)
{

}

void GoCodeFormatter::adjustIndentation(const QTextBlock &block, bool withoutLexer)
{
//    const int lexerState = withoutLexer ? BaseTextDocumentLayout::lexerState(block) : tokenizeBlock(block);
    if (!withoutLexer)
        tokenizeBlock(block);

    if (withoutLexer)
        m_tokens.clear();

    CodeState currentState = state();
    int i;
    switch (currentState.state) {
    case MultilineCommentStart:
    case MultilineCommentCont:
        if (!m_tokens.isEmpty()) {
            m_indentation = column(m_tokens.at(0).start());
            return;
        }
        break;
    }

    if (m_tokens.count()) {
        const int kind = m_tokens.first().kind();
        switch (kind) {
        case T_LPAREN:
            if (currentState.state == ImportDecl)
                m_indentation = currentState.indentation;
            m_padding = 0;
            break;
        case T_RPAREN:
            for (i = 0; state(i).state != TopMost; ++i) {
                if (state(i).state == ImportOpen
                        || state(i).state == TypeOpen
                        || state(i).state == VarOpen
                        || state(i).state == GroupOpen) {
                    m_indentation = state(i).indentation;
                    m_padding = state(i).padding;
                    break;
                }
            }
            break;
        case T_LBRACE:
            switch (currentState.state) {
            case FuncDecl:
            case ForStmt:
            case IfStmt:
                m_indentation = currentState.indentation;
                m_padding = 0;
                break;
            }
            break;
        case T_RBRACE:
            for (i = 0; state(i).state != TopMost; ++i) {
                if (state(i).state == FuncOpen
                        || state(i).state == BlockOpen
                        || state(i).state == ForOpen
                        || state(i).state == IfOpen
                        || state(i).state == InterfaceOpen
                        || state(i).state == StructOpen
                        || state(i).state == SwitchOpen) {
                    m_indentation = state(i).indentation;
                    m_padding = state(i).padding;
                    break;
                }
            }
            break;
        case T_CASE:
        case T_DEFAULT:
            for (int i = 0; state(i).state != TopMost; ++i) {
                if (state(i).state == SwitchStmt) {
                    m_indentation = state(i).indentation;
                    m_padding = state(i).padding;
                    break;
                }
            }
            break;

        case T_COMMENT:
            // unindent the last line of a comment
            if ((currentState.state == MultilineCommentCont
                 || currentState.state == MultilineCommentStart)
                    && (kind == T_COMMENT)
                    && (m_tokens.size() != 1)) {
                if (m_indentation >= m_tabSettings.m_indentSize)
                    m_indentation -= m_tabSettings.m_indentSize;
                else
                    m_indentation = 0;
            }
            break;

        case T_IDENTIFIER:
            if (currentState.state == FuncOpen) {
                if (m_tokens.size() > 1 && m_tokens.at(1).kind() == T_COLON) // label?
                    m_indentation = 0;
            }
            break;

        default:
            break;
        }
    }
}

void GoCodeFormatter::calcIndentation(const QTextBlock &block, int *indentation, int *padding, bool withoutLexer)
{
    restoreCurrentState(block.previous());

    adjustIndentation(block, withoutLexer);

    *indentation = m_indentation;
    *padding = m_padding;
}

int GoCodeFormatter::column(int position) const
{
    if (position > m_line.length())
        position = m_line.length();
    return m_tabSettings.columnAt(m_line, position);
}

void GoCodeFormatter::enter(int newState)
{
    int savedIndentation = m_indentation;
    int savedPadding = m_padding;

    //const CodeState &parentState = state();
    const bool firstToken = (m_index == 0);

    const int tokenPosition = column(m_tk.start());

    switch (newState) {
    case PackageClause:
    case ImportDecl:
    case InterfaceDecl:
    case StructDecl:
    case TypeDecl:
    case ForStmt:
    case IfStmt:
        if (firstToken)
            m_indentation = savedIndentation = tokenPosition;
        savedPadding = 0;
        m_padding = m_tabSettings.m_indentSize;
        break;
    case ImportOpen:
    case ForOpen:
    case FuncOpen:
    case IfOpen:
    case InterfaceOpen:
    case StructOpen:
    case TypeOpen:
    case BlockOpen:
    case GroupOpen:
        savedPadding = 0;
        m_padding = 0;
        m_indentation += m_tabSettings.m_indentSize;
        break;
    case SwitchStmt:
        if (firstToken)
            m_indentation = savedIndentation = tokenPosition;
        m_padding = 0;
        break;
    case CaseOpen:
        m_padding = m_tabSettings.m_indentSize;
        break;
    case MultilineCommentStart:
        m_indentation = tokenPosition + 2;
        break;
    case MultilineCommentCont:
        m_indentation = tokenPosition;
        break;

    }

    m_indentation = qMax(0, m_indentation);
    savedIndentation = qMax(0, savedIndentation);
    m_padding = qMax(0, m_padding);
    savedPadding = qMax(0, savedPadding);

    CodeState s(newState, savedIndentation, savedPadding);
    m_currentState.push(s);
    m_newStates.push(s);
}

QStack<GoCodeFormatter::CodeState> GoCodeFormatter::initialState()
{
    static QStack<CodeState> initialState; // static for better performance
    if (initialState.isEmpty())
        initialState.push(CodeState(TopMost, 0, 0));

    return initialState;
}

void GoCodeFormatter::leave()
{
    if (m_currentState.top().state == TopMost)
        return;

    if (!m_newStates.isEmpty())
        m_newStates.pop();

    // restore indent depth
    CodeState lastState = m_currentState.pop();
    m_indentation = lastState.indentation;
    m_padding = lastState.padding;
}

bool GoCodeFormatter::loadBlockData(const QTextBlock &block, BlockData *data) const
{
    TextBlockUserData *userData = TextDocumentLayout::testUserData(block);
    if (!userData)
        return false;
    GoCodeFormatterData *goData = static_cast<GoCodeFormatterData *>(userData->codeFormatterData());
    if (!goData)
        return false;

    *data = goData->blockData();
    return true;
}

void GoCodeFormatter::recalculateStateAfter(const QTextBlock &block)
{
    restoreCurrentState(block.previous());

    const LexerState lexerState(tokenizeBlock(block));

    m_newStates.clear();

    int currentState;
    for (m_index = 0; m_index < m_tokens.count(); ++m_index) {
        currentState = m_currentState.top().state;
        m_tk = m_tokens.at(m_index);
        switch (currentState) {
        case TopMost:
            tryDeclaration();
            break;
        case PackageClause:
            switch (m_tk.kind()) {
            case T_IDENTIFIER:  leave(); break;
            default:            break;
            }
            break;
        case ImportDecl:
            switch (m_tk.kind()) {
            case T_LPAREN:      enter(ImportOpen); break;
            case T_STRING:      leave(); break;
            default:            break;
            }
            break;
        case ImportOpen:
            switch (m_tk.kind()) {
            case T_RPAREN:      leave(); leave(); break;
            default:            break;
            }
            break;
        case InterfaceDecl:
            switch (m_tk.kind()) {
            case T_LBRACE:      enter(InterfaceOpen); break;
            default:            break;
            }
            break;
        case InterfaceOpen:
            switch (m_tk.kind()) {
            case T_RBRACE:      leave(); leave(); break;
            default:            break;
            }
            break;
        case StructDecl:
            switch (m_tk.kind()) {
            case T_LBRACE:      enter(StructOpen); break;
            default:            break;
            }
            break;
        case StructOpen:
            switch (m_tk.kind()) {
            case T_RBRACE:      leave(); leave(); break;
            default:            break;
            }
            break;
        case TypeDecl:
            switch (m_tk.kind()) {
            case T_LPAREN:      enter(TypeOpen); break;
            case T_INTERFACE:   leave(); enter(InterfaceDecl); break;
            case T_STRUCT:      leave(); enter(StructDecl); break;
            default:            leave(); break;
            }
            break;
        case TypeOpen:
            switch (m_tk.kind()) {
            case T_INTERFACE:   leave(); enter(InterfaceDecl); break;
            case T_STRUCT:      enter(StructDecl); break;
            case T_RPAREN:      leave(); leave(); break;
            default:            break;
            }
            break;
        case IdentDecl:
            leave(); break; // TODO: implement
            /*
            switch (m_tk.kind()) {
            case T_LPAREN:      enter(IdentArgs); break;
            case T_LBRACE:      enter(IdentFields); break;
            default:            leave(); break;
            }
            break;
        case IdentArgs:
            switch (m_tk.kind()) {
            case T_RPAREN:      leave(); break;
            default:            break;
            }
            break;
        case IdentFields:
            switch (m_tk.kind()) {
            case T_RBRACE:      leave(); break;
            default:            break;
            }
            break;*/
        case ConstDecl:
            leave(); break; // TODO: implement
        case VarDecl:
            leave(); break; // TODO: implement
            /*
            switch (m_tk.kind()) {
            case T_LPAREN:      enter(VarOpen); break;
            case T_EQUAL:       enter(VarExpr); break;
            default:            break;
            }
            break;
        case VarOpen:
            switch (m_tk.kind()) {
            case T_RPAREN:      leave(); leave(); break;
            default:            if (!tryDeclaration()) leave(); break;
            }
            break;
        case VarExpr:
            if (tryDeclaration())
                break;
            else
                leave();
            break;*/
        case FuncDecl:
            switch (m_tk.kind()) {
            case T_LBRACE:      enter(FuncOpen); break;
            case T_LPAREN:      enter(FuncParams); break;
            default:            break;
            }
            break;
        case FuncOpen:
            switch (m_tk.kind()) {
            case T_RBRACE:      leave(); leave(); break;
            default:            tryStatement(); break;
            }
            break;
        case FuncParams:
            switch (m_tk.kind()) {
            case T_RPAREN:      leave(); break;
            default:            break;
            }
            break;
        case BlockOpen:
            switch (m_tk.kind()) {
            case T_RBRACE:      leave(); break;
            default:            tryStatement(); break;
            }
            break;
        case GroupOpen:
            switch (m_tk.kind()) {
            case T_RPAREN:      leave(); break;
            default:            tryStatement(); break;
            }
            break;
        case ForStmt:
            switch (m_tk.kind()) {
            case T_LBRACE:      enter(ForOpen); break;
            default:            break;
            }
            break;
        case ForOpen:
            // TODO: parse arguments
            switch (m_tk.kind()) {
            case T_RBRACE:      leave(); leave(); break;
            default:            tryStatement(); break;
            }
            break;
        case IfStmt:
            switch (m_tk.kind()) {
            case T_LBRACE:      enter(IfOpen); break;
            default:            break;
            }
            break;
        case IfOpen:
            // TODO: parse arguments
            switch (m_tk.kind()) {
            case T_RBRACE:      leave(); leave(); break;
            default:            tryStatement(); break;
            }
            break;
        case SwitchStmt:
            switch (m_tk.kind()) {
            case T_LBRACE:      enter(SwitchOpen); break;
            default:            break;
            }
            break;
        case SwitchOpen:
            switch (m_tk.kind()) {
            case T_RBRACE:      leave(); leave(); break;
            case T_DEFAULT:
            case T_CASE:        enter(CaseClause); break;
            default:            break;
            }
            break;
        case CaseClause:
            switch (m_tk.kind()) {
            case T_COLON:       leave(); enter(CaseOpen); break;
            default:            break;
            }
            break;
        case CaseOpen:
            switch (m_tk.kind()) {
            case T_DEFAULT:
            case T_CASE:        leave(); enter(CaseClause); break;
            case T_RBRACE:      leave(); leave(); leave(); break;
            default:            tryStatement(); break;
            }
            break;
        case Label:
            switch (m_tk.kind()) {
            case T_COLON:       leave(); break;
            default:            leave(); continue; // shouldn't happen
            } break;
        case MultilineCommentStart:
        case MultilineCommentCont:
            if (m_tk.kind() != T_COMMENT) {
                leave();
                continue;
            } else if (m_index == m_tokens.size() - 1
                    && lexerState.multiLine() == LexerState::NoMultiLine) {
                leave();
            } else if (m_index == 0 && m_tk.is(T_COMMENT)) {
                // to allow enter/leave to update the indentDepth
                leave();
                enter(MultilineCommentCont);
            }
            break;
        default:
            break;
        }
    }

    currentState = m_currentState.top().state;
    if (currentState != MultilineCommentStart
            && currentState != MultilineCommentCont
            && (lexerState.multiLine() == LexerState::MultiLineComment)) {
        enter(MultilineCommentStart);
    }

    saveCurrentState(block);
}

void GoCodeFormatter::restoreCurrentState(const QTextBlock &block)
{
    if (block.isValid()) {
        BlockData blockData;
        if (loadBlockData(block, &blockData)) {
            m_indentation = blockData.indentation;
            m_padding = blockData.padding;
            m_currentState = blockData.endState;
            m_beginState = m_currentState;
            return;
        }
    }

    m_currentState = initialState();
    m_beginState = m_currentState;
    m_indentation = 0;
    m_padding = 0;
}

void GoCodeFormatter::saveBlockData(QTextBlock *block, const BlockData &data) const
{
    TextBlockUserData *userData = TextDocumentLayout::userData(*block);
    GoCodeFormatterData *goData = static_cast<GoCodeFormatterData *>(userData->codeFormatterData());
    if (!goData) {
        goData = new GoCodeFormatterData;
        userData->setCodeFormatterData(goData);
    }
    goData->setBlockData(data);
}

void GoCodeFormatter::saveCurrentState(const QTextBlock &block)
{
    if (!block.isValid())
        return;

    BlockData blockData;
    blockData.revision = block.revision();
    blockData.beginState = m_beginState;
    blockData.endState = m_currentState;
    blockData.indentation = m_indentation;
    blockData.padding = m_padding;

    QTextBlock saveableBlock(block);
    saveBlockData(&saveableBlock, blockData);
}

GoCodeFormatter::CodeState GoCodeFormatter::state(int belowTop) const
{
    if (belowTop < m_currentState.size())
        return m_currentState.at(m_currentState.size() - 1 - belowTop);
    else
        return CodeState();
}

int GoCodeFormatter::tokenizeBlock(const QTextBlock &block)
{
    int previousState = TextDocumentLayout::lexerState(block.previous());
    if (block.blockNumber() == 0)
        previousState = 0;

    m_line = block.text();
    GoLexer *lexer = GoLexer::instance();
    m_tokens = lexer->tokenize(m_line, previousState);
    int currentState = lexer->lexerState().state();

    TextEditor::TextDocumentLayout::setLexerState(block, currentState);
    return currentState;
}

bool GoCodeFormatter::tryDeclaration()
{
    switch (m_tk.kind()) {
    case T_PACKAGE:
        enter(PackageClause);
        return true;
    case T_IMPORT:
        enter(ImportDecl);
        return true;
    case T_FUNC:
        enter(FuncDecl);
        return true;
    case T_STRUCT:
        enter(StructDecl);
        return true;
    case T_TYPE:
        enter(TypeDecl);
        return true;
    case T_LBRACE: // TODO: process less general cases
        enter(BlockOpen);
        return true;
    case T_LPAREN: // TODO: process less general cases
        enter(GroupOpen);
        return true;
    case T_IDENTIFIER:
        if (m_index == 0) {
            if (m_tokens.size() > 1 && m_tokens.at(1).kind() == T_COLON) {
                enter(Label);
                return true;
            }
        }
        break;

/*
    case T_CONST:
        enter(ConstDecl);
        return true;
    case T_VAR:
        enter(VarDecl);
        return true;
*/
    default:
        break;
    }
    return false;
}

bool GoCodeFormatter::tryStatement()
{
    if (tryDeclaration())
        return true;
    switch (m_tk.kind()) {
    case T_CASE:
    case T_DEFAULT:
        enter(CaseClause);
        return true;
    case T_IF:
        enter(IfStmt);
        return true;
    case T_FOR:
        enter(ForStmt);
        return true;
    case T_SELECT:
    case T_SWITCH:
        enter(SwitchStmt);
        return true;
    default:
        return false;
    }
}

void GoCodeFormatter::updateStateUntil(const QTextBlock &endBlock)
{
    QStack<CodeState> previousState = initialState();
    QTextBlock it = endBlock.document()->firstBlock();

    // find the first block that needs recalculation
    while (it.isValid() && it != endBlock) {
        BlockData blockData;
        if (!loadBlockData(it, &blockData))
            break;
        if (blockData.revision != it.revision())
            break;
        if (previousState.isEmpty() || blockData.beginState.isEmpty()
                || previousState != blockData.beginState)
            break;
        if (TextEditor::TextDocumentLayout::lexerState(endBlock) == -1)
            break;

        previousState = blockData.endState;
        it = it.next();
    }

    if (it == endBlock) // No need to recalculate
        return;

    // update everthing until endBlock
    while (it.isValid() && it != endBlock) {
        recalculateStateAfter(it);
        it = it.next();
    }

    // invalidate everything below by marking the state in endBlock as invalid
    if (it.isValid()) {
        BlockData invalidBlockData;
        saveBlockData(&it, invalidBlockData);
    }
}

} // namespace Internal
} // namespace GoEditor
