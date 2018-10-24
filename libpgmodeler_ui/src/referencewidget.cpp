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

#include "referencewidget.h"
#include "baseobjectwidget.h"

ReferenceWidget::ReferenceWidget(QWidget *parent) : QWidget(parent)
{
	setupUi(this);

	ref_flags = 0;
	ref_alias_ht=new HintTextWidget(ref_alias_hint, this);
	ref_alias_ht->setText(ref_alias_edt->statusTip());

	used_in_ht=new HintTextWidget(used_in_hint, this);
	used_in_ht->setText(select_from_chk->statusTip());

	ref_object_ht=new HintTextWidget(ref_object_hint, this);
	ref_object_ht->setText(trUtf8("To reference all columns of a table select only a table in the object selector, this is the same as write <em><strong>[schema].[table].*</strong></em>. In order to reference a only a single column of a table select a column object in the selector."));

	expression_txt=new NumberedTextEditor(this, true);
	expression_hl=new SyntaxHighlighter(expression_txt, false, true);
	expression_hl->loadConfiguration(GlobalAttributes::SQLHighlightConfPath);

	ref_object_sel=new ObjectSelectorWidget({ ObjTable, ObjColumn }, true, this);
	ref_object_sel->enableObjectCreation(false);
	expression_cp=new CodeCompletionWidget(expression_txt, true);

	reference_grid->addWidget(ref_object_sel, 3, 1, 1, 3);
	reference_grid->addWidget(expression_txt, 5, 1, 1, 4);

	selectReferenceType();
	setMinimumSize(630, 380);

	connect(view_def_chk, SIGNAL(toggled(bool)), select_from_chk, SLOT(setDisabled(bool)));
	connect(view_def_chk, SIGNAL(toggled(bool)), from_where_chk, SLOT(setDisabled(bool)));
	connect(view_def_chk, SIGNAL(toggled(bool)), after_where_chk, SLOT(setDisabled(bool)));
	connect(view_def_chk, SIGNAL(toggled(bool)), end_expr_chk, SLOT(setDisabled(bool)));
	connect(ref_type_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(selectReferenceType(void)));

	connect(ref_object_sel, &ObjectSelectorWidget::s_objectSelected, [&](){
		col_alias_edt->setEnabled(dynamic_cast<Column *>(ref_object_sel->getSelectedObject()));
	});

	connect(ref_object_sel, &ObjectSelectorWidget::s_selectorCleared, [&](){
		col_alias_edt->setEnabled(false);
	});
}

void ReferenceWidget::setAttributes(Reference ref, unsigned ref_flags, DatabaseModel *model)
{
	this->ref_flags = ref_flags;
	this->reference = ref;

	expression_cp->configureCompletion(model, expression_hl);
	ref_object_sel->setModel(model);

	ref_type_cmb->setCurrentIndex(ref.getReferenceType());
	ref_alias_edt->setText(ref.getReferenceAlias());

	if(ref.getReferenceType()==Reference::ReferColumn)
	{
		if(ref.getColumn())
			ref_object_sel->setSelectedObject(ref.getColumn());
		else
			ref_object_sel->setSelectedObject(ref.getTable());

		tab_alias_edt->setText(ref.getAlias());
		col_alias_edt->setText(ref.getColumnAlias());
	}
	else
	{
		expression_txt->setPlainText(ref.getExpression());
		expr_alias_edt->setText(ref.getAlias());
	}

	if(ref_flags == Reference::SqlViewDefinition)
		view_def_chk->setChecked(true);
	else
	{
		select_from_chk->setChecked((ref_flags & Reference::SqlReferSelect) == Reference::SqlReferSelect);
		from_where_chk->setChecked((ref_flags & Reference::SqlReferFrom) == Reference::SqlReferFrom);
		after_where_chk->setChecked((ref_flags & Reference::SqlReferWhere) == Reference::SqlReferWhere);
		end_expr_chk->setChecked((ref_flags & Reference::SqlReferEndExpr) == Reference::SqlReferEndExpr);
	}
}

Reference ReferenceWidget::getReference(void)
{
	return(reference);
}

unsigned ReferenceWidget::getReferenceFlags(void)
{
	return(ref_flags);
}

void ReferenceWidget::applyConfiguration(void)
{
	try
	{
		//Creating a reference to a column
		if(static_cast<unsigned>(ref_type_cmb->currentIndex())==Reference::ReferColumn)
		{
			Column *column = dynamic_cast<Column *>(ref_object_sel->getSelectedObject());
			Table *table = (column ? dynamic_cast<Table *>(column->getParentTable()) :
															 dynamic_cast<Table *>(ref_object_sel->getSelectedObject()));
			reference = Reference(table, column,	tab_alias_edt->text(), col_alias_edt->text());
		}
		//Creating a reference to an expression
		else
			reference = Reference(expression_txt->toPlainText(), expr_alias_edt->text().toUtf8());

		reference.setReferenceAlias(ref_alias_edt->text());

		/* The reference must have an SQL application (be between SELECT-FROM, FROM-WHERE or after WHERE),
			 if the user do not check some of these attributes raises an error */
		if(!select_from_chk->isChecked() &&	!from_where_chk->isChecked() &&
			 !after_where_chk->isChecked() && !end_expr_chk->isChecked() &&
			 !view_def_chk->isChecked())
			throw Exception(InvSQLScopeViewReference,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		ref_flags = 0;

		if(view_def_chk->isChecked())
			ref_flags = Reference::SqlViewDefinition;

		if(select_from_chk->isChecked())
			ref_flags |= Reference::SqlReferSelect;

		if(from_where_chk->isChecked())
			ref_flags |= Reference::SqlReferFrom;

		if(after_where_chk->isChecked())
			ref_flags |= Reference::SqlReferWhere;

		if(end_expr_chk->isChecked())
			ref_flags |= Reference::SqlReferEndExpr;

		emit s_closeRequested();
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void ReferenceWidget::selectReferenceType(void)
{
	//Marks if the select reference type treats a reference to an object
	bool ref_obj=(ref_type_cmb->currentIndex()==static_cast<int>(Reference::ReferColumn));
	ref_object_sel->setEnabled(ref_obj);
	tab_alias_edt->setEnabled(ref_obj);
	col_alias_edt->setEnabled(ref_obj);
	view_def_chk->setChecked(false);
	expression_txt->setEnabled(!ref_obj);
	expr_alias_edt->setEnabled(!ref_obj);
	view_def_chk->setVisible(!ref_obj);
}
