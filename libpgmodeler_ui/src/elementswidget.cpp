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

#include "elementswidget.h"

ElementsWidget::ElementsWidget(QWidget *parent) : QWidget(parent)
{
	try
	{
		map<QString, vector<QWidget *> > fields_map;
		QFrame *frame=nullptr;

		setupUi(this);

		parent_obj=nullptr;

		elem_expr_hl=new SyntaxHighlighter(elem_expr_txt, false);
		elem_expr_hl->loadConfiguration(GlobalAttributes::CONFIGURATIONS_DIR +
																				 GlobalAttributes::DIR_SEPARATOR +
																				 GlobalAttributes::SQL_HIGHLIGHT_CONF +
																				 GlobalAttributes::CONFIGURATION_EXT);

		elements_tab=new ObjectTableWidget(ObjectTableWidget::ALL_BUTTONS, true, this);
		op_class_sel=new ObjectSelectorWidget(OBJ_OPCLASS, true, this);
		collation_sel=new ObjectSelectorWidget(OBJ_COLLATION, true, this);
		operator_sel=new ObjectSelectorWidget(OBJ_OPERATOR, true, this);

		elements_tab->setColumnCount(6);
		elements_tab->setHeaderLabel(trUtf8("Element"), 0);
		elements_tab->setHeaderIcon(QPixmap(":/icones/icones/column.png"),0);
		elements_tab->setHeaderLabel(trUtf8("Type"), 1);
		elements_tab->setHeaderIcon(QPixmap(":/icones/icones/usertype.png"),1);
		elements_tab->setHeaderLabel(trUtf8("Operator Class"), 3);
		elements_tab->setHeaderIcon(QPixmap(":/icones/icones/opclass.png"),3);
		elements_tab->setHeaderLabel(trUtf8("Sorting"), 4);
		elements_tab->setHeaderLabel(trUtf8("Nulls First"), 5);

		element_grid->addWidget(collation_sel, 2,1,1,2);
		element_grid->addWidget(op_class_sel, 3,1,1,2);
		element_grid->addWidget(operator_sel, 4,1,1,2);
		element_grid->addWidget(elements_tab, 6,0,1,3);

		fields_map[BaseObjectWidget::generateVersionsInterval(BaseObjectWidget::AFTER_VERSION, SchemaParser::PGSQL_VERSION_91)].push_back(collation_lbl);
		frame=BaseObjectWidget::generateVersionWarningFrame(fields_map);
		element_grid->addWidget(frame, element_grid->count()+1, 0, 1, 3);
		frame->setParent(this);

		connect(elements_tab, SIGNAL(s_rowAdded(int)), this, SLOT(handleElement(int)));
		connect(elements_tab, SIGNAL(s_rowUpdated(int)), this, SLOT(handleElement(int)));
		connect(elements_tab, SIGNAL(s_rowEdited(int)), this, SLOT(editElement(int)));
		connect(column_rb, SIGNAL(toggled(bool)), this, SLOT(selectElementObject(void)));
		connect(expression_rb, SIGNAL(toggled(bool)), this, SLOT(selectElementObject(void)));
		connect(sorting_chk, SIGNAL(toggled(bool)), ascending_rb, SLOT(setEnabled(bool)));
		connect(sorting_chk, SIGNAL(toggled(bool)), descending_rb, SLOT(setEnabled(bool)));
		connect(sorting_chk, SIGNAL(toggled(bool)), nulls_first_chk, SLOT(setEnabled(bool)));

		this->setEnabled(false);
		collation_sel->setVisible(false);
		collation_lbl->setVisible(false);
		operator_sel->setVisible(false);
		operator_lbl->setVisible(false);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void ElementsWidget::setAttributes(DatabaseModel *model, BaseObject *parent_obj)
{
	if(!model || !parent_obj)
	{
		this->setEnabled(false);
		throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	}
	else if(parent_obj->getObjectType()!=OBJ_TABLE &&
					parent_obj->getObjectType()!=OBJ_RELATIONSHIP)
		throw Exception(ERR_OPR_OBJ_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	this->setEnabled(true);
	this->parent_obj=parent_obj;

	op_class_sel->setModel(model);
	collation_sel->setModel(model);
	operator_sel->setModel(model);

	updateColumnsCombo();
}

void ElementsWidget::setAttributes(DatabaseModel *model, Table *table, vector<IndexElement> &elems)
{
	setAttributes(model, table);
	collation_sel->setVisible(true);
	collation_lbl->setVisible(true);

	elements_tab->setHeaderLabel(trUtf8("Collation"), 2);
	elements_tab->setHeaderIcon(QPixmap(":/icones/icones/collation.png"),2);
	elements_tab->blockSignals(true);

	for(unsigned i=0; i < elems.size(); i++)
	{
		elements_tab->addRow();
		showElementData(elems[i], i);
	}

	elements_tab->blockSignals(false);
}

void ElementsWidget::setAttributes(DatabaseModel *model, BaseObject *parent_obj, vector<ExcludeElement> &elems)
{
	setAttributes(model, parent_obj);
	operator_sel->setVisible(true);
	operator_lbl->setVisible(true);

	elements_tab->setHeaderLabel(trUtf8("Operator"), 2);
	elements_tab->setHeaderIcon(QPixmap(":/icones/icones/operator.png"),2);
	elements_tab->blockSignals(true);

	for(unsigned i=0; i < elems.size(); i++)
	{
		elements_tab->addRow();
		showElementData(elems[i], i);
	}

	elements_tab->blockSignals(false);
}

void ElementsWidget::clear(void)
{
	column_cmb->clear();
	sorting_chk->setEnabled(true);

	elements_tab->blockSignals(true);
	elements_tab->removeRows();
	elements_tab->blockSignals(false);

	op_class_sel->clearSelector();
	operator_sel->clearSelector();
	elem_expr_txt->clear();
	ascending_rb->setChecked(true);
	column_rb->setChecked(true);

	collation_sel->setVisible(false);
	collation_lbl->setVisible(false);
	operator_sel->setVisible(false);
	operator_lbl->setVisible(false);
}

void ElementsWidget::updateColumnsCombo(void)
{
	Table *table = dynamic_cast<Table *>(parent_obj);
	Relationship *rel = dynamic_cast<Relationship *>(parent_obj);
	Column *column=nullptr;
	unsigned i, col_count=0;

	try
	{
		column_cmb->clear();

		if(table)
		{
			col_count=table->getColumnCount();
			for(i=0; i < col_count; i++)
			{
				column=table->getColumn(i);
				column_cmb->addItem(Utf8String::create(column->getName()),
														QVariant::fromValue<void *>(column));
			}
		}
		else
		{
			col_count=rel->getAttributeCount();
			for(i=0; i < col_count; i++)
			{
				column=rel->getAttribute(i);
				column_cmb->addItem(Utf8String::create(column->getName()),
														QVariant::fromValue<void *>(column));
			}
		}
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void ElementsWidget::showElementData(IndexElement elem, int elem_idx)
{
	showElementData(&elem, elem_idx);
}

void ElementsWidget::showElementData(ExcludeElement elem, int elem_idx)
{
	showElementData(&elem, elem_idx);
}

void ElementsWidget::showElementData(Element *elem, int elem_idx)
{
	IndexElement *idxelem = dynamic_cast<IndexElement *>(elem);
	ExcludeElement *excelem = dynamic_cast<ExcludeElement *>(elem);

	if(elem->getColumn())
	{
		elements_tab->setCellText(Utf8String::create(elem->getColumn()->getName()), elem_idx, 0);
		elements_tab->setCellText(Utf8String::create(elem->getColumn()->getTypeName()), elem_idx, 1);
	}
	else
	{
		elements_tab->setCellText(Utf8String::create(elem->getExpression()), elem_idx, 0);
		elements_tab->setCellText(trUtf8("Expression"), elem_idx, 1);
	}

	elements_tab->clearCellText(elem_idx, 2);
	if(idxelem && idxelem->getCollation())
		elements_tab->setCellText(Utf8String::create(idxelem->getCollation()->getName(true)), elem_idx, 2);
	else if(excelem && excelem->getOperator())
		elements_tab->setCellText(Utf8String::create(excelem->getOperator()->getSignature(true)), elem_idx, 2);

	elements_tab->clearCellText(elem_idx, 3);
	if(elem->getOperatorClass())
		elements_tab->setCellText(Utf8String::create(elem->getOperatorClass()->getName(true)), elem_idx, 3);

	if(elem->isSortingEnabled())
	{
		if(elem->getSortingAttribute(IndexElement::ASC_ORDER))
			elements_tab->setCellText(ascending_rb->text(), elem_idx, 4);
		else
			elements_tab->setCellText(descending_rb->text(), elem_idx, 4);

		if(elem->getSortingAttribute(IndexElement::nullptrS_FIRST))
			elements_tab->setCellText(trUtf8("Yes"), elem_idx, 5);
		else
			elements_tab->setCellText(trUtf8("No"), elem_idx, 5);
	}
	else
	{
		elements_tab->clearCellText(elem_idx, 4);
		elements_tab->clearCellText(elem_idx, 5);
	}

	if(dynamic_cast<IndexElement *>(elem))
		elements_tab->setRowData(QVariant::fromValue<IndexElement>(*dynamic_cast<IndexElement *>(elem)), elem_idx);
	else
		elements_tab->setRowData(QVariant::fromValue<ExcludeElement>(*dynamic_cast<ExcludeElement *>(elem)), elem_idx);
}

void ElementsWidget::handleElement(int elem_idx)
{
	if(column_rb->isChecked() ||
		 (expression_rb->isChecked() && !elem_expr_txt->toPlainText().isEmpty()))
	{
		IndexElement idxelem;
		ExcludeElement excelem;

		/* Selects the correct element based upon the visibility of operator
			 selector (available only for ExcludeElement) */
		Element *elem = (operator_sel->isVisible() ?
											 dynamic_cast<Element *>(&excelem) :
											 dynamic_cast<Element *>(&idxelem));

		elem->setSortingEnabled(sorting_chk->isChecked());
		elem->setSortingAttribute(IndexElement::nullptrS_FIRST, nulls_first_chk->isChecked());
		elem->setSortingAttribute(IndexElement::ASC_ORDER, ascending_rb->isChecked());
		elem->setOperatorClass(dynamic_cast<OperatorClass *>(op_class_sel->getSelectedObject()));

		if(collation_sel->isVisible())
			idxelem.setCollation(dynamic_cast<Collation *>(collation_sel->getSelectedObject()));

		if(operator_sel->isVisible())
			excelem.setOperator(dynamic_cast<Operator *>(operator_sel->getSelectedObject()));

		if(expression_rb->isChecked())
			elem->setExpression(elem_expr_txt->toPlainText().toUtf8());
		else
			elem->setColumn(reinterpret_cast<Column *>(column_cmb->itemData(column_cmb->currentIndex()).value<void *>()));

		showElementData(elem, elem_idx);

		elem_expr_txt->clear();
		ascending_rb->setChecked(true);
		sorting_chk->setChecked(true);
		op_class_sel->clearSelector();
		operator_sel->clearSelector();
		collation_sel->clearSelector();
		nulls_first_chk->setChecked(false);
	}
	else if(elements_tab->getCellText(elem_idx,0).isEmpty())
		elements_tab->removeRow(elem_idx);
}

void ElementsWidget::editElement(int elem_idx)
{
	Element *elem = nullptr;
	IndexElement idxelem;
	ExcludeElement excelem;
	QVariant data = elements_tab->getRowData(elem_idx);

	//If the data is an IndexElement
	if(data.canConvert<IndexElement>())
	{
		idxelem = data.value<IndexElement>();
		elem = &idxelem;
	}
	else
	{
		excelem = data.value<ExcludeElement>();
		elem = &excelem;
	}

	if(elem->getColumn())
	{
		column_rb->setChecked(true);
		column_cmb->setCurrentIndex(column_cmb->findText(Utf8String::create(elem->getColumn()->getName())));
	}
	else
	{
		expression_rb->setChecked(true);
		elem_expr_txt->setPlainText(Utf8String::create(elem->getExpression()));
	}

	if(elem->getSortingAttribute(IndexElement::ASC_ORDER))
		ascending_rb->setChecked(true);
	else
		descending_rb->setChecked(true);

	nulls_first_chk->setChecked(elem->getSortingAttribute(IndexElement::nullptrS_FIRST));
	sorting_chk->setChecked(elem->isSortingEnabled());
	op_class_sel->setSelectedObject(elem->getOperatorClass());

	//Specific for IndexElement
	if(collation_sel->isVisible())
		collation_sel->setSelectedObject(idxelem.getCollation());

	//Specific for ExcludeElement
	if(operator_sel->isVisible())
		operator_sel->setSelectedObject(excelem.getOperator());
}

void ElementsWidget::selectElementObject(void)
{
	QObject *obj_sender=sender();

	column_rb->blockSignals(true);
	expression_rb->blockSignals(true);

	if(obj_sender==column_rb)
	{
		elem_expr_txt->clear();
		column_cmb->setEnabled(true);
		expression_rb->setChecked(false);
		column_rb->setChecked(true);
		elem_expr_txt->setEnabled(false);
	}
	else
	{
		column_cmb->setEnabled(false);
		column_rb->setChecked(false);
		expression_rb->setChecked(true);
		elem_expr_txt->setEnabled(true);
	}

	column_rb->blockSignals(false);
	expression_rb->blockSignals(false);
}

void ElementsWidget::getElements(vector<IndexElement> &elems)
{
	if(elements_tab->getRowCount() > 0)
	{
		//Confirming if the data on elements table is IndexElement
		if(elements_tab->getRowData(0).canConvert<IndexElement>())
		{
			elems.clear();

			for(unsigned i=0; i < elements_tab->getRowCount(); i++)
				elems.push_back(elements_tab->getRowData(i).value<IndexElement>());
		}
	}
}

void ElementsWidget::getElements(vector<ExcludeElement> &elems)
{
	if(elements_tab->getRowCount() > 0)
	{
		//Confirming if the data on elements table is ExcludeElement
		if(elements_tab->getRowData(0).canConvert<ExcludeElement>())
		{
			elems.clear();

			for(unsigned i=0; i < elements_tab->getRowCount(); i++)
				elems.push_back(elements_tab->getRowData(i).value<ExcludeElement>());
		}
	}
}

