/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2018 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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

#include "querybuilderpathwidget.h"
#include "querybuildercorewidget.h"
#include "pgmodeleruins.h"

QueryBuilderPathWidget::QueryBuilderPathWidget(QWidget *parent) : QWidget(parent)
{
	setupUi(this);
	manual_path_tw->setAccessibleName("manual_path_tw");

	connect(hide_tb, SIGNAL(clicked(void)), this, SLOT(hide(void)));

	this->setModel(nullptr);
	this->installEventFilter(this);

	manual_path_tw->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	this->manual_path_tw->verticalHeader()->setSectionsMovable(true);

	connect(manual_path_tw, &QTableWidget::itemDoubleClicked, [&](QTableWidgetItem *item){
		model_wgt->scene->clearSelection();
		QList<BaseObjectView *> obj;
		obj.push_back(
			dynamic_cast<BaseObjectView *>(
				reinterpret_cast<BaseRelationship *>(
					manual_path_tw->item(item->row(),0)->data(Qt::UserRole).value<void *>())
						->getReceiverObject()));
		model_wgt->scene->selectItems(obj);
		model_wgt->viewport->centerOn(dynamic_cast<RelationshipView *>(obj.front())->getLabel(0));
	});
}

bool QueryBuilderPathWidget::eventFilter(QObject *object, QEvent *event)
{
	auto *k_event=dynamic_cast<QKeyEvent *>(event);

	if (event->type() == QEvent::Hide)
		emit s_visibilityChanged(false);

	else if (event->type() == QEvent::Show)
		emit s_visibilityChanged(true);

	//Show sql when user presses enter/return on the pattern field
	else if(k_event!=nullptr && k_event->key()==Qt::Key_Delete)
	{
		QMap<int, int> ord_sel_rows;
		for(auto sel_item:manual_path_tw->selectedItems())
			ord_sel_rows.insert(manual_path_tw->row(sel_item), manual_path_tw->row(sel_item));
		QMapIterator<int, int> ord_sel_rows_itr(ord_sel_rows);
		ord_sel_rows_itr.toBack();
		while (ord_sel_rows_itr.hasPrevious())
		{
			ord_sel_rows_itr.previous();
			manual_path_tw->removeRow(ord_sel_rows_itr.value());
		}

		gqb_c->updateRelLabel();
		return true;
	}

	return(QWidget::eventFilter(object, event));
}

void QueryBuilderPathWidget::setModel(ModelWidget *model_wgt)
{
//	bool enable=model_wgt!=nullptr;
	this->model_wgt=model_wgt;
//	insert_btn->setEnabled(enable);
//	show_sql_btn->setEnabled(enable);
//	tab_wgt->setEnabled(enable);
}

void QueryBuilderPathWidget::insertManualRels(QMap<int, BaseObjectView *> q_rels)
{
	for (auto q_rel : q_rels)
	{
		manual_path_tw->insertRow(manual_path_tw->rowCount());
		auto tab_item=new QTableWidgetItem;
		tab_item->setText(q_rel->getSourceObject()->getName());
		tab_item->setData(Qt::UserRole, QVariant::fromValue<void *>(
							dynamic_cast<BaseRelationship *>(q_rel->getSourceObject())));
		manual_path_tw->setItem(manual_path_tw->rowCount()-1, 0, tab_item);
	}
}

QMap<int, BaseRelationship *> QueryBuilderPathWidget::getRelPath(void)
{
	QMap<int, BaseRelationship *> rel_path_res;
	for(int i=0;i<manual_path_tw->rowCount();i++)
	{
		auto base_rel = reinterpret_cast<BaseRelationship *>(manual_path_tw->item(i,0)->data(Qt::UserRole).value<void *>());
		rel_path_res.insert(i, base_rel);
	}
	return rel_path_res;
}

void QueryBuilderPathWidget::clearManualPath(void)
{
	manual_path_tw->setRowCount(0);
}
