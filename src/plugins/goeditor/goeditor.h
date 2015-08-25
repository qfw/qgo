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

#ifndef GOEDITOR_H
#define GOEDITOR_H

#include <texteditor/texteditor.h>

namespace GoEditor {
namespace Internal {

class GoEditor : public TextEditor::BaseTextEditor
{
    Q_OBJECT
public:
    GoEditor();
};

class GoEditorFactory : public TextEditor::TextEditorFactory
{
    Q_OBJECT
public:
    GoEditorFactory();
};

class GoEditorWidget : public TextEditor::TextEditorWidget
{
    Q_OBJECT
public:
    GoEditorWidget();
};

} // namespace Internal
} // namespace GoEditor

#endif // #ifndef GOEDITOR_H

