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
	connect(index_trw, &QTreeWidget::currentItemChanged, this, &TipOfDayForm::handleItemSelected);

	connect(next_tb, &QToolButton::clicked, this, [this](){
		navigateToItem(false);
	});

	connect(previous_tb, &QToolButton::clicked, this, [this](){
		navigateToItem(true);
	});

	connect(net_manager, &QNetworkAccessManager::finished, this, [this](QNetworkReply *reply){
		if(load_tip)
			loadTipOfDay(reply);
		else
			loadIndex(reply);
	});
}

void TipOfDayForm::navigateToItem(bool prev_item)
{
	int inc = prev_item ? -1 : 1;

	if(!index_trw->currentItem() && index_trw->topLevelItemCount() > 0)
		index_trw->setCurrentItem(index_trw->topLevelItem(prev_item ? index_trw->topLevelItemCount() - 1 : 0));

	QTreeWidgetItemIterator tree_itr(index_trw->currentItem(), QTreeWidgetItemIterator::Selectable);
	tree_itr += inc;

	if(*tree_itr)
		index_trw->setCurrentItem(*tree_itr);
	else
	{
		qDebug() << "no item!";
		index_trw->setCurrentItem(index_trw->topLevelItem(prev_item ? index_trw->topLevelItemCount() - 1 : 0));
	}
}

void TipOfDayForm::loadIndex(QNetworkReply *reply)
{
	QJsonObject json_obj = getJsonObject(reply);

	if(json_obj.isEmpty())
		Messagebox::error(tr("Failed to load index! %1").arg(reply->errorString()));
	else
	{
		QJsonObject tip_obj;
		QJsonArray tips;
		QTreeWidgetItem *root = nullptr, *item = nullptr;

		index_trw->clear();
		index_trw->setSortingEnabled(false);

		for(auto &key : json_obj.keys())
		{
			root = new QTreeWidgetItem;
			root->setText(0, key);
			root->setData(1, Qt::UserRole, -1);
			root->setFlags(Qt::ItemIsEnabled);

			index_trw->addTopLevelItem(root);
			tips = json_obj.value(key).toArray();

			for(const auto &tip : std::as_const(tips))
			{
				tip_obj = tip.toObject();
				item = new QTreeWidgetItem(root);
				item->setText(0, tip_obj.value("title").toString());
				item->setData(1, Qt::UserRole, tip_obj.value("id").toInt());
				item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
				index_trw->addTopLevelItem(item);
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
	QJsonObject json_obj = getJsonObject(reply);

	if(json_obj.isEmpty())
		Messagebox::error(tr("Failed to load tip of the day! %1").arg(reply->errorString()));
	else
	{
		int tip_id = json_obj.value("id").toInt();
		QString text = json_obj.value("text").toString(),
				title = json_obj.value("title").toString();

		cached_tips[tip_id].append(title);
		cached_tips[tip_id].append(text);
		showTipOfDay(title, text);
	}

	reply->deleteLater();
	qApp->restoreOverrideCursor();
}

void TipOfDayForm::showTipOfDay(const QString &title, const QString &text)
{
	tip_txt->document()->setDefaultStyleSheet("img { max-width: 70%; }");
	tip_title_lbl->setText(title);
	tip_txt->setHtml(text);
}

void TipOfDayForm::handleItemSelected(QTreeWidgetItem *item)
{
	int tip_id = item ? item->data(1, Qt::UserRole).toInt() : -1;

	if(tip_id < 0)
		return;

	if(cached_tips.contains(tip_id))
		showTipOfDay(cached_tips[tip_id].at(0), cached_tips[tip_id].at(1));
	else
	{
		load_tip = true;
		qApp->setOverrideCursor(Qt::WaitCursor);
		net_manager->get(QNetworkRequest(QUrl(QString("http://localhost:8000/tipofday/%1").arg(tip_id))));
	}
}

QJsonObject TipOfDayForm::getJsonObject(QNetworkReply *reply)
{
	unsigned http_status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toUInt();

	if(reply->error() != QNetworkReply::NoError || http_status != 200)
		return QJsonObject();

	QJsonDocument json_doc = QJsonDocument::fromJson(reply->readAll());
	return json_doc.object();
}

void TipOfDayForm::showEvent(QShowEvent *event)
{
	/* We build the index only when it was not previously populated
	 * This will avoid repetitive http requests */
	if(!event->spontaneous() && index_trw->topLevelItemCount() == 0)
	{
		qApp->setOverrideCursor(Qt::WaitCursor);
		net_manager->get(QNetworkRequest(QUrl("http://localhost:8000/tipofday")));
	}
}
