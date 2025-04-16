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
#include "messagebox.h"
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

TipOfDayForm::TipOfDayForm(QWidget *parent) : QWidget(parent)
{
	setupUi(this);
	setWindowIcon(QIcon(GuiUtilsNs::getIconPath("pgmodeler_logo")));

	GuiUtilsNs::configureWidgetFont(tip_title_lbl, GuiUtilsNs::BigFontFactor);

	load_tip = false;
	index_trw->setVisible(false);
	splitter->setSizes({ 1000, 500 });

	net_manager = new QNetworkAccessManager(this);

	connect(index_tb, &QToolButton::clicked, index_trw, &QListWidget::setVisible);
	//connect(index_trw, &QTreeWidget::itemClicked, this, &TipOfDayForm::handleItemSelected);
	connect(index_trw, &QTreeWidget::currentItemChanged, this, &TipOfDayForm::handleItemSelected);

	connect(net_manager, &QNetworkAccessManager::finished, this, [this](QNetworkReply *reply){
		if(load_tip)
			loadTipOfDay(reply);
		else
			loadIndex(reply);
	});

}

void TipOfDayForm::loadIndex(QNetworkReply *reply)
{
	if(reply->error() != QNetworkReply::NoError)
		Messagebox::error(tr("Failed to load index! %1").arg(reply->errorString()));
	else
	{
		unsigned http_status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toUInt();

		if(http_status == 200)
		{
			QJsonDocument json_doc = QJsonDocument::fromJson(reply->readAll());
			QJsonObject json_obj = json_doc.object(), tip_obj;
			QJsonArray tips;

			QTreeWidgetItem *root = nullptr, *item = nullptr;

			index_trw->clear();

			for(auto &key : json_obj.keys())
			{
				root = new QTreeWidgetItem;
				root->setText(0, key);
				root->setData(1, Qt::UserRole, -1);
				index_trw->addTopLevelItem(root);
				tips = json_obj.value(key).toArray();

				for(const auto &tip : std::as_const(tips))
				{
					tip_obj = tip.toObject();
					item = new QTreeWidgetItem(root);
					item->setText(0, tip_obj.value("title").toString());
					item->setData(1, Qt::UserRole, tip_obj.value("id").toInt());
					index_trw->addTopLevelItem(item);
				}
			}
		}

		reply->deleteLater();
		index_trw->expandAll();
		qApp->restoreOverrideCursor();
	}
}

void TipOfDayForm::loadTipOfDay(QNetworkReply *reply)
{
	load_tip = false;
	reply->deleteLater();
	qApp->restoreOverrideCursor();
}

void TipOfDayForm::handleItemSelected(QTreeWidgetItem *item)
{
	int tip_id = item->data(1, Qt::UserRole).toInt();

	if(tip_id < 0)
		return;

	load_tip = true;
	qApp->setOverrideCursor(Qt::WaitCursor);
	net_manager->get(QNetworkRequest(QUrl(QString("http://localhost:8000/tipofday/%1").arg(tip_id))));
}

void TipOfDayForm::showEvent(QShowEvent *event)
{
	if(!event->spontaneous())
	{
		qApp->setOverrideCursor(Qt::WaitCursor);
		net_manager->get(QNetworkRequest(QUrl("http://localhost:8000/tipofday")));
	}
}
