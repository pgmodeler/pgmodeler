/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2015 - Raphael Araújo e Silva <raphael@pgmodeler.com.br>
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

#include "eventtriggerwidget.h"

EventTriggerWidget::EventTriggerWidget(QWidget *parent): BaseObjectWidget(parent, OBJ_EVENT_TRIGGER)
{
	Ui_EventTriggerWidget::setupUi(this);

	function_sel=new ObjectSelectorWidget(OBJ_FUNCTION, true, this);
	filter_tab=new ObjectTableWidget(ObjectTableWidget::ADD_BUTTON |
																	 ObjectTableWidget::EDIT_BUTTON |
																	 ObjectTableWidget::UPDATE_BUTTON |
																	 ObjectTableWidget::REMOVE_BUTTON |
																	 ObjectTableWidget::REMOVE_ALL_BUTTON |
																	 ObjectTableWidget::MOVE_BUTTONS, false, this);
	filter_tab->setColumnCount(1);
	filter_tab->setHeaderLabel(trUtf8("Tag command"), 0);

	eventtrigger_grid->addWidget(function_sel, 1, 1);
	filter_layout->addWidget(filter_tab);

	configureFormLayout(eventtrigger_grid, OBJ_EVENT_TRIGGER);

	parent_form->setMinimumSize(530, 450);
	configureTabOrder({ event_cmb, function_sel, tag_edt, filter_tab });

	QStringList list;
	EventTriggerType::getTypes(list);
	event_cmb->addItems(list);

	connect(parent_form->apply_ok_btn,SIGNAL(clicked(bool)), this, SLOT(applyConfiguration(void)));
	connect(filter_tab, SIGNAL(s_rowAdded(int)), this, SLOT(handleTagValue(int)));
	connect(filter_tab, SIGNAL(s_rowUpdated(int)), this, SLOT(handleTagValue(int)));

	connect(filter_tab, &ObjectTableWidget::s_rowsRemoved,
					[=](){ filter_tab->setButtonsEnabled(ObjectTableWidget::ADD_BUTTON, false); });

	connect(filter_tab, &ObjectTableWidget::s_rowEdited,
					[=](int row){ tag_edt->setText(filter_tab->getCellText(row, 0)); });

	connect(tag_edt, &QLineEdit::textChanged,
					[=](){
								 filter_tab->setButtonsEnabled(ObjectTableWidget::ADD_BUTTON, !tag_edt->text().isEmpty());
								 filter_tab->setButtonsEnabled(ObjectTableWidget::UPDATE_BUTTON, !tag_edt->text().isEmpty());
							 });
}

void EventTriggerWidget::hideEvent(QHideEvent *)
{
	filter_tab->removeRows();
	event_cmb->setCurrentIndex(0);
	tag_edt->clear();
}

void EventTriggerWidget::setAttributes(DatabaseModel *model, OperationList *op_list, EventTrigger *event_trig)
{
	BaseObjectWidget::setAttributes(model, op_list, event_trig);
	function_sel->setModel(model);

	if(event_trig)
	{
		event_cmb->setCurrentText(~event_trig->getEvent());
		function_sel->setSelectedObject(event_trig->getFunction());

		QStringList filter=event_trig->getFilter(ParsersAttributes::TAG.toUpper());

		if(filter.isEmpty())
			filter=event_trig->getFilter(ParsersAttributes::TAG);

		filter_tab->blockSignals(true);

    for(auto &flt : filter)
		{
			filter_tab->addRow();
			filter_tab->setCellText(flt, filter_tab->getRowCount()-1, 0);
		}

		filter_tab->blockSignals(false);
		filter_tab->clearSelection();
	}

	filter_tab->setButtonsEnabled(ObjectTableWidget::ADD_BUTTON, false);
}

void EventTriggerWidget::applyConfiguration(void)
{
	try
	{
		EventTrigger *event_trig=nullptr;

		startConfiguration<EventTrigger>();
		event_trig=dynamic_cast<EventTrigger *>(this->object);
		BaseObjectWidget::applyConfiguration();

		event_trig->setEvent(EventTriggerType(event_cmb->currentText()));
		event_trig->setFunction(dynamic_cast<Function *>(function_sel->getSelectedObject()));

		event_trig->clearFilter();
		for(unsigned row=0; row < filter_tab->getRowCount(); row++)
			event_trig->setFilter(ParsersAttributes::TAG.toUpper(), filter_tab->getCellText(row, 0));

		finishConfiguration();
	}
	catch(Exception &e)
	{
		cancelConfiguration();
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void EventTriggerWidget::handleTagValue(int row)
{
	if(!tag_edt->text().isEmpty())
	{
		filter_tab->setCellText(tag_edt->text().simplified(), row, 0);
		tag_edt->clear();
		filter_tab->clearSelection();
		filter_tab->setButtonsEnabled(ObjectTableWidget::ADD_BUTTON, false);
	}
	else if(filter_tab->getCellText(row, 0).isEmpty())
		filter_tab->removeRow(row);
}
