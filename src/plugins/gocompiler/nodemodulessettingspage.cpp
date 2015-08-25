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

#include "nodemodulessettingspage.h"
#include "gocompilerconstants.h"

#include <coreplugin/icore.h>
#include <projectexplorer/kitinformation.h>
#include <projectexplorer/kitmanager.h>
#include <projectexplorer/toolchain.h>
#include <utils/hostosinfo.h>

#include <qbs.h>

#include <QtCore/QDir>

#include <QtDebug>

#include <QtWidgets/QFileDialog>

static const char QBS_SEARCH_PATHS_PREFIX[] = "preferences.qbsSearchPaths";

static const char JADE_COMPILER_PATH_KEY[] = "JadeCompilerPath";
static const char JADE_OUTPUT_SUFFIX_KEY[] = "JadeOutputSuffix";

static const char LESS_COMPILER_PATH_KEY[] = "LessCompilerPath";
static const char LESS_COMPRESS_FLAG_KEY[] = "LessCompressFlag";

namespace GoCompiler {
namespace Internal {

/****************************************************************************
 * NodeModulesSettingsWidget
 ***************************************************************************/

NodeModulesSettingsWidget::NodeModulesSettingsWidget(QWidget *parent) :
    QWidget(parent)
{
    m_ui.setupUi(this);
    readSettings();
}

void NodeModulesSettingsWidget::browse(const QString &caption, const QString &filter, QLineEdit *control)
{
    QFileInfo fi(control->text());
    const QString dir = fi.exists() ? fi.path() : QDir::currentPath();
    QString fileName = QFileDialog::getOpenFileName(this, caption, dir, filter);
    if (!fileName.isNull())
        control->setText(fileName);
}

void NodeModulesSettingsWidget::on_jadeCompilerBrowseButton_clicked()
{
    browse(tr("Find JADE"), tr("JADE Compiler (jade*)"), m_ui.jadeCompilerPathEdit);
}

void NodeModulesSettingsWidget::on_lessCompilerBrowseButton_clicked()
{
    browse(tr("Find LESS"), tr("LESS Compiler (lessc*)"), m_ui.lessCompilerPathEdit);
}

void NodeModulesSettingsWidget::readSettings()
{
    QSettings *settings = Core::ICore::settings();
    settings->beginGroup(QLatin1String(Constants::GOCOMPILER_GROUP));

    m_ui.jadeCompilerPathEdit->setText(settings->value(QLatin1String(JADE_COMPILER_PATH_KEY)).toString());
    m_ui.jadeOutputSuffixEdit->setText(settings->value(QLatin1String(JADE_OUTPUT_SUFFIX_KEY), QLatin1String("tpl")).toString());

    m_ui.lessCompilerPathEdit->setText(settings->value(QLatin1String(LESS_COMPILER_PATH_KEY)).toString());
    m_ui.lessCompressCheckBox->setChecked(settings->value(QLatin1String(LESS_COMPRESS_FLAG_KEY), true).toBool());

    settings->endGroup();
}

void NodeModulesSettingsWidget::updateQbsSettings()
{
    qbs::Settings qbsSettings(Core::ICore::userResourcePath());
    foreach (ProjectExplorer::Kit *k, ProjectExplorer::KitManager::kits()) {
        ProjectExplorer::ToolChain *tc = ProjectExplorer::ToolChainKitInformation::toolChain(k);
        if (tc->type() == QLatin1String("go")) {
            const QString profileName = QLatin1String("qtc_") + k->fileSystemFriendlyName();
            const QString profilePrefix = QLatin1String("profiles") + QLatin1Char('.') + profileName + QLatin1Char('.');

            qbsSettings.setValue(profilePrefix + QLatin1String("jade.compilerPath"), m_ui.jadeCompilerPathEdit->text());
            qbsSettings.setValue(profilePrefix + QLatin1String("jade.outputSuffix"), m_ui.jadeOutputSuffixEdit->text());

            qbsSettings.setValue(profilePrefix + QLatin1String("less.compilerPath"), m_ui.lessCompilerPathEdit->text());
            qbsSettings.setValue(profilePrefix + QLatin1String("less.compressFlag"), m_ui.lessCompressCheckBox->isChecked());
        }
    }
}

void NodeModulesSettingsWidget::writeSettings()
{
    QSettings *settings = Core::ICore::settings();
    settings->beginGroup(QLatin1String(Constants::GOCOMPILER_GROUP));

    settings->setValue(QLatin1String(JADE_COMPILER_PATH_KEY), m_ui.jadeCompilerPathEdit->text());
    settings->setValue(QLatin1String(JADE_OUTPUT_SUFFIX_KEY), m_ui.jadeOutputSuffixEdit->text());

    settings->setValue(QLatin1String(LESS_COMPILER_PATH_KEY), m_ui.lessCompilerPathEdit->text());
    settings->setValue(QLatin1String(LESS_COMPRESS_FLAG_KEY), m_ui.lessCompressCheckBox->isChecked());

    settings->endGroup();
}

/****************************************************************************
 * NodeModulesSettingsPage
 ***************************************************************************/

NodeModulesSettingsPage::NodeModulesSettingsPage(QObject *parent) :
    Core::IOptionsPage(parent)
{
    setId(Constants::NODE_MODULES_SETTINGS_ID);
    setDisplayName(tr("Node.js Modules"));
    setCategory(Constants::GOCOMPILER_SETTINGS_CATEGORY);
    setDisplayCategory(QCoreApplication::translate("GoCompiler",
        Constants::GOCOMPILER_SETTINGS_TR_CATEGORY));
    setCategoryIcon(QLatin1String(Constants::GOCOMPILER_SETTINGS_CATEGORY_ICON));
}

QWidget *NodeModulesSettingsPage::widget()
{
    if (!m_widget)
        m_widget = new NodeModulesSettingsWidget;
    return m_widget;
}

void NodeModulesSettingsPage::apply()
{
    if (m_widget) {
        m_widget->writeSettings();
        m_widget->updateQbsSettings();
    }
}

void NodeModulesSettingsPage::finish()
{
    delete m_widget;
    m_widget = 0;
}

} // namespace Internal
} // namespace TiSupport
