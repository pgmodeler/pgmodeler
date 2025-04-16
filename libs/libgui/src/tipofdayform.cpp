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
#include <QRandomGenerator>

TipOfDayForm::TipOfDayForm(QWidget *parent) : QWidget(parent)
{
	setupUi(this);
	setWindowIcon(QIcon(GuiUtilsNs::getIconPath("pgmodeler_logo")));

	GuiUtilsNs::configureWidgetFont(tip_title_lbl, GuiUtilsNs::BigFontFactor);

	load_tip = false;
	index_trw->setVisible(false);
	splitter->setSizes({ 1000, 500 });

	net_manager = new QNetworkAccessManager(this);

	connect(close_tb, &QToolButton::clicked, this, &TipOfDayForm::close);
	connect(index_tb, &QToolButton::clicked, index_trw, &QListWidget::setVisible);
	connect(index_trw, &QTreeWidget::currentItemChanged, this, &TipOfDayForm::handleItemSelected);

	connect(next_tb, &QToolButton::clicked, this, [this](){
		navigateToItem(false);
	});

	connect(previous_tb, &QToolButton::clicked, this, [this](){
		navigateToItem(true);
	});

	connect(random_tb, &QToolButton::clicked, this, [this](){
		static QRandomGenerator rndgen = QRandomGenerator::securelySeeded();
		static QList<QTreeWidgetItem *> list = index_trw->findItems("*", Qt::MatchWildcard | Qt::MatchRecursive);
		QTreeWidgetItem *item = nullptr, *curr_item = index_trw->currentItem();

		do
		{
			item = list.at(rndgen.bounded(0, list.size()));
			index_trw->setCurrentItem(item);
			navigateToItem(false);
		}
		while(curr_item == index_trw->currentItem());
	});

	connect(net_manager, &QNetworkAccessManager::finished, this, [this](QNetworkReply *reply){
		if(load_tip)
			loadTipOfDay(reply);
		else
		{
			loadIndex(reply);
			navigateToItem(false);
		}
	});
}

void TipOfDayForm::navigateToItem(bool prev_item)
{
	if(index_trw->topLevelItemCount() == 0)
		return;

	int inc = prev_item ? -1 : 1;

	if(!index_trw->currentItem() && index_trw->topLevelItemCount() > 0)
		index_trw->setCurrentItem(index_trw->topLevelItem(0));

	/* This iterator jumps to the first selectable item in index_trw
	 * in which flag Qt::ItemIsSelectable is set */
	QTreeWidgetItemIterator tree_itr(index_trw->currentItem(), QTreeWidgetItemIterator::Selectable);

	if(*tree_itr && *tree_itr != index_trw->currentItem())
		index_trw->setCurrentItem(*tree_itr);
	else
	{
		tree_itr += inc;

		if(*tree_itr)
			index_trw->setCurrentItem(*tree_itr);

		/* When the iterator reaches the end of the tree,
		 * it'll contain null value. In this case we need
		 * to select either to top or bottom item depending
		 * on the direction of the navigation (prev_item = true|false) */
		else
		{
			QTreeWidgetItem *item = nullptr;

			/* If we're moving foward in the index
			 * we select the first top item */
			if(!prev_item)
				item = index_trw->topLevelItem(0);
			else
			{
				/* Otherwise we select the last item in the hierarchy
				 * which can be a toplevel item or the last child of it */
				item = index_trw->topLevelItem(index_trw->topLevelItemCount() - 1);

				if(item->childCount() > 0)
					item = item->child(item->childCount() - 1);
			}

			QTreeWidgetItemIterator aux_itr(item, QTreeWidgetItemIterator::Selectable);
			index_trw->setCurrentItem(*aux_itr);
		}
	}
}

void TipOfDayForm::loadIndex(QNetworkReply *reply)
{
	QJsonArray index = getJsonObject<QJsonArray>(reply);

	if(!index.isEmpty())
	{
		QJsonObject obj;
		QTreeWidgetItem *root = nullptr, *item = nullptr;
		QMap<QString, QTreeWidgetItem *> root_items;
		QString category, title;
		int id = -1, item_idx = 1;

		index_trw->clear();
		index_trw->setSortingEnabled(false);

		for(const auto &idx_elem : std::as_const(index))
		{
			obj = idx_elem.toObject();
			category = obj.value("category").toString();
			title = obj.value("title").toString();
			id = obj.value("id").toInt();

			root = root_items.value(category);

			if(!root)
			{
				root = new QTreeWidgetItem;
				root->setText(0, category);
				root->setData(1, Qt::UserRole, -1);
				root->setFlags(Qt::ItemIsEnabled);
				root->setIcon(0, QIcon(GuiUtilsNs::getIconPath("bookmark")));
				index_trw->addTopLevelItem(root);
				root_items[category] = root;
			}

			item = new QTreeWidgetItem(root);
			item->setText(0, title);
			item->setData(1, Qt::UserRole, id);
			item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
			item->setIcon(0, QIcon(GuiUtilsNs::getIconPath("tipofday")));
			index_trw->addTopLevelItem(item);

			cached_tips[id].append(QString::number(item_idx++));
		}

		reply->deleteLater();
		index_trw->expandAll();
	}

	next_tb->setEnabled(index_trw->topLevelItemCount() > 0);
	previous_tb->setEnabled(next_tb->isEnabled());
	random_tb->setEnabled(next_tb->isEnabled());
	qApp->restoreOverrideCursor();
}

void TipOfDayForm::loadTipOfDay(QNetworkReply *reply)
{
	load_tip = false;
	QJsonObject json_obj = getJsonObject<QJsonObject>(reply);

	if(!json_obj.isEmpty())
	{
		int tip_id = json_obj.value("id").toInt();
		QString text = json_obj.value("text").toString(),
				title = json_obj.value("title").toString();

		cached_tips[tip_id].append(title);
		cached_tips[tip_id].append(text);
		showTipOfDay(tip_id);
	}

	reply->deleteLater();
	qApp->restoreOverrideCursor();
}

void TipOfDayForm::showTipOfDay(int tip_id)
{
	tip_txt->document()->setDefaultStyleSheet("img { max-width: 80%; }");
	tip_title_lbl->setText(QString("#%1 ").arg(cached_tips[tip_id].at(0)) +
												 cached_tips[tip_id].at(1));
	tip_txt->setHtml(cached_tips[tip_id].at(2));
}

void TipOfDayForm::handleItemSelected(QTreeWidgetItem *item)
{
	int tip_id = item ? item->data(1, Qt::UserRole).toInt() : -1;

	if(tip_id < 0)
		return;

	/* If the tip is fully cached (all needed attributes are present)
	 * we display the tip info from cache instead of retrieving it
	 * from the project's site */
	if(cached_tips.contains(tip_id) &&
		 cached_tips[tip_id].size() > 1)
		showTipOfDay(tip_id);
	else
	{
		// If the tip is not cached we retrieve it from the website
		load_tip = true;
		qApp->setOverrideCursor(Qt::WaitCursor);
		net_manager->get(QNetworkRequest(QUrl(QString("http://localhost:8000/tipofday/%1").arg(tip_id))));
	}
}

template<class RetClass>
RetClass TipOfDayForm::getJsonObject(QNetworkReply *reply)
{
	unsigned http_status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toUInt();

	if(reply->error() != QNetworkReply::NoError || http_status != 200)
	{
		Messagebox::error(tr("Failed to load resource <strong>%1</strong>! %2")
											.arg(reply->url().toString(), reply->errorString()));
		return RetClass();
	}

	QJsonDocument json_doc = QJsonDocument::fromJson(reply->readAll());

	if constexpr (std::is_same_v<QJsonArray, RetClass>)
	{
		if(json_doc.isArray())
			return json_doc.array();
	}
	else if constexpr (std::is_same_v<QJsonObject, RetClass>)
	{
		if(json_doc.isObject())
			return json_doc.object();
	}

	return RetClass();
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
