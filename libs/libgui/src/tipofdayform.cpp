/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2025 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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

#include "tipofdayform.h"
#include "guiutilsns.h"
#include <QNetworkReply>
#include <QNetworkRequest>

TipOfDayForm::TipOfDayForm(QWidget *parent) : QWidget(parent)
{
	setupUi(this);
	setWindowIcon(QIcon(GuiUtilsNs::getIconPath("pgmodeler_logo")));

	GuiUtilsNs::configureWidgetFont(tip_title_lbl, GuiUtilsNs::BigFontFactor);

	index_lst->setVisible(false);
	splitter->setSizes({ 1000, 500 });

	connect(index_tb, &QToolButton::clicked, index_lst, &QListWidget::setVisible);

	/* net_manager = new QNetworkAccessManager(this);

	connect(net_manager, &QNetworkAccessManager::finished, this, [this](QNetworkReply *reply){
		if(reply->error() == QNetworkReply::NoError)
		{
			textBrowser->setMarkdown(reply->readAll());
		}
	}); */
}

TipOfDayForm::~TipOfDayForm()
{

}

void TipOfDayForm::showTipOfDay(const QString &tip)
{

}

QString TipOfDayForm::loadTipOfDay(const QString &url)
{

}

void TipOfDayForm::showEvent(QShowEvent *event)
{
	/* if(!event->spontaneous())
	{
		net_manager->get(QNetworkRequest(QUrl("http://localhost:8000/checkupdate?version=1.1.0")));
	} */
}
