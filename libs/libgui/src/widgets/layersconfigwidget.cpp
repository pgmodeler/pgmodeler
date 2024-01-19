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

#include "layersconfigwidget.h"
#include "colorpickerwidget.h"
#include "guiutilsns.h"

LayersConfigWidget::LayersConfigWidget(QWidget *parent) : QWidget(parent)
{
	setupUi(this);
	setModel(nullptr);

	old_pos = QPoint(-1, -1);
	curr_item = nullptr;
	curr_item = nullptr;
	curr_row = -1;

	layers_tab->installEventFilter(this);
	frame->installEventFilter(this);

	QAction *act = visibility_menu.addAction(tr("Show all"), this, &LayersConfigWidget::setLayersActive);
	act->setData(true);

	act = visibility_menu.addAction(tr("Hide all"), this, &LayersConfigWidget::setLayersActive);
	act->setData(false);

	add_tb->setToolTip(add_tb->toolTip() + QString(" (%1)").arg(add_tb->shortcut().toString()));
	remove_tb->setToolTip(remove_tb->toolTip() + QString(" (%1)").arg(remove_tb->shortcut().toString()));
	remove_all_tb->setToolTip(remove_all_tb->toolTip() + QString(" (%1)").arg(remove_all_tb->shortcut().toString()));

	visibility_tb->setMenu(&visibility_menu);
	GuiUtilsNs::createDropShadow(this, 5, 5, 30);

	connect(toggle_layers_rects_chk, &QCheckBox::toggled, this, &LayersConfigWidget::toggleLayersRects);
	connect(toggle_layers_rects_chk, &QCheckBox::toggled, toggle_layers_names_chk, &QCheckBox::setEnabled);
	connect(toggle_layers_names_chk, &QCheckBox::toggled, this, &LayersConfigWidget::toggleLayersRects);

	connect(hide_tb, &QToolButton::clicked, this, &LayersConfigWidget::s_visibilityChanged);

	connect(add_tb, &QToolButton::clicked, this, [this](){
		addLayer();
	});

	connect(remove_tb, &QToolButton::clicked, this, &LayersConfigWidget::removeLayer);

	connect(layers_tab, &QTableWidget::itemDoubleClicked, this, &LayersConfigWidget::startLayerRenaming);
	connect(layers_tab, &QTableWidget::itemChanged, this, &LayersConfigWidget::updateActiveLayers);
	connect(layers_tab, &QTableWidget::itemSelectionChanged, this, &LayersConfigWidget::finishLayerRenaming);
	connect(layers_tab, &QTableWidget::itemSelectionChanged, this, &LayersConfigWidget::enableButtons);

	connect(remove_all_tb, &QToolButton::clicked, this, [this](){
		removeLayer(true);
	});
}

bool LayersConfigWidget::eventFilter(QObject *watched, QEvent *event)
{
	if(watched == layers_tab)
	{
		if(event->type() == QEvent::KeyPress)
		{
			QKeyEvent *k_event = dynamic_cast<QKeyEvent *>(event);

			if(curr_item && (k_event->key() == Qt::Key_Enter || k_event->key() == Qt::Key_Return))
				finishLayerRenaming();
		}
		else if(event->type() == QEvent::FocusIn && curr_item && curr_item != layers_tab->currentItem())
			finishLayerRenaming();
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

void LayersConfigWidget::updateActiveLayers()
{
	QList<unsigned> active_layers;
	QTableWidgetItem *item = nullptr;

	for(int row = 0; row < layers_tab->rowCount(); row++)
	{
		item = layers_tab->item(row, 0);

		if(item->checkState() == Qt::Checked)
			active_layers.append(static_cast<unsigned>(row));
	}

	model->scene->setActiveLayers(active_layers);
	model->getDatabaseModel()->setObjectsModified({ ObjectType::Schema });
	emit s_activeLayersChanged();
}

void LayersConfigWidget::removeLayer(bool clear)
{
	QTableWidgetItem *item = nullptr;
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

			while(layers_tab->rowCount() > 1)
			{
				disconnect(rect_color_pickers.last(), nullptr, nullptr, nullptr);
				rect_color_pickers.removeLast();

				disconnect(name_color_pickers.last(), nullptr, nullptr, nullptr);
				name_color_pickers.removeLast();

				layers_tab->setRowCount(layers_tab->rowCount() - 1);
			}
		}
		else if(layers_tab->currentRow() > 0)
		{
			int row = layers_tab->currentRow();

			item = layers_tab->item(layers_tab->currentRow(), 0);
			model->scene->removeLayer(item->text());

			disconnect(rect_color_pickers.at(row), nullptr, nullptr, nullptr);
			rect_color_pickers.removeAt(row);

			disconnect(name_color_pickers.at(row), nullptr, nullptr, nullptr);
			name_color_pickers.removeAt(row);

			layers_tab->removeRow(row);
		}

		layers_tab->clearSelection();
		enableButtons();
	}
}

void LayersConfigWidget::updateLayerColors(int layer_idx)
{
	if(layer_idx < 0)
	{
		layer_idx = rect_color_pickers.indexOf(dynamic_cast<ColorPickerWidget *>(sender()));

		if(layer_idx < 0)
			layer_idx = name_color_pickers.indexOf(dynamic_cast<ColorPickerWidget *>(sender()));
	}

	if(layer_idx >= 0 && layer_idx < layers_tab->rowCount())
	{
		model->scene->setLayerColors(layer_idx,
																 name_color_pickers[layer_idx]->getColor(0),
																 rect_color_pickers[layer_idx]->getColor(0));
		model->updateModelLayersInfo();
	}
}

void LayersConfigWidget::enableButtons()
{
	remove_tb->setEnabled(layers_tab->currentRow() > 0);
	remove_all_tb->setEnabled(layers_tab->rowCount() > 1);
}

void LayersConfigWidget::setLayersActive()
{
	QAction *act = qobject_cast<QAction *>(sender());
	Qt::CheckState chk_state = act->data().toBool() ? Qt::Checked : Qt::Unchecked;

	layers_tab->blockSignals(true);

	for(int row = 0; row < layers_tab->rowCount(); row++)
		layers_tab->item(row, 0)->setCheckState(chk_state);

	layers_tab->blockSignals(false);
	updateActiveLayers();
}

void LayersConfigWidget::setVisible(bool value)
{
	QWidget::setVisible(value);
	emit s_visibilityChanged(value);
}

void LayersConfigWidget::updateLayersRects()
{
	if(!model)
		return;

	model->getObjectsScene()->clearSelection();
	model->getObjectsScene()->updateLayerRects();
}

void LayersConfigWidget::toggleLayersRects()
{
	if(!model)
		return;

	model->getObjectsScene()->setLayerRectsVisible(toggle_layers_rects_chk->isChecked());
	model->getObjectsScene()->setLayerNamesVisible(toggle_layers_names_chk->isChecked());
	model->updateModelLayersInfo();
	model->getDatabaseModel()->setObjectsModified({ ObjectType::Schema });

	emit s_activeLayersChanged();
}

void LayersConfigWidget::updateLayersList()
{
	if(!model)
		return;

	toggle_layers_rects_chk->blockSignals(true);
	toggle_layers_rects_chk->setChecked(model->getObjectsScene()->isLayerRectsVisible());
	toggle_layers_rects_chk->blockSignals(false);

	layers_tab->blockSignals(true);
	for(auto &layer : model->scene->getLayers())
	{
		__addLayer(layer,
							 model->scene->isLayerActive(layer) ? Qt::Checked : Qt::Unchecked);
	}
	layers_tab->blockSignals(false);

	int idx = 0, p_idx = 0;
	QList<QStringList> colors_lists = {
		model->getDatabaseModel()->getLayerNameColors(),
		model->getDatabaseModel()->getLayerRectColors()
	};

	QList<QList<ColorPickerWidget*>*> pickers = { &name_color_pickers,
																								&rect_color_pickers };

	for(auto &cl_list : colors_lists)
	{
		idx = 0;

		for(auto &cl : cl_list)
		{
			if(idx >= pickers[p_idx]->size())
				break;

			pickers[p_idx]->at(idx)->blockSignals(true);
			pickers[p_idx]->at(idx)->setColor(0, QColor(cl));
			pickers[p_idx]->at(idx)->blockSignals(false);
			idx++;
		}

		p_idx++;
	}

	toggle_layers_names_chk->blockSignals(true);
	toggle_layers_names_chk->setChecked(model->getDatabaseModel()->isLayerNamesVisible());
	toggle_layers_names_chk->setEnabled(model->getDatabaseModel()->isLayerNamesVisible());
	toggle_layers_names_chk->blockSignals(false);

	toggle_layers_rects_chk->blockSignals(true);
	toggle_layers_rects_chk->setChecked(model->getDatabaseModel()->isLayerRectsVisible());
	toggle_layers_rects_chk->blockSignals(false);

}

void LayersConfigWidget::setModel(ModelWidget *model)
{
	bool enable = model != nullptr;

	this->model = model;
	layers_tab->setRowCount(0);
	name_color_pickers.clear();
	rect_color_pickers.clear();
	add_tb->setEnabled(enable);
	updateLayersList();
}

void LayersConfigWidget::__addLayer(const QString &name, Qt::CheckState chk_state)
{
	ColorPickerWidget *color_picker = nullptr;
	QTableWidgetItem *item = nullptr;

	int row = layers_tab->rowCount();
	layers_tab->insertRow(row);

	item = new QTableWidgetItem;
	item->setText(name);
	item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
	item->setCheckState(chk_state);
	layers_tab->setItem(row, 0, item);

	color_picker = new ColorPickerWidget(1, layers_tab);
	color_picker->setButtonToolTip(0, tr("Layer name color"));
	color_picker->layout()->setContentsMargins(GuiUtilsNs::LtMargin,GuiUtilsNs::LtMargin,GuiUtilsNs::LtMargin,GuiUtilsNs::LtMargin);
	color_picker->setColor(0, QColor(0,0,0));
	name_color_pickers.append(color_picker);

	connect(color_picker, &ColorPickerWidget::s_colorChanged, this, &LayersConfigWidget::updateLayerColors);
	connect(color_picker, &ColorPickerWidget::s_colorsChanged, this, [this]() {
		updateLayerColors();
	});

	layers_tab->setCellWidget(row, 1, color_picker);

	color_picker = new ColorPickerWidget(1, layers_tab);
	color_picker->setButtonToolTip(0, tr("Layer rectangle color"));
	color_picker->layout()->setContentsMargins(5,5,5,5);
	color_picker->generateRandomColors();
	rect_color_pickers.append(color_picker);
	connect(color_picker, &ColorPickerWidget::s_colorChanged, this, &LayersConfigWidget::updateLayerColors);
	connect(color_picker, &ColorPickerWidget::s_colorsChanged, this, [this]() {
		updateLayerColors();
	});

	layers_tab->setCellWidget(row, 2, color_picker);

	layers_tab->horizontalHeader()->setStretchLastSection(false);
	layers_tab->resizeRowsToContents();
	layers_tab->resizeColumnsToContents();
	layers_tab->horizontalHeader()->setStretchLastSection(true);
	layers_tab->clearSelection();

	enableButtons();
}

void LayersConfigWidget::addLayer(const QString &name)
{
	QString fmt_name = name.isEmpty() ? tr("New layer") : name;
	QStringList act_layers = model->scene->getActiveLayers();

	fmt_name = model->scene->addLayer(fmt_name);

	__addLayer(fmt_name);
	act_layers.prepend(fmt_name);

	updateLayerColors(layers_tab->rowCount() - 1);
	model->scene->setActiveLayers(act_layers);

	/* Reconfigure the model's menu if we have selected items so the new layer can
	 * appear in the "Move to layer" quick action */
	if(!model->scene->selectedItems().isEmpty())
		model->configureObjectSelection();
}

void LayersConfigWidget::startLayerRenaming()
{
	if(layers_tab->currentRow() < 0)
		return;

	QTableWidgetItem *item = layers_tab->item(layers_tab->currentRow(), 0);

	curr_item = item;
	curr_text = item->text();
	curr_row = layers_tab->currentRow();
	layers_tab->openPersistentEditor(item);
}

void LayersConfigWidget::finishLayerRenaming()
{
	if(layers_tab->currentRow() < 0)
		return;

	if(curr_item)
	{
		layers_tab->closePersistentEditor(curr_item);

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

