/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2024 - Raphael Araújo e Silva <raphael@pgmodeler.io>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation version 3.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# The complete text of GPLv3 is at LICENSE file on source code root directory.
# Also, you can get the complete GNU General Public License at <http://www.gnu.org/licenses/>
*/

#include "pgmodelerplugin.h"
#include "guiutilsns.h"

PgModelerPlugin::PgModelerPlugin()
{
	QGridLayout *gridLayout=nullptr;
	QSpacerItem *verticalSpacer=nullptr;
	QFont font;
	QWidget *widget=nullptr;

	main_window = nullptr;
	plugin_info_frm=new BaseForm;
	gridLayout=new QGridLayout;

	widget=new QWidget;
	widget->setWindowTitle(QT_TRANSLATE_NOOP("PgModelerPlugin", "Plug-in information"));

	gridLayout->setHorizontalSpacing(GuiUtilsNs::LtSpacing);
	gridLayout->setVerticalSpacing(GuiUtilsNs::LtSpacing);
	gridLayout->setContentsMargins(GuiUtilsNs::LtMargin, GuiUtilsNs::LtMargin, GuiUtilsNs::LtMargin, GuiUtilsNs::LtMargin);

	icon_lbl = new QLabel(widget);
	icon_lbl->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	icon_lbl->setMinimumSize(QSize(64, 64));
	icon_lbl->setMaximumSize(QSize(64, 64));
	icon_lbl->setScaledContents(true);
	gridLayout->addWidget(icon_lbl, 0, 0, 2, 1);

	title_lbl = new QLabel(widget);
	title_lbl->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
	font.setPointSize(12);
	font.setBold(true);
	font.setItalic(true);
	title_lbl->setFont(font);
	gridLayout->addWidget(title_lbl, 0, 1, 1, 1);

	author_lbl = new QLabel(widget);
	author_lbl->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
	author_lbl->setTextInteractionFlags(Qt::TextBrowserInteraction);
	gridLayout->addWidget(author_lbl, 1, 1, 2, 1);

	verticalSpacer = new QSpacerItem(20, 18, QSizePolicy::Minimum, QSizePolicy::Expanding);
	gridLayout->addItem(verticalSpacer, 2, 0, 2, 1);

	version_lbl = new QLabel(widget);
	version_lbl->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
	version_lbl->setTextInteractionFlags(Qt::TextBrowserInteraction);
	gridLayout->addWidget(version_lbl, 3, 1, 1, 1);

	description_lbl = new QLabel(widget);
	description_lbl->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
	description_lbl->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
	description_lbl->setWordWrap(true);
	description_lbl->setTextInteractionFlags(Qt::TextBrowserInteraction);
	gridLayout->addWidget(description_lbl, 4, 0, 1, 2);

	widget->setLayout(gridLayout);
	widget->setMinimumSize(480, 240);
	plugin_info_frm->setMainWidget(widget);
}

PgModelerPlugin::~PgModelerPlugin()
{
	delete plugin_info_frm;
}

void PgModelerPlugin::initPlugin(MainWindow *main_window)
{
	this->main_window = main_window;

	configurePluginInfo(getPluginTitle(),
						getPluginVersion(),
						getPluginAuthor(),
						getPluginDescription());
}

void PgModelerPlugin::postInitPlugin()
{
	if(!main_window)
	{
		throw Exception(QT_TRANSLATE_NOOP("PgModelerPlugin", "Trying to perform a post initialization on a plug-in without initializing the application's main window!"),
										ErrorCode::Custom, __PRETTY_FUNCTION__, __FILE__, __LINE__);
	}
}

void PgModelerPlugin::showPluginInfo() const
{
	plugin_info_frm->show();
}

void PgModelerPlugin::setLibraryName(const QString &lib)
{
	libname = lib;
}

void PgModelerPlugin::setPluginName(const QString &name)
{
	plugin_name = name;
}

QString PgModelerPlugin::getLibraryName() const
{
	return libname;
}

QString PgModelerPlugin::getPluginName() const
{
	return plugin_name;
}

QString PgModelerPlugin::getPluginIcon(const QString &icon_name) const
{
	return QString(":/%1/%2.png").arg(plugin_name, icon_name);
}

QString PgModelerPlugin::getPluginFilePath(const QString &root_path, const QString &subdir, const QString &filename) const
{
	QString file_pth = root_path + GlobalAttributes::DirSeparator + getPluginName();

	if(!subdir.isEmpty())
		file_pth += GlobalAttributes::DirSeparator + subdir;

	if(!filename.isEmpty())
		file_pth += GlobalAttributes::DirSeparator + filename;

	return file_pth;
}

QString PgModelerPlugin::getPluginFilePath(const QString &subdir, const QString &filename) const
{
	return getPluginFilePath(GlobalAttributes::getConfigurationsPath() +
													 GlobalAttributes::DirSeparator +
													 GlobalAttributes::PluginsDir, subdir, filename);
}

QString PgModelerPlugin::getTmplPluginFilePath(const QString &subdir, const QString &filename) const
{
	return getPluginFilePath(GlobalAttributes::getPluginsPath(), subdir, filename);
}

void PgModelerPlugin::configurePluginInfo(const QString &title, const QString &version, const QString &author, const QString &description)
{
	title_lbl->setText(title);
	version_lbl->setText(QString(QT_TRANSLATE_NOOP("PgModelerPlugin", "<strong>Version:</strong> %1")).arg(version));
	author_lbl->setText(QString(QT_TRANSLATE_NOOP("PgModelerPlugin","<strong>Author:</strong> %1")).arg(author));
	description_lbl->setText(description);
	icon_lbl->setPixmap(QPixmap(getPluginIcon(plugin_name)));
}
