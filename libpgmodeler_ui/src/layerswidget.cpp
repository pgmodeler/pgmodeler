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

#include "layerswidget.h"
#include <QTextStream>

LayersWidget::LayersWidget(QWidget *parent) : QWidget(parent)
{
	setupUi(this);
	setModel(nullptr);
	curr_item = nullptr;

	layers_lst->installEventFilter(this);

	connect(hide_tb, SIGNAL(clicked(bool)), this, SIGNAL(s_visibilityChanged(bool)));
	connect(layers_lst, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(startLayerRenaming(QListWidgetItem*)));
	connect(layers_lst, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(updateActiveLayers()));
	connect(layers_lst, SIGNAL(itemSelectionChanged()), this, SLOT(finishLayerRenaming()));
	connect(layers_lst, SIGNAL(itemSelectionChanged()), this, SLOT(enableButtons()));
	connect(add_tb, SIGNAL(clicked(bool)), this, SLOT(addLayer()));
	connect(remove_tb, SIGNAL(clicked(bool)), this, SLOT(removeLayer(bool)));
	connect(remove_all_tb, &QToolButton::clicked, [&](){
		removeLayer(true);
	});
}

void LayersWidget::setLayers(const QStringList &layers)
{
	QListWidgetItem *item = nullptr;
	QStringList active_layers = model->scene->getActiveLayers();

	for(auto &layer : layers)
	{
		item = new QListWidgetItem(layer);
		item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
		item->setCheckState(active_layers.contains(layer) ? Qt::Checked : Qt::Unchecked);
		layers_lst->addItem(item);
	}
}

bool LayersWidget::eventFilter(QObject *watched, QEvent *event)
{
	if(watched == layers_lst && event->type() == QEvent::KeyPress)
	{
		QKeyEvent *k_event = dynamic_cast<QKeyEvent *>(event);

		if(curr_item && (k_event->key() == Qt::Key_Enter || k_event->key() == Qt::Key_Return))
			finishLayerRenaming();
	}

	return(false);
}

void LayersWidget::updateActiveLayers(void)
{
	QStringList active_layers;
	QListWidgetItem *item = nullptr;

	for(int row = 0; row < layers_lst->count(); row++)
	{
		item = layers_lst->item(row);

		if(item->checkState() == Qt::Checked)
			active_layers.push_back(item->text());
	}

	model->scene->setActiveLayers(active_layers);
}

void LayersWidget::removeLayer(bool clear)
{
	QListWidgetItem *item = nullptr;
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

			while(layers_lst->count() > 1)
			{
				item = layers_lst->takeItem(layers_lst->count() - 1);
				delete(item);
			}
		}
		else if(layers_lst->currentRow() > 0)
		{
			item = layers_lst->currentItem();
			model->scene->removeLayer(item->text());
			layers_lst->takeItem(layers_lst->currentRow());
			delete(item);
		}

		layers_lst->setCurrentRow(-1);
		enableButtons();
	}
}

void LayersWidget::enableButtons(void)
{
	remove_tb->setEnabled(layers_lst->currentRow() > 0);
	remove_all_tb->setEnabled(layers_lst->count() > 1);
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
	layers_lst->clear();
	add_tb->setEnabled(enable);

	if(model)
		setLayers(model->scene->getLayers());
}

QListWidgetItem *LayersWidget::addLayer(const QString &name)
{
	QListWidgetItem *item = nullptr;
	QString aux_name = name.isEmpty() ? trUtf8("New layer") : name;

	aux_name = model->scene->addLayer(aux_name);
	item = new QListWidgetItem(aux_name);
	item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
	item->setCheckState(Qt::Unchecked);
	layers_lst->addItem(item);
	enableButtons();

	return(item);
}

void LayersWidget::startLayerRenaming(QListWidgetItem *item)
{
	if(layers_lst->item(0) != item)
	{
		curr_item = item;
		curr_text = item->text();
		layers_lst->openPersistentEditor(item);
	}
}

void LayersWidget::finishLayerRenaming(void)
{
	if(curr_item)
	{
		layers_lst->closePersistentEditor(curr_item);

		if(curr_item->text().isEmpty())
			curr_item->setText(curr_text);
		else
			curr_item->setText(model->scene->renameLayer(layers_lst->currentRow(), curr_item->text()));

		curr_item = nullptr;
		curr_text.clear();
	}
}

