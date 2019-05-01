/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2019 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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

#include "layerswidget.h"
#include <QTextStream>

LayersWidget::LayersWidget(QWidget *parent) : QWidget(parent)
{
	setupUi(this);
	setModel(nullptr);
	curr_item = nullptr;
	curr_row = -1;
	curr_dim=-1;
	layers_tw->installEventFilter(this);
	layers_tw->insertRow(0);

	connect(hide_tb, SIGNAL(clicked(bool)), this, SIGNAL(s_visibilityChanged(bool)));
	connect(layers_tw, SIGNAL(itemSelectionChanged()), this, SLOT(finishLayerRenaming()));
	connect(add_tb, SIGNAL(clicked(bool)), this, SLOT(addLayer()));
	connect(remove_tb, SIGNAL(clicked(bool)), this, SLOT(removeLayer(bool)));
	connect(add_dim_tb, &QToolButton::clicked, [&](){
			addDimension(false, layers_tw->columnCount(), {"Default layer"});
			layers_tw->resizeRowsToContents();
			layers_tw->resizeColumnsToContents();
			model->scene->addDimension();
	});
	connect(remove_dim_tb, SIGNAL(clicked(bool)), this, SLOT(removeDimension()));
	connect(remove_all_tb, &QToolButton::clicked, [&](){
		removeLayer(true);
	});
}

void LayersWidget::updateLayers(void)
{
	layers_tw->setColumnCount(0);
	int d_idx=0;
	for(auto &dim : model->scene->getLayers())
		addDimension(true, d_idx++, dim);

	layers_tw->verticalHeader()->setVisible(false);
	layers_tw->setSelectionMode(QAbstractItemView::SingleSelection);
	layers_tw->resizeRowsToContents();
	layers_tw->resizeColumnsToContents();
}

bool LayersWidget::eventFilter(QObject *watched, QEvent *event)
{
	if(watched == layers_tw && event->type() == QEvent::KeyPress && curr_dim!=-1)
	{
		QKeyEvent *k_event = dynamic_cast<QKeyEvent *>(event);

		auto curr_tree_wgt = qobject_cast<QTreeWidget *>(layers_tw->cellWidget(0, curr_dim));
		if(curr_item && (k_event->key() == Qt::Key_Enter || k_event->key() == Qt::Key_Return))
			finishLayerRenaming();
		else if(!curr_item && k_event->key() == Qt::Key_F2 &&
				curr_tree_wgt && curr_tree_wgt->currentIndex().row() > 0)
			startLayerRenaming(curr_tree_wgt->currentItem());
	}

	return(false);
}

void LayersWidget::updateSelection(void)
{
	curr_dim=-1;
	for(int col=0; col<layers_tw->columnCount(); col++)
	{
		if(qobject_cast<QTreeWidget *>(layers_tw->cellWidget(0, col)->children().last()) ==
			qobject_cast<QTreeWidget *>(sender()))
			curr_dim=col;
		else
			qobject_cast<QTreeWidget *>(layers_tw->cellWidget(0, col)->children().last())
					->clearSelection();
	}
}


void LayersWidget::updateActiveLayers(void)
{
	QStringList active_dim;
	vector<QStringList> active_layers;
	QTreeWidgetItem *item = nullptr;

	for(int dim=0; dim< layers_tw->columnCount(); dim++)
	{
		auto curr_tree_wgt = qobject_cast<QTreeWidget *>(layers_tw->cellWidget(0, dim)->children().last());
		for(int row = 0; row < curr_tree_wgt->topLevelItemCount(); row++)
		{
			item = curr_tree_wgt->topLevelItem(row);
			if(item->checkState(0) == Qt::Checked)
				active_dim.push_back(item->text(0));
		}
		active_layers.push_back(active_dim);
		active_dim.clear();
	}

	model->scene->setActiveLayers(active_layers);
	emit s_activeLayersChanged();
}

void LayersWidget::removeLayer(bool clear)
{
	QTreeWidgetItem *item = nullptr;
	Messagebox msg_box;

	if(clear)
		msg_box.show(trUtf8("This action will delete all layers (except the default one) and the objects in them will be moved to the default layer. Do you want to proceed?"),
								 Messagebox::ConfirmIcon, Messagebox::YesNoButtons);
	else
		msg_box.show(trUtf8("Delete the selected layer will cause objects in it to be moved to the default layer. Do you want to proceed?"),
								 Messagebox::ConfirmIcon, Messagebox::YesNoButtons);

	if(msg_box.result() == QDialog::Accepted)
	{
		if(clear)
		{
			model->scene->removeLayers();

			layers_tw->setColumnCount(1);
			auto last_tree_wgt = qobject_cast<QTreeWidget *>(layers_tw->cellWidget(0, 0)->children().last());
			while(last_tree_wgt->topLevelItemCount() > 1)
			{
				item = last_tree_wgt->takeTopLevelItem(last_tree_wgt->topLevelItemCount()-1);
				delete(item);
			}
		}
		else if(curr_dim!=-1)
		{
			auto sel_tree_wgt = qobject_cast<QTreeWidget *>(layers_tw->cellWidget(0,
															curr_dim)->children().last());
			if (sel_tree_wgt->currentIndex().row()>0)
			{
				item = sel_tree_wgt->currentItem();
				model->scene->removeLayer(curr_dim, item->text(0));
				sel_tree_wgt->setCurrentItem(sel_tree_wgt->itemAbove(item));
				sel_tree_wgt->takeTopLevelItem(sel_tree_wgt->currentIndex().row()+1);
				delete(item);
			}
		}
		enableButtons();
	}
}

void LayersWidget::enableButtons(void)
{
	auto curr_tree_wgt= qobject_cast<QTreeWidget *>(layers_tw->cellWidget(0,curr_dim<0?0:curr_dim)
											->children().last());
	remove_tb->setEnabled(curr_dim!=-1 && curr_tree_wgt->currentIndex().row() > 0);
	remove_dim_tb->setEnabled(curr_dim!=-1 && layers_tw->columnCount()>1);
	remove_all_tb->setEnabled(layers_tw->columnCount() > 1 || curr_tree_wgt->topLevelItemCount() > 1);
}

void LayersWidget::setVisible(bool value)
{
	QWidget::setVisible(value);
	emit s_visibilityChanged(value);
}

void LayersWidget::setModel(ModelWidget *model)
{
	bool enable = model != nullptr;

	this->model = model;
	layers_tw->clear();
	add_tb->setEnabled(enable);

	if(model)
		updateLayers();
}

QTreeWidgetItem *LayersWidget::addLayer(const QString &name)
{
	QTreeWidgetItem *item = nullptr;
	QString aux_name = name.isEmpty() ? trUtf8("New layer") : name;

	aux_name = model->scene->addLayer(curr_dim==-1?0:curr_dim, aux_name);
	item = new QTreeWidgetItem((QTreeWidget*)nullptr, QStringList(QString(aux_name)));
	item->setFlags((item->flags() | Qt::ItemIsUserCheckable) ^ Qt::ItemIsEditable);
	item->setCheckState(0, Qt::Unchecked);
	qobject_cast<QTreeWidget *>(layers_tw->cellWidget(0,curr_dim==-1?0:curr_dim)
								->children().last())->addTopLevelItem(item);
	/* Reconfigure the model's menu if we have selected items so the new layer can
	 * appear in the "Move to layer" quick action */
	if(!model->scene->selectedItems().isEmpty())
		model->configureObjectSelection();

	enableButtons();

	return(item);
}

void LayersWidget::startLayerRenaming(QTreeWidgetItem *item)
{
	if(curr_dim!=-1)
	{
		auto curr_tree_wgt = qobject_cast<QTreeWidget *>(
					layers_tw->cellWidget(0,curr_dim)->children().last());
		if(curr_tree_wgt->itemAt(0,0) != item)
		{
			curr_item = item;
			curr_text = item->text(0);
			curr_row = curr_tree_wgt->currentIndex().row();
			curr_tree_wgt->openPersistentEditor(item);
		}
	}
}

void LayersWidget::finishLayerRenaming(void)
{
	if(curr_item)
	{
		qobject_cast<QTreeWidget *>(layers_tw->cellWidget(0,curr_dim)->children().last())
				->closePersistentEditor(curr_item);

		if(curr_item->text(0).isEmpty())
			curr_item->setText(0, curr_text);
		else
			curr_item->setText(0, model->scene->renameLayer(
								   curr_dim, curr_row, curr_item->text(0)));

		curr_item = nullptr;
		curr_text.clear();
		curr_row = -1;

		/* Reconfigure the model's menu if we have selected items so the renamed layer can
		 * appear in the "Move to layer" quick action */
		if(!model->scene->selectedItems().isEmpty())
			model->configureObjectSelection();
	}
}

void LayersWidget::addDimension(bool begin, int d_idx, QStringList dim)
{
	layers_tw->insertColumn(d_idx);

	auto *wgt=new QWidget;
	auto *twgt=new QTreeWidget;
	auto *lyt=new QHBoxLayout(wgt);
	lyt->addWidget(twgt);
	lyt->setMargin(0);
	twgt->setHeaderHidden(true);
	twgt->setRootIsDecorated(false);
	layers_tw->setCellWidget(0, d_idx, wgt);

	QList<QTreeWidgetItem *> items;
	for(auto &layer : dim)
	{
		items.append(new QTreeWidgetItem((QTreeWidget*)nullptr, QStringList(QString(layer))));
		items.last()->setFlags(items.last()->flags() | Qt::ItemIsUserCheckable);
		if(begin)
			items.last()->setCheckState(0, model->scene->isLayerActive(d_idx, layer) ? Qt::Checked : Qt::Unchecked);
		else
			items.last()->setCheckState(0, Qt::Checked);
	}

	twgt->insertTopLevelItems(0, items);
	twgt->setColumnCount(1);
	twgt->resizeColumnToContents(0);

	connect(twgt, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(startLayerRenaming(QTreeWidgetItem*)));
	connect(twgt, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(updateActiveLayers()));
	connect(twgt, SIGNAL(itemSelectionChanged()), this, SLOT(updateSelection()));
	connect(twgt, SIGNAL(itemSelectionChanged()), this, SLOT(enableButtons()));
}

void LayersWidget::removeDimension(void)
{
	if(curr_dim!=-1)
	{
		layers_tw->removeColumn(curr_dim);
		model->scene->removeDimension(curr_dim);
		curr_dim=-1;
		remove_dim_tb->setEnabled(false);
	}
}
