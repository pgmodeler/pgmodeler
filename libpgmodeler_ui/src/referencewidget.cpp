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

#include "referencewidget.h"
#include "baseobjectwidget.h"

ReferenceWidget::ReferenceWidget(QWidget *parent) : QWidget(parent)
{
	setupUi(this);

	model = nullptr;
	ref_flags = 0;

	expression_txt=new NumberedTextEditor(this, true);
	expression_hl=new SyntaxHighlighter(expression_txt, false, true);
	expression_hl->loadConfiguration(GlobalAttributes::getSQLHighlightConfPath());

	ref_object_sel=new ObjectSelectorWidget({ ObjectType::Table, ObjectType::ForeignTable, ObjectType::Column }, true, this);
	ref_object_sel->enableObjectCreation(false);
	ref_object_sel->setToolTip(tr("To reference all columns of a table select only a table in the object selector, this is the same as write <em><strong>[schema].[table].*</strong></em>. In order to reference a only a single column of a table select a column object in the selector."));
	expression_cp=new CodeCompletionWidget(expression_txt, true);

	QGridLayout *grid = dynamic_cast<QGridLayout *>(properties_tbw->widget(0)->layout());
	grid->addWidget(ref_object_sel, 2, 1, 1, 3);
	grid->addWidget(expression_txt, 4, 1, 1, 4);

	properties_tbw->setTabEnabled(1, false);
	properties_tbw->setTabEnabled(2, false);

	pgsqltype_wgt = new PgSQLTypeWidget(this);

	columns_tab = new ObjectsTableWidget(ObjectsTableWidget::AllButtons, true, this);
	columns_tab->setColumnCount(3);
	columns_tab->setHeaderLabel(tr("Name"), 0);
	columns_tab->setHeaderIcon(QPixmap(PgModelerUiNs::getIconPath("uid")),0);
	columns_tab->setHeaderLabel(tr("Type"), 1);
	columns_tab->setHeaderIcon(QPixmap(PgModelerUiNs::getIconPath("usertype")),1);
	columns_tab->setHeaderLabel(tr("Alias"), 2);

	QFrame *info_frm=BaseObjectWidget::generateInformationFrame(tr("This tab can be used to inform the columns that the view owns. This is just a convenience to make the visualization of this kind of object more intuitive. If no column is specified here the columns of the view displayed in the canvas will be a fragment of the expression defined in the previous tab."));
	QVBoxLayout *vbox =  dynamic_cast<QVBoxLayout *>(properties_tbw->widget(1)->layout());

	vbox->addWidget(pgsqltype_wgt);
	vbox->addWidget(columns_tab);

	info_frm->setParent(this);
	vbox->addWidget(info_frm);

	ref_tables_tab = new ObjectsTableWidget(ObjectsTableWidget::AllButtons ^ (ObjectsTableWidget::UpdateButton |
																																				ObjectsTableWidget::DuplicateButton|
																																				ObjectsTableWidget::EditButton), true, this);
	ref_tables_tab->setColumnCount(3);
	ref_tables_tab->setHeaderLabel(tr("Name"), 0);
	ref_tables_tab->setHeaderIcon(QPixmap(PgModelerUiNs::getIconPath("uid")),0);
	ref_tables_tab->setHeaderLabel(tr("Schema"), 1);
	ref_tables_tab->setHeaderIcon(QPixmap(PgModelerUiNs::getIconPath("schema")),1);

	ref_table_sel=new ObjectSelectorWidget({ ObjectType::Table, ObjectType::ForeignTable }, true, this);
	ref_table_sel->enableObjectCreation(false);

	QHBoxLayout *hbox = new QHBoxLayout;
	hbox->addWidget(ref_table_lbl);
	hbox->addWidget(ref_table_sel);

	info_frm=BaseObjectWidget::generateInformationFrame(tr("This tab can be used to inform the tables that the view references. This is just a convenience to make the visualization of this kind of object more intuitive. If no table is specified here no relationship will be displayed in the canvas. Note that no validation will be done to check if the provided tables are really referenced by the view."));
	vbox = new QVBoxLayout;
	vbox->setContentsMargins(4,4,4,4);
	vbox->addLayout(hbox);
	vbox->addWidget(ref_tables_tab);
	vbox->addWidget(info_frm);
	properties_tbw->widget(2)->setLayout(vbox);

	selectReferenceType();
	setMinimumSize(640, 480);

	connect(columns_tab, SIGNAL(s_rowAdded(int)), this, SLOT(addColumn(int)));
	connect(columns_tab, SIGNAL(s_rowUpdated(int)), this, SLOT(updateColumn(int)));
	connect(columns_tab, SIGNAL(s_rowEdited(int)), this, SLOT(editColumn(int)));
	connect(columns_tab, SIGNAL(s_rowDuplicated(int,int)), this, SLOT(duplicateColumn(int,int)));

	connect(ref_tables_tab, SIGNAL(s_rowAdded(int)), this, SLOT(addRefTable(int)));

	connect(view_def_chk, SIGNAL(toggled(bool)), select_from_chk, SLOT(setDisabled(bool)));
	connect(view_def_chk, SIGNAL(toggled(bool)), from_where_chk, SLOT(setDisabled(bool)));
	connect(view_def_chk, SIGNAL(toggled(bool)), after_where_chk, SLOT(setDisabled(bool)));
	connect(view_def_chk, SIGNAL(toggled(bool)), end_expr_chk, SLOT(setDisabled(bool)));

	connect(view_def_chk, &QCheckBox::toggled, [&](bool checked){
		properties_tbw->setTabEnabled(1, checked);
		properties_tbw->setTabEnabled(2, checked);
	});

	connect(ref_type_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(selectReferenceType()));

	connect(ref_object_sel, &ObjectSelectorWidget::s_objectSelected, [&](){
		col_alias_edt->setEnabled(dynamic_cast<Column *>(ref_object_sel->getSelectedObject()));
	});

	connect(ref_object_sel, &ObjectSelectorWidget::s_selectorCleared, [&](){
		col_alias_edt->setEnabled(false);
	});

	connect(ref_table_sel, &ObjectSelectorWidget::s_selectorChanged, [&](bool selected){
		ref_tables_tab->setButtonsEnabled(ObjectsTableWidget::AddButton, selected);
	});

}

void ReferenceWidget::setAttributes(Reference ref, unsigned ref_flags, DatabaseModel *model)
{
	this->ref_flags = ref_flags;
	this->reference = ref;
	this->model = model;

	pgsqltype_wgt->setAttributes(PgSqlType(), model,
															 UserTypeConfig::AllUserTypes ^ UserTypeConfig::SequenceType, true, false);

	expression_cp->configureCompletion(model, expression_hl);
	ref_object_sel->setModel(model);
	ref_table_sel->setModel(model);

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
	{
		int row = 0;
		view_def_chk->setChecked(true);
		columns_tab->blockSignals(true);

		for(auto &col : ref.getColumns())
		{
			columns_tab->addRow();
			columns_tab->setCellText(col.name, row, 0);
			columns_tab->setCellText(col.type, row, 1);
			columns_tab->setCellText(col.alias, row, 2);
			columns_tab->setRowData(QVariant::fromValue<PgSqlType>(PgSqlType::parseString(col.type)), row);
			row++;
		}

		columns_tab->blockSignals(false);

		ref_tables_tab->blockSignals(true);
		row = 0;
		for(auto &tab : ref.getReferencedTables())
		{
			ref_tables_tab->addRow();
			ref_tables_tab->setCellText(tab->getName(), row, 0);
			ref_tables_tab->setCellText(tab->getSchema()->getName(), row, 1);
			ref_tables_tab->setRowData(QVariant::fromValue<void *>(reinterpret_cast<void *>(tab)), row);
			row++;
		}

		ref_tables_tab->blockSignals(false);
		columns_tab->clearSelection();
		ref_tables_tab->clearSelection();
	}
	else
	{
		select_from_chk->setChecked((ref_flags & Reference::SqlReferSelect) == Reference::SqlReferSelect);
		from_where_chk->setChecked((ref_flags & Reference::SqlReferFrom) == Reference::SqlReferFrom);
		after_where_chk->setChecked((ref_flags & Reference::SqlReferWhere) == Reference::SqlReferWhere);
		end_expr_chk->setChecked((ref_flags & Reference::SqlReferEndExpr) == Reference::SqlReferEndExpr);
	}

	ref_tables_tab->setButtonsEnabled(ObjectsTableWidget::AddButton, false);
}

Reference ReferenceWidget::getReference()
{
	return reference;
}

unsigned ReferenceWidget::getReferenceFlags()
{
	return ref_flags;
}

void ReferenceWidget::applyConfiguration()
{
	try
	{
		//Creating a reference to a column
		if(static_cast<unsigned>(ref_type_cmb->currentIndex())==Reference::ReferColumn)
		{
			Column *column = dynamic_cast<Column *>(ref_object_sel->getSelectedObject());
			PhysicalTable *table = (column ? dynamic_cast<PhysicalTable *>(column->getParentTable()) :
																			 dynamic_cast<PhysicalTable *>(ref_object_sel->getSelectedObject()));
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
			throw Exception(ErrorCode::InvSQLScopeViewReference,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		ref_flags = 0;

		if(view_def_chk->isChecked())
		{
			ref_flags = Reference::SqlViewDefinition;
			reference.removeColumns();

			for(unsigned row = 0; row < columns_tab->getRowCount(); row++)
				reference.addColumn(columns_tab->getCellText(row, 0),
														columns_tab->getRowData(row).value<PgSqlType>(),
														columns_tab->getCellText(row, 2));

			for(unsigned row = 0; row < ref_tables_tab->getRowCount(); row++)
				reference.addReferencedTable(reinterpret_cast<PhysicalTable *>(ref_tables_tab->getRowData(row).value<void *>()));
		}

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
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void ReferenceWidget::handleColumn(int row)
{
	PgSqlType type = pgsqltype_wgt->getPgSQLType();
	columns_tab->setCellText(name_edt->text(), row, 0);
	columns_tab->setCellText(*type, row, 1);
	columns_tab->setCellText(alias_edt->text(), row, 2);
	columns_tab->setRowData(QVariant::fromValue<PgSqlType>(type), row);
	name_edt->clear();
	alias_edt->clear();
	name_edt->setFocus();
}

void ReferenceWidget::addColumn(int row)
{
	if(!name_edt->text().isEmpty())
		handleColumn(row);
	else
		columns_tab->removeRow(row);
}

void ReferenceWidget::addRefTable(int row)
{
	PhysicalTable *table = dynamic_cast<PhysicalTable *>(ref_table_sel->getSelectedObject());
	ref_tables_tab->setRowData(QVariant::fromValue<void *>(reinterpret_cast<void *>(table)), row);
	ref_tables_tab->setCellText(table->getName(), row, 0);
	ref_tables_tab->setCellText(table->getSchema()->getName(), row, 1);
	ref_table_sel->clearSelector();
}

void ReferenceWidget::updateColumn(int row)
{
	if(!name_edt->text().isEmpty())
		handleColumn(row);
}

void ReferenceWidget::editColumn(int row)
{
	name_edt->setText(columns_tab->getCellText(row, 0));
	alias_edt->setText(columns_tab->getCellText(row, 2));
	pgsqltype_wgt->setAttributes(columns_tab->getRowData(row).value<PgSqlType>(), model,
															 UserTypeConfig::AllUserTypes ^ UserTypeConfig::SequenceType, true, false);
}

void ReferenceWidget::duplicateColumn(int src_row, int new_row)
{
	columns_tab->setRowData(columns_tab->getRowData(src_row), new_row);
}

void ReferenceWidget::selectReferenceType()
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
	properties_tbw->setTabEnabled(1, !ref_obj && view_def_chk->isChecked());
}
