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

#include "columnwidget.h"
#include "sequencewidget.h"
#include "baseform.h"
#include "generalconfigwidget.h"

ColumnWidget::ColumnWidget(QWidget *parent): BaseObjectWidget(parent, ObjectType::Column)
{
	try
	{
		QSpacerItem *spacer=new QSpacerItem(10,10,QSizePolicy::Fixed,QSizePolicy::Expanding);
		QStringList list;

		Ui_ColumnWidget::setupUi(this);
		edit_seq_btn->setVisible(false);

		IdentityType::getTypes(list);
		identity_type_cmb->addItems(list);

		data_type=nullptr;
		data_type=new PgSQLTypeWidget(this);

		hl_default_value=nullptr;
		hl_default_value=new SyntaxHighlighter(def_value_txt, true);
		hl_default_value->loadConfiguration(GlobalAttributes::SQLHighlightConfPath);

		sequence_sel=new ObjectSelectorWidget(ObjectType::Sequence, true, this);
		sequence_sel->setEnabled(false);

		column_grid->addWidget(data_type,0,0,1,0);
		column_grid->addWidget(default_value_grp,1,0,1,1);

		column_grid->addItem(spacer,column_grid->count(),0);
		dynamic_cast<QGridLayout *>(default_value_grp->layout())->addWidget(sequence_sel, 1, 1, 1, 6);

		configureFormLayout(column_grid, ObjectType::Column);
		configureTabOrder({ data_type });

		map<QString, vector<QWidget *> > fields_map;
		fields_map[generateVersionsInterval(AFTER_VERSION, PgSqlVersions::PgSqlVersion100)].push_back(identity_rb);
		highlightVersionSpecificFields(fields_map);

		connect(sequence_rb, &QRadioButton::clicked,
				[&](){
						sequence_sel->setEnabled(true);
						def_value_txt->setEnabled(false);
						identity_type_cmb->setEnabled(false);
						notnull_chk->setEnabled(true);
						edit_seq_btn->setVisible(false);
				});

		connect(expression_rb, &QRadioButton::clicked,
				[&](){
						sequence_sel->setEnabled(false);
						def_value_txt->setEnabled(true);
						identity_type_cmb->setEnabled(false);
						notnull_chk->setEnabled(true);
						edit_seq_btn->setVisible(false);
				});

		connect(identity_rb, &QRadioButton::clicked,
				[&](){
						sequence_sel->setEnabled(false);
						def_value_txt->setEnabled(false);
						identity_type_cmb->setEnabled(true);
						notnull_chk->setChecked(true);
						notnull_chk->setEnabled(false);
						edit_seq_btn->setVisible(true);
				});

		connect(edit_seq_btn, SIGNAL(clicked(bool)), this, SLOT(editSequenceAttributes()));

		setMinimumSize(540, 480);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void ColumnWidget::setAttributes(DatabaseModel *model, OperationList *op_list, BaseObject *parent_obj, Column *column)
{
	PgSqlType type;

	if(!parent_obj)
		throw Exception(ErrorCode::AsgNotAllocattedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	BaseObjectWidget::setAttributes(model, op_list, column, parent_obj);
	sequence_sel->setModel(model);

	ident_col_seq.setValues(QString(), QString(), QString(), QString(), QString());
	ident_col_seq.setCycle(false);

	if(column)
	{
		type=column->getType();
		notnull_chk->setChecked(column->isNotNull());
		def_value_txt->setPlainText(column->getDefaultValue());

		if(column->getSequence())
		{
			sequence_rb->click();
			sequence_sel->setEnabled(true);
			sequence_sel->setSelectedObject(column->getSequence());
		}
		else if(column->getIdentityType() != BaseType::Null)
		{
			identity_rb->click();
			identity_type_cmb->setEnabled(true);
			identity_type_cmb->setCurrentText(~column->getIdentityType());
			notnull_chk->setEnabled(false);
		}
	}

	data_type->setAttributes(type, model,
													 UserTypeConfig::BaseType | UserTypeConfig::TableType | UserTypeConfig::ViewType |
													 UserTypeConfig::DomainType | UserTypeConfig::ExtensionType, true,false);
}

void ColumnWidget::editSequenceAttributes(void)
{
	Column *col = dynamic_cast<Column *>(this->object);
	Schema *schema = nullptr;
	BaseForm editing_form(this);
	SequenceWidget *seq_wgt=new SequenceWidget;
	BaseTable *table = col ? col->getParentTable() : nullptr;

	if(table)
		schema = dynamic_cast<Schema *>(table->getSchema());
	else
		schema = this->model->getSchema("public");

	ident_col_seq.setName(QString("%1_%2_seq").arg(table ? table->getName() : QString()).arg(col ? col->getName() : QString("new_column")));
	ident_col_seq.setName(PgModelerNs::generateUniqueName(&ident_col_seq, *model->getObjectList(ObjectType::Sequence), false));
	ident_col_seq.setSchema(schema);

	if(col)
	{
		ident_col_seq.setDefaultValues(col->getType());
		ident_col_seq.setValues(col->getIdSeqMinValue(), col->getIdSeqMaxValue(), col->getIdSeqIncrement(), col->getIdSeqStart(), col->getIdSeqCache());
		ident_col_seq.setCycle(col->isIdSeqCycle());
	}

	seq_wgt->setAttributesReadonly(this->model, nullptr, nullptr, &ident_col_seq, col);
	editing_form.setMainWidget(seq_wgt);
	GeneralConfigWidget::restoreWidgetGeometry(&editing_form, seq_wgt->metaObject()->className());
	editing_form.exec();
	GeneralConfigWidget::saveWidgetGeometry(&editing_form, seq_wgt->metaObject()->className());
}

void ColumnWidget::applyConfiguration(void)
{
	try
	{
		Column *column=nullptr;
		Constraint *pk = nullptr;
		startConfiguration<Column>();

		column=dynamic_cast<Column *>(this->object);
		column->setNotNull(notnull_chk->isChecked());
		column->setType(data_type->getPgSQLType());

		if(expression_rb->isChecked())
			column->setDefaultValue(def_value_txt->toPlainText());
		else if(sequence_rb->isChecked())
			column->setSequence(sequence_sel->getSelectedObject());
		else
			column->setIdentityType(IdentityType(identity_type_cmb->currentText()));

		column->setIdSeqAttributes(ident_col_seq.getMinValue(), ident_col_seq.getMaxValue(), ident_col_seq.getIncrement(),
															 ident_col_seq.getStart(), ident_col_seq.getCache(), ident_col_seq.isCycle());

		if(table)
		{
			pk = dynamic_cast<Table *>(table)->getPrimaryKey();
			if(pk && pk->isColumnReferenced(column) && !notnull_chk->isChecked())
				throw Exception(Exception::getErrorMessage(ErrorCode::NullPrimaryKeyColumn)
												.arg(column->getName())
												.arg(pk->getParentTable()->getSignature(true)),
												ErrorCode::NullPrimaryKeyColumn,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		}

		BaseObjectWidget::applyConfiguration();
		model->updateViewsReferencingTable(dynamic_cast<Table *>(table));

		finishConfiguration();
	}
	catch(Exception &e)
	{
		cancelConfiguration();
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

