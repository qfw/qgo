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

#ifndef NODEMODULESSETTINGSPAGE_H
#define NODEMODULESSETTINGSPAGE_H

#include <coreplugin/dialogs/ioptionspage.h>
#include "ui_nodemodulessettingspage.h"

#include <QPointer>

namespace GoCompiler {
namespace Internal {

class NodeModulesSettingsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NodeModulesSettingsWidget(QWidget *parent = 0);

    void readSettings();
    void writeSettings();

    void updateQbsSettings();

private Q_SLOTS:
    void on_jadeCompilerBrowseButton_clicked();
    void on_lessCompilerBrowseButton_clicked();

private:
    void browse(const QString &caption, const QString &filter, QLineEdit *control);

private:
    Ui::NodeModulesSettingsPage m_ui;
};

class NodeModulesSettingsPage : public Core::IOptionsPage
{
    Q_OBJECT
public:
    NodeModulesSettingsPage(QObject *parent = 0);

    QWidget *widget();
    void apply();
    void finish();

private:
    QPointer<NodeModulesSettingsWidget> m_widget;
};

} // namespace Internal
} // namespace GoCompiler

#endif // #ifndef NODEMODULESSETTINGSPAGE_H
