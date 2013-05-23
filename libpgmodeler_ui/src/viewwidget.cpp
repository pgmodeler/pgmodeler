/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2013 - Raphael Araújo e Silva <rkhaotix@gmail.com>
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

#include "viewwidget.h"
#include "rulewidget.h"
#include "triggerwidget.h"

extern RuleWidget *rule_wgt;
extern TriggerWidget *trigger_wgt;

ViewWidget::ViewWidget(QWidget *parent): BaseObjectWidget(parent, OBJ_VIEW)
{
	try
	{
		ObjectTableWidget *tab=nullptr;
		ObjectType types[]={ OBJ_TRIGGER, OBJ_RULE };
		QGridLayout *grid=nullptr;

		Ui_ViewWidget::setupUi(this);

		operation_count=0;

		expression_hl=new SyntaxHighlighter(expression_txt, false);
		expression_hl->loadConfiguration(GlobalAttributes::CONFIGURATIONS_DIR +
																		 GlobalAttributes::DIR_SEPARATOR +
																		 GlobalAttributes::SQL_HIGHLIGHT_CONF +
																		 GlobalAttributes::CONFIGURATION_EXT);

		code_hl=new SyntaxHighlighter(code_txt, false);
		code_hl->loadConfiguration(GlobalAttributes::CONFIGURATIONS_DIR +
																			 GlobalAttributes::DIR_SEPARATOR +
																			 GlobalAttributes::SQL_HIGHLIGHT_CONF +
																			 GlobalAttributes::CONFIGURATION_EXT);

		cte_expression_hl=new SyntaxHighlighter(cte_expression_txt, false);
		cte_expression_hl->loadConfiguration(GlobalAttributes::CONFIGURATIONS_DIR +
																			 GlobalAttributes::DIR_SEPARATOR +
																			 GlobalAttributes::SQL_HIGHLIGHT_CONF +
																			 GlobalAttributes::CONFIGURATION_EXT);

		table_sel=new ObjectSelectorWidget(OBJ_TABLE, true, this);
		column_sel=new ObjectSelectorWidget(OBJ_COLUMN, true, this);

		references_tab=new ObjectTableWidget(ObjectTableWidget::ALL_BUTTONS, true, this);
		references_tab->setColumnCount(4);
		references_tab->setHeaderLabel(trUtf8("Col./Expr."),0);
		references_tab->setHeaderLabel(trUtf8("Alias"),1);
		references_tab->setHeaderLabel(trUtf8("Alias Col."),2);
		references_tab->setHeaderLabel(trUtf8("Flags: SF FW AW VD"),3);

		frame_info=generateInformationFrame(trUtf8("To reference all columns in a table (*) just do not fill the field <strong>Column</strong>, this is the same as write <em><strong>[schema].[tablel].*</strong></em>"));

		referencias_grid->addWidget(table_sel, 2,1,1,2);
		referencias_grid->addWidget(column_sel, 3,1,1,2);
		referencias_grid->addWidget(frame_info, 6, 0, 1, 0);
		referencias_grid->addWidget(references_tab, 7,0,2,0);

		//Configuring the table objects that stores the triggers and rules
		for(unsigned i=0, tab_id=1; i < sizeof(types)/sizeof(ObjectType); i++, tab_id++)
		{
			tab=new ObjectTableWidget(ObjectTableWidget::ALL_BUTTONS ^
																(ObjectTableWidget::UPDATE_BUTTON  | ObjectTableWidget::MOVE_BUTTONS), true, this);

			objects_tab_map[types[i]]=tab;

			grid=new QGridLayout;
			grid->addWidget(tab, 0,0,1,1);
			grid->setContentsMargins(4,4,4,4);
			tabWidget->widget(tab_id)->setLayout(grid);

			connect(tab, SIGNAL(s_rowsRemoved(void)), this, SLOT(removeObjects(void)));
			connect(tab, SIGNAL(s_rowRemoved(int)), this, SLOT(removeObject(int)));
			connect(tab, SIGNAL(s_rowAdded(int)), this, SLOT(handleObject(void)));
			connect(tab, SIGNAL(s_rowEdited(int)), this, SLOT(handleObject(void)));
		}

		objects_tab_map[OBJ_TRIGGER]->setColumnCount(4);
		objects_tab_map[OBJ_TRIGGER]->setHeaderLabel(trUtf8("Name"), 0);
		objects_tab_map[OBJ_TRIGGER]->setHeaderIcon(QPixmap(":/icones/icones/uid.png"),0);
		objects_tab_map[OBJ_TRIGGER]->setHeaderLabel(trUtf8("Refer. Table"), 1);
		objects_tab_map[OBJ_TRIGGER]->setHeaderIcon(QPixmap(":/icones/icones/table.png"),1);
		objects_tab_map[OBJ_TRIGGER]->setHeaderLabel(trUtf8("Firing"), 2);
		objects_tab_map[OBJ_TRIGGER]->setHeaderIcon(QPixmap(":/icones/icones/trigger.png"),2);
		objects_tab_map[OBJ_TRIGGER]->setHeaderLabel(trUtf8("Events"), 3);

		objects_tab_map[OBJ_RULE]->setColumnCount(3);
		objects_tab_map[OBJ_RULE]->setHeaderLabel(trUtf8("Name"), 0);
		objects_tab_map[OBJ_RULE]->setHeaderIcon(QPixmap(":/icones/icones/uid.png"),0);
		objects_tab_map[OBJ_RULE]->setHeaderLabel(trUtf8("Execution"), 1);
		objects_tab_map[OBJ_RULE]->setHeaderLabel(trUtf8("Event"), 2);

		configureFormLayout(view_grid, OBJ_VIEW);

		connect(parent_form->apply_ok_btn,SIGNAL(clicked(bool)), this, SLOT(applyConfiguration(void)));
		connect(ref_type_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(selectReferenceType(void)));
		connect(column_sel, SIGNAL(s_objectSelected(void)), this, SLOT(showObjectName(void)));
		connect(column_sel, SIGNAL(s_selectorCleared(void)), this, SLOT(showObjectName(void)));
		connect(table_sel, SIGNAL(s_objectSelected(void)), this, SLOT(showObjectName(void)));
		connect(references_tab, SIGNAL(s_rowAdded(int)), this, SLOT(handleReference(int)));
		connect(references_tab, SIGNAL(s_rowUpdated(int)), this, SLOT(handleReference(int)));
		connect(references_tab, SIGNAL(s_rowEdited(int)), this, SLOT(editReference(int)));
		connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(updateCodePreview(void)));

		connect(view_def_chk, SIGNAL(toggled(bool)), select_from_chk, SLOT(setDisabled(bool)));
		connect(view_def_chk, SIGNAL(toggled(bool)), from_where_chk, SLOT(setDisabled(bool)));
		connect(view_def_chk, SIGNAL(toggled(bool)), after_where_chk, SLOT(setDisabled(bool)));
		connect(view_def_chk, SIGNAL(toggled(bool)), expr_alias_edt, SLOT(setDisabled(bool)));
		connect(view_def_chk, SIGNAL(toggled(bool)), expr_alias_lbl, SLOT(setDisabled(bool)));


		parent_form->setMinimumSize(650, 630);
		selectReferenceType();
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

ObjectTableWidget *ViewWidget::getObjectTable(ObjectType obj_type)
{
	if(objects_tab_map.count(obj_type) > 0)
		return(objects_tab_map[obj_type]);
	else
		return(nullptr);
}

void ViewWidget::showTableObjectForm(ObjectType obj_type)
{
	TableObject *object=nullptr;
	ObjectTableWidget *obj_table=nullptr;
	View *view=nullptr;

	obj_table=getObjectTable(obj_type);

	if(obj_table->getSelectedRow()>=0)
		object=reinterpret_cast<TableObject *>(obj_table->getRowData(obj_table->getSelectedRow()).value<void *>());

	view=dynamic_cast<View *>(this->object);

	switch(obj_type)
	{
		case OBJ_TRIGGER:
			trigger_wgt->setAttributes(this->model, view, this->op_list, dynamic_cast<Trigger *>(object));
			trigger_wgt->show();
		break;

		default:
		case OBJ_RULE:
			rule_wgt->setAttributes(this->model, view, this->op_list, dynamic_cast<Rule *>(object));
			rule_wgt->show();
		break;
	}
}

void ViewWidget::handleObject(void)
{
	ObjectType obj_type=BASE_OBJECT;

	try
	{
		obj_type=getObjectType(sender());
		showTableObjectForm(obj_type);
		listObjects(obj_type);
	}
	catch(Exception &e)
	{
		listObjects(obj_type);
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void ViewWidget::removeObjects(void)
{
	View *view=nullptr;
	unsigned count, op_count=0, i;
	BaseObject *object=nullptr;
	ObjectType obj_type=getObjectType(sender());

	try
	{
		view=dynamic_cast<View *>(this->object);
		op_count=op_list->getCurrentSize();

		while(view->getObjectCount(obj_type) > 0)
		{
			object=view->getObject(0, obj_type);
			view->removeObject(object);
			op_list->registerObject(object, Operation::OBJECT_REMOVED, 0, this->object);
		}
	}
	catch(Exception &e)
	{
		if(op_count < op_list->getCurrentSize())
		{
			count=op_list->getCurrentSize()-op_count;
			op_list->ignoreOperationChain(true);

			for(i=0; i < count; i++)
			{
				op_list->undoOperation();
				op_list->removeLastOperation();
			}

			op_list->ignoreOperationChain(false);
		}

		listObjects(obj_type);
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void ViewWidget::removeObject(int row)
{
	View *view=nullptr;
	BaseObject *object=nullptr;
	ObjectType obj_type=getObjectType(sender());

	try
	{
		view=dynamic_cast<View *>(this->object);
		object=view->getObject(row, obj_type);
		view->removeObject(object);
		op_list->registerObject(object, Operation::OBJECT_REMOVED, row, this->object);
	}
	catch(Exception &e)
	{
		listObjects(obj_type);
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

ObjectType ViewWidget::getObjectType(QObject *sender)
{
	ObjectType obj_type=BASE_OBJECT;

	if(sender)
	{
		map<ObjectType, ObjectTableWidget *>::iterator itr, itr_end;

		itr=objects_tab_map.begin();
		itr_end=objects_tab_map.end();

		while(itr!=itr_end && obj_type==BASE_OBJECT)
		{
			if(itr->second==sender)
				obj_type=itr->first;

			itr++;
		}
	}

	return(obj_type);
}

void ViewWidget::showObjectData(TableObject *object, int row)
{
	ObjectTableWidget *tab=nullptr;
	Trigger *trigger=nullptr;
	Rule *rule=nullptr;
	ObjectType obj_type;
	QString str_aux;
	unsigned i;
	EventType events[]={ EventType::on_insert, EventType::on_delete,
											 EventType::on_truncate,	EventType::on_update };

	obj_type=object->getObjectType();
	tab=objects_tab_map[obj_type];

	//Column 0: Object name
	tab->setCellText(Utf8String::create(object->getName()),row,0);

	if(obj_type==OBJ_TRIGGER)
	{
		trigger=dynamic_cast<Trigger *>(object);

		//Column 1: Table referenced by the trigger (constraint trigger)
		tab->clearCellText(row,1);
		if(trigger->getReferencedTable())
			tab->setCellText(Utf8String::create(trigger->getReferencedTable()->getName(true)),row,1);

		//Column 2: Trigger firing type
		tab->setCellText(~trigger->getFiringType(),row,2);

		//Column 3: Events that fires the trigger
		for(i=0; i < sizeof(events)/sizeof(EventType); i++)
		{
			if(trigger->isExecuteOnEvent(events[i]))
				str_aux+=~events[i] + QString(", ");
		}

		str_aux.remove(str_aux.size()-2, 2);
		tab->setCellText(str_aux ,row,3);
	}
	else if(obj_type==OBJ_RULE)
	{
		rule=dynamic_cast<Rule *>(object);

		//Column 1: Rule execution type
		tab->setCellText(~rule->getExecutionType(),row,1);

		//Column 2: Rule event type
		tab->setCellText(~rule->getEventType(),row,2);
	}

	tab->setRowData(QVariant::fromValue<void *>(object), row);
}

void ViewWidget::listObjects(ObjectType obj_type)
{
	ObjectTableWidget *tab=nullptr;
	unsigned count, i;
	View *view=nullptr;

	try
	{
		//Gets the object table related to the object type
		tab=objects_tab_map[obj_type];
		view=dynamic_cast<View *>(this->object);

		tab->blockSignals(true);
		tab->removeRows();

		count=view->getObjectCount(obj_type);
		for(i=0; i < count; i++)
		{
			tab->addRow();
			showObjectData(dynamic_cast<TableObject*>(view->getObject(i, obj_type)), i);
		}
		tab->clearSelection();
		tab->blockSignals(false);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void ViewWidget::hideEvent(QHideEvent *evento)
{
	View *view=dynamic_cast<View *>(this->object);
	ObjectType types[]={ OBJ_TRIGGER, OBJ_RULE };

	references_tab->removeRows();
	tabWidget->setCurrentIndex(0);
	cte_expression_txt->clear();

	clearReferenceForm();

	for(unsigned i=0; i < sizeof(types)/sizeof(ObjectType); i++)
	{
		objects_tab_map[types[i]]->blockSignals(true);
		objects_tab_map[types[i]]->removeRows();
		objects_tab_map[types[i]]->blockSignals(false);
	}

	if(this->new_object && !view->isModified())
		this->cancelConfiguration();

	BaseObjectWidget::hideEvent(evento);
}

void ViewWidget::clearReferenceForm(void)
{
	column_sel->clearSelector();
	table_sel->clearSelector();
	col_alias_edt->clear();
	expr_alias_edt->clear();
	tab_alias_edt->clear();
	expression_txt->clear();
	select_from_chk->setChecked(false);
	from_where_chk->setChecked(false);
	after_where_chk->setChecked(false);
}

void ViewWidget::selectReferenceType(void)
{
	//Marks if the select reference type treats a reference to an object
	bool ref_obj=(ref_type_cmb->currentIndex()==static_cast<int>(Reference::REFER_COLUMN));

	table_lbl->setVisible(ref_obj);
	column_lbl->setVisible(ref_obj);
	table_sel->setVisible(ref_obj);
	column_sel->setVisible(ref_obj);
	col_alias_lbl->setVisible(ref_obj);
	col_alias_edt->setVisible(ref_obj);
	tab_alias_edt->setVisible(ref_obj);
	tab_alias_lbl->setVisible(ref_obj);
	frame_info->setVisible(ref_obj);

	view_def_chk->setChecked(false);
	expression_lbl->setVisible(!ref_obj);
	expression_txt->setVisible(!ref_obj);
	expr_alias_edt->setVisible(!ref_obj);
	expr_alias_lbl->setVisible(!ref_obj);
	view_def_chk->setVisible(!ref_obj);
}

void ViewWidget::handleReference(int ref_idx)
{
	try
	{
		Reference ref;

		//Creating a reference to a column
		if(static_cast<unsigned>(ref_type_cmb->currentIndex())==Reference::REFER_COLUMN)
		{
			ref=Reference(dynamic_cast<Table *>(table_sel->getSelectedObject()),
										dynamic_cast<Column *>(column_sel->getSelectedObject()),
										tab_alias_edt->text().toUtf8(), col_alias_edt->text().toUtf8());
		}
		//Creating a reference to an expression
		else
		{
			ref=Reference(expression_txt->toPlainText(),
										expr_alias_edt->text().toUtf8());
		}

		/* The reference must have an SQL application (be between SELECT-FROM, FROM-WHERE or after WHERE),
			 if the user do not check some of these attributes raises an error */
		if(!select_from_chk->isChecked() &&
			 !from_where_chk->isChecked() &&
			 !after_where_chk->isChecked() &&
			 !view_def_chk->isChecked())
			throw Exception(ERR_SQL_SCOPE_INV_VIEW_REF,__PRETTY_FUNCTION__,__FILE__,__LINE__);


		if(view_def_chk->isChecked())
		{
			select_from_chk->setChecked(false);
			from_where_chk->setChecked(false);
			after_where_chk->setChecked(false);
		}

		showReferenceData(ref, select_from_chk->isChecked(), from_where_chk->isChecked(),
													after_where_chk->isChecked(), view_def_chk->isChecked() ,ref_idx);

		clearReferenceForm();
		references_tab->clearSelection();
	}
	catch(Exception &e)
	{
		if(references_tab->getCellText(ref_idx, 0).isEmpty())
			references_tab->removeRow(ref_idx);

		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void ViewWidget::editReference(int ref_idx)
{
	Reference ref;
	QString str_aux;

	//Get the reference at the selected table row
	ref=references_tab->getRowData(ref_idx).value<Reference>();
	ref_type_cmb->setCurrentIndex(ref.getReferenceType());

	if(ref.getReferenceType()==Reference::REFER_COLUMN)
	{
		if(ref.getColumn())
			column_sel->setSelectedObject(ref.getColumn());
		else
			table_sel->setSelectedObject(ref.getTable());

		col_alias_edt->setText(Utf8String::create(ref.getColumnAlias()));
		tab_alias_edt->setText(Utf8String::create(ref.getAlias()));
	}
	else
	{
		expression_txt->setPlainText(Utf8String::create(ref.getExpression()));
		expr_alias_edt->setText(Utf8String::create(ref.getAlias()));
	}

	str_aux=references_tab->getCellText(ref_idx,3);
	select_from_chk->setChecked(str_aux[0]=='1');
	from_where_chk->setChecked(str_aux[1]=='1');
	after_where_chk->setChecked(str_aux[2]=='1');
	view_def_chk->setChecked(str_aux[3]=='1');
}

void ViewWidget::showObjectName(void)
{
	Column *col=nullptr;
	QObject *obj_sender=sender();

	//If the sender is the table selector means that user want to reference all table columns
	if(obj_sender==table_sel)
	{
		column_sel->blockSignals(true);
		//Clears the column selector indicating the situation. (TABLE.*)
		column_sel->clearSelector();

		column_sel->blockSignals(false);
	}
	//If the sender is not the table sender the user wants to reference a specific column
	else
	{
		col=dynamic_cast<Column *>(column_sel->getSelectedObject());

		table_sel->blockSignals(true);

		if(col)
			table_sel->setSelectedObject(col->getParentTable());
		else
			table_sel->clearSelector();

		table_sel->blockSignals(false);
	}
}

void ViewWidget::showReferenceData(Reference refer, bool selec_from, bool from_where, bool after_where, bool view_def, unsigned row)
{
	Table *tab=nullptr;
	Column *col=nullptr;
	QString str_aux;

	if(refer.getReferenceType()==Reference::REFER_COLUMN)
	{
		tab=refer.getTable();
		col=refer.getColumn();

		/* If the table is allocated but not the column indicates that the reference
		 is to all table columns this way shows a string in format: [SCHEMA].[TABLE].* */
		if(tab && !col)
			references_tab->setCellText(Utf8String::create(tab->getName(true) + QString(".*")),row,0);
		/* If the table and column are allocated indicates that the reference
		 is to a specific column this way shows a string in format: [SCHEMA].[TABLE].[COLUMN] */
		else
			references_tab->setCellText(Utf8String::create(tab->getName(true) + QString(".") + col->getName(true)),row,0);

		references_tab->setCellText(Utf8String::create(refer.getAlias()),row,1);

		if(col)
			references_tab->setCellText(Utf8String::create(refer.getColumnAlias()),row,2);
	}
	else
	{
		references_tab->setCellText(Utf8String::create(refer.getExpression()),row,0);
		references_tab->setCellText(Utf8String::create(refer.getAlias()),row,1);
	}

	//Configures the string that denotes the SQL application for the reference
	str_aux+=(selec_from ? "1" : "0");
	str_aux+=(from_where ? "1" : "0");
	str_aux+=(after_where ? "1" : "0");
	str_aux+=(view_def ? "1" : "0");
	references_tab->setCellText(str_aux,row,3);

	refer.setDefinitionExpression(view_def);
	references_tab->setRowData(QVariant::fromValue<Reference>(refer), row);
}

void ViewWidget::updateCodePreview(void)
{
	try
	{
		if(tabWidget->currentIndex()==tabWidget->count()-1)
		{
			View aux_view;
			Reference refer;
			QString str_aux;
			TableObject *tab_obj=nullptr;
			map<ObjectType, ObjectTableWidget *>::iterator itr, itr_end;
			unsigned i, count, i1, expr_type[]={Reference::SQL_REFER_SELECT,
																					Reference::SQL_REFER_FROM,
																					Reference::SQL_REFER_WHERE,
																					Reference::SQL_VIEW_DEFINITION};

			aux_view.BaseObject::setName(name_edt->text().toUtf8());
			aux_view.BaseObject::setSchema(schema_sel->getSelectedObject());
			aux_view.setCommomTableExpression(cte_expression_txt->toPlainText().toUtf8());

			count=references_tab->getRowCount();
			for(i=0; i < count; i++)
			{
				refer=references_tab->getRowData(i).value<Reference>();

				//Get the SQL application string for the current reference
				str_aux=references_tab->getCellText(i,3);

				for(i1=0; i1 < sizeof(expr_type)/sizeof(unsigned); i1++)
				{
					if(str_aux[i1]=='1')
						aux_view.addReference(refer, expr_type[i1]);
				}
			}

			itr=objects_tab_map.begin();
			itr_end=objects_tab_map.end();

			//Inserts the triggers / rules into the auxiliary view
			while(itr!=itr_end)
			{
				count=itr->second->getRowCount();

				//Make a copy of each view objects (rule/trigger) to the auxiliary view
				for(i=0; i < count; i++)
				{
					if(itr->first==OBJ_TRIGGER)
					{
						tab_obj=new Trigger;
						(*dynamic_cast<Trigger *>(tab_obj))=
								(*reinterpret_cast<Trigger *>(itr->second->getRowData(i).value<void *>()));
					}
					else
					{
						tab_obj=new Rule;
						(*dynamic_cast<Rule *>(tab_obj))=
								(*reinterpret_cast<Rule *>(itr->second->getRowData(i).value<void *>()));
					}
					aux_view.addObject(tab_obj);
				}

				itr++;
			}
			code_txt->setPlainText(Utf8String::create(aux_view.getCodeDefinition(SchemaParser::SQL_DEFINITION)));
		}
	}
	catch(Exception &e)
	{
		QString str_aux;
		//In case of error no code is outputed, showing a error message in the code preview widget
		str_aux=trUtf8("/* Could not generate the SQL code. Make sure all attributes are correctly filled! ");
		str_aux+=QString("\n\n>> Returned error(s): \n\n%1*/").arg(e.getExceptionsText());
		code_txt->setPlainText(str_aux);
	}
}

void ViewWidget::setAttributes(DatabaseModel *model, OperationList *op_list, Schema *schema, View *view, float px, float py)
{
	unsigned i, count;
	bool sel_from, from_where, after_where, view_def;
	Reference refer;

	if(!view)
	{
		view=new View;

		if(schema)
			view->setSchema(schema);

		/* Sets the 'new_object' flag as true indicating that the alocated table must be treated
			 as a recently created object */
		this->new_object=true;
	}

	BaseObjectWidget::setAttributes(model,op_list, view, schema, px, py);

	op_list->startOperationChain();
	operation_count=op_list->getCurrentSize();

	if(this->new_object)
		op_list->registerObject(view, Operation::OBJECT_CREATED);

	column_sel->setModel(model);
	table_sel->setModel(model);

	if(view)
	{
		cte_expression_txt->setPlainText(Utf8String::create(view->getCommomTableExpression()));

		count=view->getReferenceCount();
		references_tab->blockSignals(true);

		for(i=0; i < count; i++)
		{
			references_tab->addRow();

			refer=view->getReference(i);
			sel_from=(view->getReferenceIndex(refer,Reference::SQL_REFER_SELECT) >= 0);
			from_where=(view->getReferenceIndex(refer,Reference::SQL_REFER_FROM) >= 0);
			after_where=(view->getReferenceIndex(refer,Reference::SQL_REFER_WHERE)>= 0);
			view_def=(view->getReferenceIndex(refer,Reference::SQL_VIEW_DEFINITION)>= 0);

			showReferenceData(refer, sel_from, from_where, after_where, view_def, i);
		}

		references_tab->blockSignals(false);
		references_tab->clearSelection();

		listObjects(OBJ_TRIGGER);
		listObjects(OBJ_RULE);
	}
}

void ViewWidget::applyConfiguration(void)
{
	try
	{
		View *view=nullptr;
		ObjectType types[]={ OBJ_TRIGGER, OBJ_RULE };
		unsigned expr_type[]={ Reference::SQL_REFER_SELECT,
													 Reference::SQL_REFER_FROM,
													 Reference::SQL_REFER_WHERE,
													 Reference::SQL_VIEW_DEFINITION};
		Reference refer;
		QString str_aux;

		if(!this->new_object)
			op_list->registerObject(this->object, Operation::OBJECT_MODIFIED);

		BaseObjectWidget::applyConfiguration();

		view=dynamic_cast<View *>(this->object);
		view->removeObjects();
		view->removeReferences();
		view->setCommomTableExpression(cte_expression_txt->toPlainText().toUtf8());

		for(unsigned i=0; i < references_tab->getRowCount(); i++)
		{
			refer=references_tab->getRowData(i).value<Reference>();

			//Get the SQL application string for the current reference
			str_aux=references_tab->getCellText(i,3);
			for(unsigned i=0; i < sizeof(expr_type)/sizeof(unsigned); i++)
			{
				if(str_aux[i]=='1')
					view->addReference(refer, expr_type[i]);
			}
		}

		//Adds the auxiliary view objects into configured view
		for(unsigned type_id=0; type_id < sizeof(types)/sizeof(ObjectType); type_id++)
		{
			for(unsigned i=0; i < objects_tab_map[types[type_id]]->getRowCount(); i++)
				view->addObject(reinterpret_cast<TableObject *>(objects_tab_map[types[type_id]]->getRowData(i).value<void *>()));
		}

		op_list->finishOperationChain();
		this->model->updateViewRelationships(view);
		finishConfiguration();
	}
	catch(Exception &e)
	{
		op_list->ignoreOperationChain(true);
		this->cancelConfiguration();
		op_list->ignoreOperationChain(false);
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void ViewWidget::cancelConfiguration(void)
{
	if(op_list->isOperationChainStarted())
		op_list->finishOperationChain();

	if(operation_count < op_list->getCurrentSize())
		BaseObjectWidget::cancelConfiguration();
}
