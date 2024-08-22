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

#include "layerswidget.h"
#include "modelwidget.h"

LayersWidget::LayersWidget(QWidget *parent) : QWidget(parent)
{
	setupUi(this);

	layers_changed = false;

	connect(layers_lst, &QListWidget::itemChanged, this, &LayersWidget::updateObjectsLayers);

	connect(layer_name_edt, &QLineEdit::textChanged, this, [this](const QString &txt){
		add_tb->setEnabled(!txt.isEmpty());
	});

	connect(add_tb, &QToolButton::clicked, this, [this](){
		emit s_newLayerRequested(layer_name_edt->text());
		layer_name_edt->clear();

		// The  new layer is checked by default
		QListWidgetItem *item = layers_lst->item(layers_lst->count() - 1);
		item->setCheckState(Qt::Checked);
	});
}

void LayersWidget::setAttributes(ModelWidget *model_wgt)
{
	if(!model_wgt)
	{
		setEnabled(false);
		return;
	}

	QListWidgetItem *item = nullptr;
	BaseGraphicObject *graph_obj = nullptr;
	QList<unsigned> sel_layers;
	unsigned layer_id = 0;
	QStringList layers = model_wgt->getObjectsScene()->getLayers();

	setEnabled(true);
	layers_changed = false;
	selected_objs.clear();
	layers_lst->clear();

	for(auto &obj : model_wgt->getSelectedObjects())
	{
		if(BaseGraphicObject::isGraphicObject(obj->getObjectType()))
		{
			graph_obj = dynamic_cast<BaseGraphicObject *>(obj);
			selected_objs.push_back(graph_obj);
			sel_layers.append(graph_obj->getLayers());
		}
	}

	for(auto &layer : layers)
	{
		item = new QListWidgetItem(layer);
		item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
		item->setCheckState(sel_layers.contains(layer_id) ? Qt::Checked : Qt::Unchecked);
		layers_lst->addItem(item);
		layer_id++;
	}

	layers_lst->setEnabled(layers.size() > 1);
}

bool LayersWidget::isLayersChanged()
{
	return layers_changed;
}

void LayersWidget::updateObjectsLayers()
{
	QList<unsigned> sel_layers;
	QListWidgetItem *item = nullptr;

	for(int row = 0; row < layers_lst->count(); row++)
	{
		item = layers_lst->item(row);

		if(item->checkState() == Qt::Checked)
			sel_layers.append(static_cast<unsigned>(row));
	}

	if(sel_layers.isEmpty())
	{
		layers_lst->item(0)->setCheckState(Qt::Checked);
		sel_layers.append(0);
	}

	for(auto &obj : selected_objs)
		obj->setLayers(sel_layers);

	layers_changed = true;
}
