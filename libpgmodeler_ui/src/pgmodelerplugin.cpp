/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2020 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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
	widget->setWindowTitle(QT_TRANSLATE_NOOP("PgModelerPlugin", "Plugin Information"));

	gridLayout->setHorizontalSpacing(10);
	gridLayout->setVerticalSpacing(6);
	gridLayout->setContentsMargins(6, 6, 6, 6);

	icon_lbl = new QLabel(widget);
	icon_lbl->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	icon_lbl->setMinimumSize(QSize(32, 32));
	icon_lbl->setMaximumSize(QSize(32, 32));
	gridLayout->addWidget(icon_lbl, 0, 0, 2, 1);

	title_lbl = new QLabel(widget);
	title_lbl->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
	font.setPointSize(12);
	font.setBold(true);
	font.setItalic(true);
	font.setWeight(75);
	title_lbl->setFont(font);
	gridLayout->addWidget(title_lbl, 0, 1, 1, 1);

	author_lbl = new QLabel(widget);
	author_lbl->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
	gridLayout->addWidget(author_lbl, 1, 1, 2, 1);

	verticalSpacer = new QSpacerItem(20, 18, QSizePolicy::Minimum, QSizePolicy::Expanding);
	gridLayout->addItem(verticalSpacer, 2, 0, 2, 1);

	version_lbl = new QLabel(widget);
	version_lbl->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
	gridLayout->addWidget(version_lbl, 3, 1, 1, 1);

	description_lbl = new QLabel(widget);
	description_lbl->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
	description_lbl->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
	description_lbl->setWordWrap(true);
	gridLayout->addWidget(description_lbl, 4, 0, 1, 2);

	widget->setLayout(gridLayout);
	widget->setMinimumSize(400, 200);
	plugin_info_frm->setMainWidget(widget);
}

PgModelerPlugin::~PgModelerPlugin()
{
	delete plugin_info_frm;
}

void PgModelerPlugin::initPlugin(MainWindow *main_window)
{
	this->main_window = main_window;
}

QKeySequence PgModelerPlugin::getPluginShortcut()
{
	return QKeySequence();
}

bool PgModelerPlugin::hasMenuAction()
{
	return true;
}

void PgModelerPlugin::configurePluginInfo(const QString &title, const QString &version, const QString &author,
										  const QString &description, const QString &ico_filename)
{
	QPixmap ico;

	title_lbl->setText(title);
	version_lbl->setText(QString(QT_TRANSLATE_NOOP("PgModelerPlugin", "Version: %1")).arg(version));
	author_lbl->setText(QString(QT_TRANSLATE_NOOP("PgModelerPlugin","Author: %1")).arg(author));
	description_lbl->setText(description);

	ico.load(ico_filename);
	icon_lbl->setPixmap(ico);
}
