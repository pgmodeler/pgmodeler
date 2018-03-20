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

#include "tablewidget.h"
#include "columnwidget.h"
#include "constraintwidget.h"
#include "rulewidget.h"
#include "indexwidget.h"
#include "triggerwidget.h"
#include "baseform.h"
#include "tabledatawidget.h"
#include "policywidget.h"

TableWidget::TableWidget(QWidget *parent): BaseObjectWidget(parent, OBJ_TABLE)
{
	QGridLayout *grid=nullptr;
	ObjectsTableWidget *tab=nullptr;
	ObjectType types[]={ OBJ_COLUMN, OBJ_CONSTRAINT, OBJ_TRIGGER, OBJ_RULE, OBJ_INDEX, OBJ_POLICY };
	map<QString, vector<QWidget *> > fields_map;
	QFrame *frame=nullptr;
	QToolButton *edt_data_tb=nullptr;

	Ui_TableWidget::setupUi(this);

	edt_data_tb=new QToolButton(this);
	QPixmap icon=QPixmap(PgModelerUiNS::getIconPath("editdata"));
	edt_data_tb->setMinimumSize(edt_perms_tb->minimumSize());
	edt_data_tb->setText(trUtf8("Edit data"));
	edt_data_tb->setToolTip(trUtf8("Define initial data for the table"));
	edt_data_tb->setIcon(icon);
	edt_data_tb->setIconSize(edt_perms_tb->iconSize());
	edt_data_tb->setToolButtonStyle(edt_perms_tb->toolButtonStyle());

	connect(edt_data_tb, SIGNAL(clicked(bool)), this, SLOT(editData()));
	misc_btns_lt->insertWidget(1, edt_data_tb);

	fields_map[generateVersionsInterval(AFTER_VERSION, PgSQLVersions::PGSQL_VERSION_91)].push_back(unlogged_chk);
	fields_map[generateVersionsInterval(AFTER_VERSION, PgSQLVersions::PGSQL_VERSION_95)].push_back(enable_rls_chk);
	fields_map[generateVersionsInterval(AFTER_VERSION, PgSQLVersions::PGSQL_VERSION_95)].push_back(force_rls_chk);
	frame=generateVersionWarningFrame(fields_map);
	table_grid->addWidget(frame, table_grid->count()+1, 0, 1, 2);
	frame->setParent(this);

	parent_tables = new ObjectsTableWidget(ObjectsTableWidget::NO_BUTTONS, true, this);
	parent_tables->setColumnCount(3);
	parent_tables->setHeaderLabel(trUtf8("Name"), 0);
	parent_tables->setHeaderIcon(QPixmap(PgModelerUiNS::getIconPath("uid")),0);
	parent_tables->setHeaderLabel(trUtf8("Schema"), 1);
	parent_tables->setHeaderIcon(QPixmap(PgModelerUiNS::getIconPath("schema")),1);
	parent_tables->setHeaderLabel(trUtf8("Type"), 2);
	parent_tables->setHeaderIcon(QPixmap(PgModelerUiNS::getIconPath("usertype")),2);

	tag_sel=new ObjectSelectorWidget(OBJ_TAG, false, this);
	dynamic_cast<QGridLayout *>(options_gb->layout())->addWidget(tag_sel, 0, 1,1,3);

	grid=new QGridLayout;
	grid->addWidget(parent_tables, 0,0,1,1);
	grid->setContentsMargins(4,4,4,4);
	attributes_tbw->widget(6)->setLayout(grid);

	//Configuring the table objects that stores the columns, triggers, constraints, rules and indexes
	for(unsigned i=0; i <= 5; i++)
	{
		tab=new ObjectsTableWidget(ObjectsTableWidget::ALL_BUTTONS ^
								  (ObjectsTableWidget::UPDATE_BUTTON), true, this);

		objects_tab_map[types[i]]=tab;

		grid=new QGridLayout;
		grid->addWidget(tab, 0,0,1,1);
		grid->setContentsMargins(4,4,4,4);
		attributes_tbw->widget(i)->setLayout(grid);

		connect(tab, SIGNAL(s_rowsRemoved(void)), this, SLOT(removeObjects(void)));
		connect(tab, SIGNAL(s_rowRemoved(int)), this, SLOT(removeObject(int)));
		connect(tab, SIGNAL(s_rowAdded(int)), this, SLOT(handleObject(void)));
		connect(tab, SIGNAL(s_rowEdited(int)), this, SLOT(handleObject(void)));
		connect(tab, SIGNAL(s_rowDuplicated(int,int)), this, SLOT(duplicateObject(int,int)));
		connect(tab, SIGNAL(s_rowsMoved(int,int)), this, SLOT(swapObjects(int,int)));
	}

	objects_tab_map[OBJ_COLUMN]->setColumnCount(5);
	objects_tab_map[OBJ_COLUMN]->setHeaderLabel(trUtf8("PK"), 0);
	objects_tab_map[OBJ_COLUMN]->setHeaderLabel(trUtf8("Name"), 1);
	objects_tab_map[OBJ_COLUMN]->setHeaderIcon(QPixmap(PgModelerUiNS::getIconPath("uid")),1);
	objects_tab_map[OBJ_COLUMN]->setHeaderLabel(trUtf8("Type"), 2);
	objects_tab_map[OBJ_COLUMN]->setHeaderIcon(QPixmap(PgModelerUiNS::getIconPath("usertype")),2);
	objects_tab_map[OBJ_COLUMN]->setHeaderLabel(trUtf8("Default Value"), 3);
	objects_tab_map[OBJ_COLUMN]->setHeaderLabel(trUtf8("Attribute(s)"), 4);
	objects_tab_map[OBJ_COLUMN]->adjustColumnToContents(0);

	connect(objects_tab_map[OBJ_COLUMN], &ObjectsTableWidget::s_cellClicked, [&](int row, int col){
		if(col == 0 && objects_tab_map[OBJ_COLUMN]->isCellDisabled(static_cast<unsigned>(row), static_cast<unsigned>(col)))
		{
			Messagebox msg_box;
			Table *table = dynamic_cast<Table *>(this->object);
			Constraint *pk = table->getPrimaryKey();

			if(pk && pk->isAddedByRelationship())
				msg_box.show(trUtf8("It is not possible to mark a column as primary key when the table already has a primary key which was created by a relationship! This action should be done in the section <strong>Primary key</strong> of the relationship's editing form."), Messagebox::ALERT_ICON);
			else
				msg_box.show(trUtf8("It is not possible to mark a column created by a relationship as primary key! This action should be done in the section <strong>Primary key</strong> of the relationship's editing form."), Messagebox::ALERT_ICON);
		}
	});

	objects_tab_map[OBJ_CONSTRAINT]->setColumnCount(4);
	objects_tab_map[OBJ_CONSTRAINT]->setHeaderLabel(trUtf8("Name"), 0);
	objects_tab_map[OBJ_CONSTRAINT]->setHeaderIcon(QPixmap(PgModelerUiNS::getIconPath("uid")),0);
	objects_tab_map[OBJ_CONSTRAINT]->setHeaderLabel(trUtf8("Type"), 1);
	objects_tab_map[OBJ_CONSTRAINT]->setHeaderIcon(QPixmap(PgModelerUiNS::getIconPath("usertype")),1);
	objects_tab_map[OBJ_CONSTRAINT]->setHeaderLabel(trUtf8("ON DELETE"), 2);
	objects_tab_map[OBJ_CONSTRAINT]->setHeaderLabel(trUtf8("ON UPDATE"), 3);

	objects_tab_map[OBJ_TRIGGER]->setColumnCount(4);
	objects_tab_map[OBJ_TRIGGER]->setHeaderLabel(trUtf8("Name"), 0);
	objects_tab_map[OBJ_TRIGGER]->setHeaderIcon(QPixmap(PgModelerUiNS::getIconPath("uid")),0);
	objects_tab_map[OBJ_TRIGGER]->setHeaderLabel(trUtf8("Refer. Table"), 1);
	objects_tab_map[OBJ_TRIGGER]->setHeaderIcon(QPixmap(PgModelerUiNS::getIconPath("table")),1);
	objects_tab_map[OBJ_TRIGGER]->setHeaderLabel(trUtf8("Firing"), 2);
	objects_tab_map[OBJ_TRIGGER]->setHeaderIcon(QPixmap(PgModelerUiNS::getIconPath("trigger")),2);
	objects_tab_map[OBJ_TRIGGER]->setHeaderLabel(trUtf8("Events"), 3);

	objects_tab_map[OBJ_RULE]->setColumnCount(3);
	objects_tab_map[OBJ_RULE]->setHeaderLabel(trUtf8("Name"), 0);
	objects_tab_map[OBJ_RULE]->setHeaderIcon(QPixmap(PgModelerUiNS::getIconPath("uid")),0);
	objects_tab_map[OBJ_RULE]->setHeaderLabel(trUtf8("Execution"), 1);
	objects_tab_map[OBJ_RULE]->setHeaderLabel(trUtf8("Event"), 2);

	objects_tab_map[OBJ_INDEX]->setColumnCount(2);
	objects_tab_map[OBJ_INDEX]->setHeaderLabel(trUtf8("Name"), 0);
	objects_tab_map[OBJ_INDEX]->setHeaderIcon(QPixmap(PgModelerUiNS::getIconPath("uid")),0);
	objects_tab_map[OBJ_INDEX]->setHeaderLabel(trUtf8("Indexing"), 1);

	objects_tab_map[OBJ_POLICY]->setColumnCount(6);
	objects_tab_map[OBJ_POLICY]->setHeaderLabel(trUtf8("Name"), 0);
	objects_tab_map[OBJ_POLICY]->setHeaderIcon(QPixmap(PgModelerUiNS::getIconPath("uid")),0);
	objects_tab_map[OBJ_POLICY]->setHeaderLabel(trUtf8("Command"), 1);
	objects_tab_map[OBJ_POLICY]->setHeaderIcon(QPixmap(PgModelerUiNS::getIconPath("keyword")),1);
	objects_tab_map[OBJ_POLICY]->setHeaderLabel(trUtf8("Permissive"), 2);
	objects_tab_map[OBJ_POLICY]->setHeaderLabel(trUtf8("USING expression"), 3);
	objects_tab_map[OBJ_POLICY]->setHeaderLabel(trUtf8("CHECK expression"), 4);
	objects_tab_map[OBJ_POLICY]->setHeaderLabel(trUtf8("Roles"), 5);
	objects_tab_map[OBJ_POLICY]->setHeaderIcon(QPixmap(PgModelerUiNS::getIconPath("role")),5);

	configureFormLayout(table_grid, OBJ_TABLE);
	configureTabOrder({ tag_sel });

	setMinimumSize(600, 610);
}

void TableWidget::hideEvent(QHideEvent *event)
{
	map<ObjectType, ObjectsTableWidget *>::iterator itr, itr_end;
	Table *tab=dynamic_cast<Table *>(this->object);

	parent_tables->removeRows();
	with_oids_chk->setChecked(false);
	unlogged_chk->setChecked(false);
	attributes_tbw->setCurrentIndex(0);

	itr=objects_tab_map.begin();
	itr_end=objects_tab_map.end();
	while(itr!=itr_end)
	{
		(itr->second)->blockSignals(true);
		(itr->second)->removeRows();
		(itr->second)->blockSignals(false);
		itr++;
	}

	if(this->new_object && !tab->isModified())
		this->cancelConfiguration();

	BaseObjectWidget::hideEvent(event);
}

template<class Class, class WidgetClass>
int TableWidget::openEditingForm(TableObject *object)
{
	BaseForm editing_form(this);
	WidgetClass *object_wgt=new WidgetClass;
	object_wgt->setAttributes(this->model, this->op_list,
														dynamic_cast<Table *>(this->object), dynamic_cast<Class *>(object));
	editing_form.setMainWidget(object_wgt);

	editing_form.adjustSize();
	return(editing_form.exec());
}

ObjectsTableWidget *TableWidget::getObjectTable(ObjectType obj_type)
{
	if(objects_tab_map.count(obj_type) > 0)
		return(objects_tab_map[obj_type]);
	else
		return(nullptr);
}

ObjectType TableWidget::getObjectType(QObject *sender)
{
	ObjectType obj_type=BASE_OBJECT;

	if(sender)
	{
		map<ObjectType, ObjectsTableWidget *>::iterator itr, itr_end;

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

void TableWidget::setAttributes(DatabaseModel *model, OperationList *op_list, Schema *schema, Table *table, double pos_x, double pos_y)
{
	try
	{
		unsigned i, count;
		Table *aux_tab=nullptr;
		ObjectType types[]={ OBJ_COLUMN, OBJ_CONSTRAINT, OBJ_TRIGGER, OBJ_RULE, OBJ_INDEX, OBJ_POLICY };

		if(!table)
		{
			table=new Table;

			if(schema)
				table->setSchema(schema);

			/* Sets the 'new_object' flag as true indicating that the alocated table must be treated
				 as a recently created object */
			this->new_object=true;
		}

		BaseObjectWidget::setAttributes(model, op_list, table, schema, pos_x, pos_y);

		op_list->startOperationChain();
		operation_count=op_list->getCurrentSize();

		/* Listing all objects (column, constraint, trigger, index, rule) on the
		respective table objects */
		for(i=0; i < 6; i++)
		{
			listObjects(types[i]);
			objects_tab_map[types[i]]->setButtonConfiguration(ObjectsTableWidget::ALL_BUTTONS ^
															  (ObjectsTableWidget::UPDATE_BUTTON));
		}

		//Listing the ancestor tables
		count=table->getAncestorTableCount();
		for(i=0; i < count; i++)
		{
			aux_tab=table->getAncestorTable(i);
			parent_tables->addRow();
			parent_tables->setCellText(aux_tab->getName(), i, 0);
			parent_tables->setCellText(aux_tab->getSchema()->getName(), i, 1);
			parent_tables->setCellText(trUtf8("Parent"), i, 2);
		}

		aux_tab=table->getCopyTable();
		if(aux_tab)
		{
			parent_tables->addRow();
			parent_tables->setCellText(aux_tab->getName(), i, 0);
			parent_tables->setCellText(aux_tab->getSchema()->getName(), i, 1);
			parent_tables->setCellText(trUtf8("Copy"), i, 2);
		}

		parent_tables->clearSelection();
		with_oids_chk->setChecked(table->isWithOIDs());
		unlogged_chk->setChecked(table->isUnlogged());
		gen_alter_cmds_chk->setChecked(table->isGenerateAlterCmds());
		enable_rls_chk->setChecked(table->isRLSEnabled());
		force_rls_chk->setChecked(table->isRLSForced());

		tag_sel->setModel(this->model);
		tag_sel->setSelectedObject(table->getTag());
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void TableWidget::listObjects(ObjectType obj_type)
{
	ObjectsTableWidget *tab=nullptr;
	unsigned count, i;
	Table *table=nullptr;

	try
	{
		//Gets the object table related to the object type
		tab=objects_tab_map[obj_type];

		table=dynamic_cast<Table *>(this->object);

		tab->blockSignals(true);
		tab->removeRows();

		count=table->getObjectCount(obj_type);
		for(i=0; i < count; i++)
		{
			tab->addRow();
			showObjectData(dynamic_cast<TableObject*>(table->getObject(i, obj_type)), i);
		}
		tab->clearSelection();
		tab->blockSignals(false);

		//Enables the add button on the constraints, triggers and index tab only when there is columns created
		if(obj_type==OBJ_COLUMN)
		{
			objects_tab_map[OBJ_CONSTRAINT]->setButtonsEnabled(ObjectsTableWidget::ADD_BUTTON,
															   objects_tab_map[OBJ_COLUMN]->getRowCount() > 0);
			objects_tab_map[OBJ_TRIGGER]->setButtonsEnabled(ObjectsTableWidget::ADD_BUTTON,
															objects_tab_map[OBJ_COLUMN]->getRowCount() > 0);
			objects_tab_map[OBJ_INDEX]->setButtonsEnabled(ObjectsTableWidget::ADD_BUTTON,
														  objects_tab_map[OBJ_COLUMN]->getRowCount() > 0);
		}
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void TableWidget::handleObject(void)
{
	ObjectType obj_type=BASE_OBJECT;
	TableObject *object=nullptr;
	ObjectsTableWidget *obj_table=nullptr;

	try
	{
		obj_type=getObjectType(sender());

		//Selects the object table based upon the passed object type
		obj_table=getObjectTable(obj_type);

		//Gets the object reference if there is an item select on table
		if(obj_table->getSelectedRow()>=0)
			object=reinterpret_cast<TableObject *>(obj_table->getRowData(obj_table->getSelectedRow()).value<void *>());

		if(obj_type==OBJ_COLUMN)
			openEditingForm<Column, ColumnWidget>(object);
		else if(obj_type==OBJ_CONSTRAINT)
			openEditingForm<Constraint, ConstraintWidget>(object);
		else if(obj_type==OBJ_TRIGGER)
			openEditingForm<Trigger, TriggerWidget>(object);
		else if(obj_type==OBJ_INDEX)
			openEditingForm<Index, IndexWidget>(object);
		else if(obj_type==OBJ_RULE)
			openEditingForm<Rule, RuleWidget>(object);
		else
			openEditingForm<Policy, PolicyWidget>(object);

		listObjects(obj_type);

		if(obj_type == OBJ_CONSTRAINT)
			listObjects(OBJ_COLUMN);
	}
	catch(Exception &e)
	{
		listObjects(obj_type);
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void TableWidget::showObjectData(TableObject *object, int row)
{
	ObjectsTableWidget *tab=nullptr;
	Column *column=nullptr;
	Constraint *constr=nullptr;
	Trigger *trigger=nullptr;
	Rule *rule=nullptr;
	Index *index=nullptr;
	Policy *policy=nullptr;
	ObjectType obj_type;
	QString str_aux, str_aux1;

	QStringList contr_types={ ~ConstraintType(ConstraintType::primary_key), ~ConstraintType(ConstraintType::foreign_key),
							  ~ConstraintType(ConstraintType::check), ~ConstraintType(ConstraintType::unique),
							  QString("NOT NULL") },
			constr_codes={ TableObjectView::TXT_PRIMARY_KEY,
										 TableObjectView::TXT_FOREIGN_KEY,
										 TableObjectView::TXT_CHECK,
										 TableObjectView::TXT_UNIQUE,
										 TableObjectView::TXT_NOT_NULL};

	QFont font;
	unsigned i;
	EventType events[]={ EventType::on_insert, EventType::on_delete,
						 EventType::on_truncate,	EventType::on_update };

	obj_type=object->getObjectType();
	tab=objects_tab_map[obj_type];


	if(obj_type==OBJ_COLUMN)
		tab->setCellText(object->getName(),row,1);
	else
		tab->setCellText(object->getName(),row,0);

	//For each object type there is a use for the columns from 1 to 3
	if(obj_type==OBJ_COLUMN)
	{
		Table *table = dynamic_cast<Table *>(this->object);
		Constraint *pk = table->getPrimaryKey();
		column=dynamic_cast<Column *>(object);

		//Column 2: Column data type
		tab->setCellText(*column->getType(),row,2);

		//Column 3: Column defaul value
		if(column->getSequence())
			str_aux=QString("nextval('%1'::regclass)").arg(column->getSequence()->getName(true).remove('"'));
		else if(column->getIdentityType() != BaseType::null)
			str_aux=QString("GENERATED %1 AS IDENTITY").arg(~column->getIdentityType());
		else
			str_aux=column->getDefaultValue();

		if(str_aux.isEmpty()) str_aux=QString("-");
		tab->setCellText(str_aux,row,3);

		//Column 4: Column attributes (constraints which belongs)
		str_aux=TableObjectView::getConstraintString(column);
		for(int i=0; i < constr_codes.size(); i++)
		{
			if(str_aux.indexOf(constr_codes[i]) >= 0)
				str_aux1+=contr_types[i] + QString(", ");
		}

		if(str_aux1.isEmpty())
			str_aux1=QString("-");
		else
			str_aux1.remove(str_aux1.size()-2, 2);

		tab->setCellText(str_aux1,row,4);

		if(str_aux.indexOf(TableObjectView::TXT_PRIMARY_KEY) >= 0)
			tab->setCellCheckState(row, 0, Qt::Checked);
		else
			tab->setCellCheckState(row, 0, Qt::Unchecked);

		if(column->isAddedByRelationship() || (pk && pk->isAddedByRelationship()))
			tab->setCellDisabled(row, 0, true);

		tab->adjustColumnToContents(0);
	}
	else if(obj_type==OBJ_CONSTRAINT)
	{
		constr=dynamic_cast<Constraint *>(object);

		//Column 1: Constraint type
		tab->setCellText(~constr->getConstraintType(),row,1);

		if(constr->getConstraintType()==ConstraintType::foreign_key)
		{
			//Column 2: ON DELETE action
			tab->setCellText(~constr->getActionType(false),row,2);

			//Column 3: ON UPDATE action
			tab->setCellText(~constr->getActionType(true),row,3);
		}
		else
		{
			tab->setCellText(QString("-"),row,2);
			tab->setCellText(QString("-"),row,3);
		}
	}
	else if(obj_type==OBJ_TRIGGER)
	{
		trigger=dynamic_cast<Trigger *>(object);

		//Column 1: Table referenced by the trigger (constraint trigger)
		tab->clearCellText(row,1);
		if(trigger->getReferencedTable())
			tab->setCellText(trigger->getReferencedTable()->getName(true),row,1);

		//Column 2: Trigger firing type
		tab->setCellText(~trigger->getFiringType(),row,2);

		//Column 3: Events that fires the trigger
		for(i=0; i < 4; i++)
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
	else if(obj_type==OBJ_INDEX)
	{
		index=dynamic_cast<Index *>(object);

		//Coluna 1: Indexing type
		tab->setCellText(~index->getIndexingType(),row,1);
	}
	else if(obj_type==OBJ_POLICY)
	{
		QStringList rol_names;

		policy = dynamic_cast<Policy *>(object);

		//Column 1: Command
		tab->setCellText(~policy->getPolicyCommand(), row, 1);

		//Column 2: Permissive
		tab->setCellText(QString("%1").arg(policy->isPermissive() ? trUtf8("Yes") : trUtf8("No")), row, 2);

		//Column 3: USING expression
		tab->setCellText(policy->getUsingExpression(), row, 3);

		//Column 4: CHECK expression
		tab->setCellText(policy->getCheckExpression(), row, 4);

		for(auto role : policy->getRoles())
			rol_names.append(role->getName());

		//Column 5: Roles
		tab->setCellText(!rol_names.isEmpty() ? rol_names.join(", ") : QString("PUBLIC"), row, 5);
	}

	//Changes the foreground/background color of the table row if the object is protected or added by relationship
	if(object->isAddedByRelationship() || object->isProtected())
	{
		font=tab->font();
		font.setItalic(true);

		if(object->isAddedByRelationship())
			tab->setRowFont(row, font, RELINC_LINE_FGCOLOR, RELINC_LINE_BGCOLOR);
		else
			tab->setRowFont(row, font, PROT_LINE_FGCOLOR, PROT_LINE_BGCOLOR);
	}

	tab->setRowData(QVariant::fromValue<void *>(object), row);
}

void TableWidget::removeObjects(void)
{
	Table *table=nullptr;
	unsigned count, op_count=0, i;
	BaseObject *object=nullptr;
	ObjectType obj_type=BASE_OBJECT;

	try
	{
		table=dynamic_cast<Table *>(this->object);

		obj_type=getObjectType(sender());
		count=table->getObjectCount(obj_type);
		op_count=op_list->getCurrentSize();

		for(i=0; i < count; i++)
		{
			object=table->getObject(0, obj_type);

			if(!object->isProtected() &&
					!dynamic_cast<TableObject *>(object)->isAddedByRelationship())
			{
				op_list->registerObject(object, Operation::OBJECT_REMOVED, 0, this->object);
				table->removeObject(object);
			}
			else
				throw Exception(Exception::getErrorMessage(ERR_REM_PROTECTED_OBJECT)
								.arg(object->getName())
								.arg(object->getTypeName()),
								ERR_REM_PROTECTED_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		}

		if(obj_type == OBJ_CONSTRAINT)
			listObjects(OBJ_COLUMN);
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

void TableWidget::removeObject(int row)
{
	Table *table=nullptr;
	BaseObject *object=nullptr;
	ObjectType obj_type=BASE_OBJECT;
	int op_id=-1;

	try
	{
		table=dynamic_cast<Table *>(this->object);
		obj_type=getObjectType(sender());

		object=table->getObject(row, obj_type);

		if(!object->isProtected() &&
				!dynamic_cast<TableObject *>(object)->isAddedByRelationship())
		{
			op_id=op_list->registerObject(object, Operation::OBJECT_REMOVED, row, this->object);
			table->removeObject(object);
			table->setModified(true);
		}
		else
			throw Exception(Exception::getErrorMessage(ERR_REM_PROTECTED_OBJECT)
							.arg(object->getName())
							.arg(object->getTypeName()),
							ERR_REM_PROTECTED_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		if(obj_type == OBJ_CONSTRAINT)
			listObjects(OBJ_COLUMN);
	}
	catch(Exception &e)
	{
		//If operation was registered
		if(op_id >= 0)
		{
			op_list->ignoreOperationChain(true);
			op_list->removeLastOperation();
			op_list->ignoreOperationChain(false);
		}

		listObjects(obj_type);
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void TableWidget::duplicateObject(int sel_row, int new_row)
{
	ObjectType obj_type=BASE_OBJECT;
	BaseObject *object=nullptr, *dup_object=nullptr;
	ObjectsTableWidget *obj_table=nullptr;
	Table *table = dynamic_cast<Table *>(this->object);
	int op_id = -1;

	try
	{
		obj_type=getObjectType(sender());

		//Selects the object table based upon the passed object type
		obj_table=getObjectTable(obj_type);

		//Gets the object reference if there is an item select on table
		if(sel_row >= 0)
			object = reinterpret_cast<BaseObject *>(obj_table->getRowData(sel_row).value<void *>());

		PgModelerNS::copyObject(&dup_object, object, obj_type);
		dup_object->setName(PgModelerNS::generateUniqueName(dup_object, *table->getObjectList(obj_type), false, QString("_cp")));

		op_id=op_list->registerObject(dup_object, Operation::OBJECT_CREATED, new_row, this->object);

		table->addObject(dup_object);
		table->setModified(true);
		listObjects(obj_type);
	}
	catch(Exception &e)
	{
		//If operation was registered
		if(op_id >= 0)
		{
			op_list->ignoreOperationChain(true);
			op_list->removeLastOperation();
			op_list->ignoreOperationChain(false);
		}

		listObjects(obj_type);
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void TableWidget::TableWidget::swapObjects(int idx1, int idx2)
{
	ObjectType obj_type=BASE_OBJECT;
	Table *table=nullptr;
	int count;

	try
	{
		obj_type=getObjectType(sender());
		table=dynamic_cast<Table *>(this->object);
		count=table->getObjectCount(obj_type);

		if(idx1 >= count)
			//Special case 1: the object was moved to the first row, its index is swapped with index 0
			op_list->updateObjectIndex(table->getObject(idx2, obj_type), 0);
		else if(idx2 >= count)
			//Special case 2: the object was moved to the last row, its index is swapped with index count-1
			op_list->updateObjectIndex(table->getObject(idx1, obj_type), count-1);
		else
		{
			op_list->updateObjectIndex(table->getObject(idx1, obj_type), idx2);
			op_list->updateObjectIndex(table->getObject(idx2, obj_type), idx1);
		}

		table->swapObjectsIndexes(obj_type, idx1, idx2);
	}
	catch(Exception &e)
	{
		listObjects(obj_type);
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void TableWidget::editData(void)
{
	BaseForm base_form(this);
	TableDataWidget *tab_data_wgt=new TableDataWidget(this);

	tab_data_wgt->setAttributes(this->model, dynamic_cast<Table *>(this->object));
	base_form.setMainWidget(tab_data_wgt);
	base_form.setButtonConfiguration(Messagebox::OK_CANCEL_BUTTONS);
	base_form.exec();
}

void TableWidget::applyConfiguration(void)
{
	try
	{
		Table *table=nullptr;
		Constraint *pk = nullptr;
		vector<BaseRelationship *> rels;
		vector<Column *> pk_cols;
		ObjectsTableWidget *col_tab = objects_tab_map[OBJ_COLUMN];

		if(!this->new_object)
			op_list->registerObject(this->object, Operation::OBJECT_MODIFIED);
		else
			registerNewObject();

		table=dynamic_cast<Table *>(this->object);
		table->setWithOIDs(with_oids_chk->isChecked());
		table->setGenerateAlterCmds(gen_alter_cmds_chk->isChecked());
		table->setRLSEnabled(enable_rls_chk->isChecked());
		table->setRLSForced(force_rls_chk->isChecked());
		table->setUnlogged(unlogged_chk->isChecked());
		table->setTag(dynamic_cast<Tag *>(tag_sel->getSelectedObject()));

		BaseObjectWidget::applyConfiguration();

		//Retrieving all columns marked as primary key
		for(unsigned row = 0; row < col_tab->getRowCount(); row++)
		{
			if(col_tab->getCellCheckState(row, 0) == Qt::Checked)
				pk_cols.push_back(reinterpret_cast<Column *>(col_tab->getRowData(row).value<void *>()));
		}

		pk = table->getPrimaryKey();

		//If there is at least one column marked as pk
		if(!pk_cols.empty())
		{
			if(!pk)
			{
				//Create the primary key if the table does not own one
				QString pk_name = QString("%1_pk").arg(table->getName());

				pk = new Constraint;
				pk->setName(pk_name);
				pk->setName(PgModelerNS::generateUniqueName(pk, *table->getObjectList(OBJ_CONSTRAINT)));

				for(Column *col : pk_cols)
					pk->addColumn(col, Constraint::SOURCE_COLS);

				table->addConstraint(pk);
				op_list->registerObject(pk, Operation::OBJECT_CREATED, -1, table);
			}
			else if(!pk->isAddedByRelationship())
			{
				//If the table owns a pk we only update the columns
				op_list->registerObject(pk, Operation::OBJECT_MODIFIED, -1, table);
				pk->removeColumns();

				for(Column *col : pk_cols)
					pk->addColumn(col, Constraint::SOURCE_COLS);
			}
		}
		else if(pk_cols.empty() && pk && !pk->isAddedByRelationship())
		{
			//Removing the primary key from the table when no column is checked as pk
			op_list->registerObject(pk, Operation::OBJECT_REMOVED, -1, table);
			table->removeObject(pk);
		}

		try
		{
			table->saveRelObjectsIndexes();

			if(model->getRelationship(table, nullptr))
				model->validateRelationships();

			model->updateTableFKRelationships(table);
		}
		catch(Exception &e)
		{
			Messagebox msg_box;

			if(e.getErrorType()==ERR_INVALIDATED_OBJECTS)
				msg_box.show(e);
			else
				throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
		}

		op_list->finishOperationChain();
		finishConfiguration();

		if(RelationshipView::getLineConnectinMode()==RelationshipView::CONNECT_FK_TO_PK)
		{
			/* Forcing the update of relationships connected to the table in order to reconfigure the line
			 in case of the relationship is using the CONNECT_FK_TO_PK line mode */
			rels=model->getRelationships(table);
			for(auto &rel : rels)
			{
				if(rel->getRelationshipType()==Relationship::RELATIONSHIP_11 ||
						rel->getRelationshipType()==Relationship::RELATIONSHIP_1N ||
						rel->getRelationshipType()==Relationship::RELATIONSHIP_FK)
					rel->setModified(true);
			}
		}
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void TableWidget::cancelConfiguration(void)
{
	BaseObjectWidget::cancelChainedOperation();
}

