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

ViewWidget::ViewWidget(QWidget *parent): BaseObjectWidget(parent, OBJ_VIEW)
{
	try
	{
		Ui_ViewWidget::setupUi(this);

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

		table_sel=new ObjectSelectorWidget(OBJ_TABLE, true, this);
		column_sel=new ObjectSelectorWidget(OBJ_COLUMN, true, this);

		references_tab=new ObjectTableWidget(ObjectTableWidget::ALL_BUTTONS, true, this);
		references_tab->setColumnCount(4);
		references_tab->setHeaderLabel(trUtf8("Col./Expr."),0);
		references_tab->setHeaderLabel(trUtf8("Alias"),1);
		references_tab->setHeaderLabel(trUtf8("Alias Col."),2);
		references_tab->setHeaderLabel(trUtf8("SF FW AW"),3);

		frame_info=generateInformationFrame(trUtf8("To reference all columns in a table (*) just do not fill the field <strong>Column</strong>, this is the same as write <em><strong>[schema].[tablel].*</strong></em>"));

		referencias_grid->addWidget(table_sel, 2,1,1,2);
		referencias_grid->addWidget(column_sel, 3,1,1,2);
		referencias_grid->addWidget(frame_info, 6, 0, 1, 0);
		referencias_grid->addWidget(references_tab, 7,0,2,0);

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

		parent_form->setMinimumSize(650, 630);
		selectReferenceType();
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void ViewWidget::hideEvent(QHideEvent *evento)
{
	references_tab->removeRows();
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

	expression_lbl->setVisible(!ref_obj);
	expression_txt->setVisible(!ref_obj);
	expr_alias_edt->setVisible(!ref_obj);
	expr_alias_lbl->setVisible(!ref_obj);
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
			 !after_where_chk->isChecked())
			throw Exception(ERR_SQL_SCOPE_INV_VIEW_REF,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		showReferenceData(ref, select_from_chk->isChecked(), from_where_chk->isChecked(),
													after_where_chk->isChecked(), ref_idx);

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
}

void ViewWidget::showObjectName(void)
{
	Column *col=NULL;
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

void ViewWidget::showReferenceData(Reference refer, bool selec_from, bool from_where, bool after_where, unsigned row)
{
	Table *tab=NULL;
	Column *col=NULL;
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
		else
			references_tab->setCellText(QString("-"),row,2);
	}
	else
	{
		references_tab->setCellText(Utf8String::create(refer.getExpression()),row,0);
		references_tab->setCellText(Utf8String::create(refer.getAlias()),row,1);
		references_tab->setCellText(QString("-"),row,2);
	}

	//Configures the string that denotes the SQL application for the reference
	str_aux+=(selec_from ? "1" : "0");
	str_aux+=(from_where ? "1" : "0");
	str_aux+=(after_where ? "1" : "0");
	references_tab->setCellText(str_aux,row,3);

	references_tab->setRowData(QVariant::fromValue<Reference>(refer), row);
	updateCodePreview();
}

void ViewWidget::updateCodePreview(void)
{
	Reference refer;
	QString str_aux;
	unsigned i, count, i1, expr_type[3]={Reference::SQL_REFER_SELECT,
																		Reference::SQL_REFER_FROM,
																		Reference::SQL_REFER_WHERE};
	try
	{
		//Clears the auxiliary view
		aux_view.removeReferences();

		aux_view.BaseObject::setName(name_edt->text().toUtf8());
		aux_view.setSchema(schema_sel->getSelectedObject());

		count=references_tab->getRowCount();
		for(i=0; i < count; i++)
		{
			refer=references_tab->getRowData(i).value<Reference>();

			//Get the SQL application string for the current reference
			str_aux=references_tab->getCellText(i,3);

			for(i1=0; i1 < 3; i1++)
			{
				if(str_aux[i1]=='1')
					aux_view.addReference(refer, expr_type[i1]);
			}
		}

		code_txt->setPlainText(Utf8String::create(aux_view.getCodeDefinition(SchemaParser::SQL_DEFINITION)));
	}
	catch(Exception &e)
	{
		//In case of error no code is outputed, showing a error message in the code preview widget
		code_txt->setPlainText(trUtf8("-- Could not generate the code. Make sure all attributes are correctly filled! --"));
	}
}

void ViewWidget::setAttributes(DatabaseModel *model, OperationList *op_list, Schema *schema, View *view, float px, float py)
{
	unsigned i, count;
	bool sel_from, from_where, after_where;
	Reference refer;

	BaseObjectWidget::setAttributes(model,op_list, view, schema, px, py);

	column_sel->setModel(model);
	table_sel->setModel(model);

	if(view)
	{
		count=view->getReferenceCount();

		references_tab->blockSignals(true);
		for(i=0; i < count; i++)
		{
			references_tab->addRow();

			refer=view->getReference(i);
			sel_from=(view->getReferenceIndex(refer,Reference::SQL_REFER_SELECT) >= 0);
			from_where=(view->getReferenceIndex(refer,Reference::SQL_REFER_FROM) >= 0);
			after_where=(view->getReferenceIndex(refer,Reference::SQL_REFER_WHERE)>= 0);

			showReferenceData(refer, sel_from, from_where, after_where, i);
		}

		references_tab->blockSignals(false);
		references_tab->clearSelection();
	}
}

void ViewWidget::applyConfiguration(void)
{
	try
	{
		View *view=NULL;

		startConfiguration<View>();

		view=dynamic_cast<View *>(this->object);
		(*view)=aux_view;

		BaseObjectWidget::applyConfiguration();

		this->model->updateViewRelationships(view);

		finishConfiguration();
	}
	catch(Exception &e)
	{
		cancelConfiguration();
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

