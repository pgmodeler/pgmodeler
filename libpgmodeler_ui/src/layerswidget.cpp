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

#include "layerswidget.h"

LayersWidget::LayersWidget(QWidget *parent) : QWidget(parent)
{
	setupUi(this);
	setModel(nullptr);

	old_pos = QPoint(-1, -1);
	curr_item = nullptr;
	curr_row = -1;

	layers_lst->installEventFilter(this);
	frame->installEventFilter(this);

	QAction *act = visibility_menu.addAction(tr("Show all"), this, SLOT(setLayersVisible()));
	act->setData(true);

	act = visibility_menu.addAction(tr("Hide all"), this, SLOT(setLayersVisible()));
	act->setData(false);

	visibility_tb->setMenu(&visibility_menu);

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

void LayersWidget::updateLayers()
{
	QListWidgetItem *item = nullptr;

	for(auto &layer : model->scene->getLayers())
	{
		item = new QListWidgetItem(layer);
		item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
		item->setCheckState(model->scene->isLayerActive(layer) ? Qt::Checked : Qt::Unchecked);
		layers_lst->addItem(item);
	}
}

bool LayersWidget::eventFilter(QObject *watched, QEvent *event)
{
	if(watched == layers_lst)
	{
		if(event->type() == QEvent::KeyPress)
		{
			QKeyEvent *k_event = dynamic_cast<QKeyEvent *>(event);

			if(curr_item && (k_event->key() == Qt::Key_Enter || k_event->key() == Qt::Key_Return))
				finishLayerRenaming();
			else if(!curr_item && k_event->key() == Qt::Key_F2 && layers_lst->currentRow() > 0)
				startLayerRenaming(layers_lst->currentItem());
		}
		else if(event->type() == QEvent::FocusIn && curr_item)
		{
			finishLayerRenaming();
		}
	}
	else if(watched == frame && (event->type()==QEvent::MouseMove || event->type()==QEvent::MouseButtonPress))
	{
		QMouseEvent *m_event=dynamic_cast<QMouseEvent *>(event);

		if(event->type() == QEvent::MouseButtonPress)
			old_pos = QPoint(-1,-1);
		else
		{
			if(m_event->buttons() == Qt::LeftButton)
			{
				QPoint pnt = this->mapToParent(m_event->pos());
				int w = 0, h = 0;

				//Calculates the width and height based upon the delta between the points
				w = this->width() + (pnt.x() - old_pos.x());
				h = this->geometry().bottom() - pnt.y() + 1;

				if(h >= this->minimumHeight() && h <= this->maximumHeight() &&
					 w >= this->minimumWidth() && w <= this->maximumWidth())
					this->setGeometry(this->pos().x(), pnt.y(), w, h);

				old_pos = pnt;
			}
		}
	}

	return false;
}

void LayersWidget::updateActiveLayers()
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
	emit s_activeLayersChanged();
}

void LayersWidget::removeLayer(bool clear)
{
	QListWidgetItem *item = nullptr;
	Messagebox msg_box;

	if(clear)
		msg_box.show(tr("This action will delete all layers (except the default one) and the objects in them will be moved to the default layer. Do you want to proceed?"),
								 Messagebox::ConfirmIcon, Messagebox::YesNoButtons);
	else
		msg_box.show(tr("Delete the selected layer will cause objects in it to be moved to the default layer. Do you want to proceed?"),
								 Messagebox::ConfirmIcon, Messagebox::YesNoButtons);

	if(msg_box.result() == QDialog::Accepted)
	{
		if(clear)
		{
			model->scene->removeLayers();

			while(layers_lst->count() > 1)
			{
				item = layers_lst->takeItem(layers_lst->count() - 1);
				delete item;
			}
		}
		else if(layers_lst->currentRow() > 0)
		{
			item = layers_lst->currentItem();
			model->scene->removeLayer(item->text());
			layers_lst->takeItem(layers_lst->currentRow());
			delete item;
		}

		layers_lst->setCurrentRow(-1);
		enableButtons();
	}
}

void LayersWidget::enableButtons()
{
	remove_tb->setEnabled(layers_lst->currentRow() > 0);
	remove_all_tb->setEnabled(layers_lst->count() > 1);
}

void LayersWidget::setLayersVisible()
{
	QAction *act = qobject_cast<QAction *>(sender());
	Qt::CheckState chk_state = act->data().toBool() ? Qt::Checked : Qt::Unchecked;

	layers_lst->blockSignals(true);

	for(auto &item : layers_lst->findItems("*", Qt::MatchWildcard))
		item->setCheckState(chk_state);

	layers_lst->blockSignals(false);
	updateActiveLayers();
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
		updateLayers();
}

QListWidgetItem *LayersWidget::addLayer(const QString &name)
{
	QListWidgetItem *item = nullptr;
	QString aux_name = name.isEmpty() ? tr("New layer") : name;
	QStringList act_layers = 	model->scene->getLayers();

	aux_name = model->scene->addLayer(aux_name);
	item = new QListWidgetItem(aux_name);

	item->setCheckState(Qt::Checked);
	item->setFlags((item->flags() | Qt::ItemIsUserCheckable) ^ Qt::ItemIsEditable);

	layers_lst->addItem(item);

	act_layers.prepend(aux_name);
	model->scene->setActiveLayers(act_layers);

	/* Reconfigure the model's menu if we have selected items so the new layer can
	 * appear in the "Move to layer" quick action */
	if(!model->scene->selectedItems().isEmpty())
		model->configureObjectSelection();

	enableButtons();

	return item;
}

void LayersWidget::startLayerRenaming(QListWidgetItem *item)
{
	if(layers_lst->item(0) != item)
	{
		curr_item = item;
		curr_text = item->text();
		curr_row = layers_lst->currentRow();
		layers_lst->openPersistentEditor(item);
	}
}

void LayersWidget::finishLayerRenaming()
{
	if(curr_item)
	{
		layers_lst->closePersistentEditor(curr_item);

		if(curr_item->text().isEmpty())
			curr_item->setText(curr_text);
		else
			curr_item->setText(model->scene->renameLayer(curr_row, curr_item->text()));

		curr_item = nullptr;
		curr_text.clear();
		curr_row = -1;

		/* Reconfigure the model's menu if we have selected items so the renamed layer can
		 * appear in the "Move to layer" quick action */
		if(!model->scene->selectedItems().isEmpty())
			model->configureObjectSelection();
	}
}

