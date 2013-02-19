#include "indexwidget.h"

IndexWidget::IndexWidget(QWidget *parent): BaseObjectWidget(parent, OBJ_INDEX)
{
	try
	{
		map<QString, vector<QWidget *> > field_map;
		map<QWidget *, vector<QString> > value_map;
		QFrame *frame=NULL;
		QStringList list;
		QGridLayout *grid=NULL;

		Ui_IndexWidget::setupUi(this);

		//Cria um destacador de sintaxe no campo de expressão de checagem
		cond_expr_hl=new SyntaxHighlighter(cond_expr_txt, false);
		cond_expr_hl->loadConfiguration(GlobalAttributes::CONFIGURATIONS_DIR +
																						GlobalAttributes::DIR_SEPARATOR +
																						GlobalAttributes::SQL_HIGHLIGHT_CONF +
																						GlobalAttributes::CONFIGURATION_EXT);

		elem_expr_hl=new SyntaxHighlighter(elem_expr_txt, false);
		elem_expr_hl->loadConfiguration(GlobalAttributes::CONFIGURATIONS_DIR +
																				 GlobalAttributes::DIR_SEPARATOR +
																				 GlobalAttributes::SQL_HIGHLIGHT_CONF +
																				 GlobalAttributes::CONFIGURATION_EXT);

		elements_tab=new ObjectTableWidget(ObjectTableWidget::ALL_BUTTONS, true, this);
		op_class_sel=new ObjectSelectorWidget(OBJ_OPCLASS, true, this);

		elements_tab->setColumnCount(5);
		elements_tab->setHeaderLabel(trUtf8("Element"), 0);
		elements_tab->setHeaderIcon(QPixmap(":/icones/icones/column.png"),0);
		elements_tab->setHeaderLabel(trUtf8("Type"), 1);
		elements_tab->setHeaderIcon(QPixmap(":/icones/icones/usertype.png"),1);
		elements_tab->setHeaderLabel(trUtf8("Operator Class"), 2);
		elements_tab->setHeaderIcon(QPixmap(":/icones/icones/opclass.png"),2);
		elements_tab->setHeaderLabel(trUtf8("Sorting"), 3);
		elements_tab->setHeaderLabel(trUtf8("Nulls First"), 4);

		grid=dynamic_cast<QGridLayout *>(elements_grp->layout());
		grid->addWidget(op_class_sel, 2,1,1,3);
		grid->addWidget(elements_tab, 5,0,1,4);

		configureFormLayout(index_grid, OBJ_INDEX);
		parent_form->setMinimumSize(600, 640);

		IndexingType::getTypes(list);
		indexing_cmb->addItems(list);

		field_map[generateVersionsInterval(UNTIL_VERSION, SchemaParser::PGSQL_VERSION_81)].push_back(indexing_lbl);
		field_map[generateVersionsInterval(AFTER_VERSION, SchemaParser::PGSQL_VERSION_82)].push_back(concurrent_chk);
		field_map[generateVersionsInterval(AFTER_VERSION, SchemaParser::PGSQL_VERSION_83)].push_back(sorting_lbl);
		field_map[generateVersionsInterval(AFTER_VERSION, SchemaParser::PGSQL_VERSION_84)].push_back(fast_update_chk);
		value_map[indexing_lbl].push_back(~IndexingType(IndexingType::rtree));

		frame=generateVersionWarningFrame(field_map, &value_map);
		index_grid->addWidget(frame, index_grid->count()+1, 0, 1, 0);
		frame->setParent(this);

		connect(parent_form->aplicar_ok_btn,SIGNAL(clicked(bool)), this, SLOT(applyConfiguration(void)));
		connect(elements_tab, SIGNAL(s_rowAdded(int)), this, SLOT(handleElement(int)));
		connect(elements_tab, SIGNAL(s_rowUpdated(int)), this, SLOT(handleElement(int)));
		connect(elements_tab, SIGNAL(s_rowEdited(int)), this, SLOT(editElement(int)));
		connect(column_rb, SIGNAL(toggled(bool)), this, SLOT(selectElementObject(void)));
		connect(expression_rb, SIGNAL(toggled(bool)), this, SLOT(selectElementObject(void)));
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void IndexWidget::hideEvent(QHideEvent *event)
{
	BaseObjectWidget::hideEvent(event);

	cond_expr_txt->clear();
	column_cmb->clear();
	concurrent_chk->setChecked(false);
	unique_chk->setChecked(false);
	indexing_cmb->setCurrentIndex(0);
	fill_factor_sb->setValue(fill_factor_sb->minimum());

	elements_tab->blockSignals(true);
	elements_tab->removeRows();
	elements_tab->blockSignals(false);

	op_class_sel->clearSelector();
	elem_expr_txt->clear();
	ascending_rb->setChecked(true);
	column_rb->setChecked(true);
}

void IndexWidget::updateColumnsCombo(void)
{
	Column *column=NULL;
	unsigned i, col_count=0;

	try
	{
		column_cmb->clear();

		col_count=table->getColumnCount();
		for(i=0; i < col_count; i++)
		{
			column=table->getColumn(i);
			column_cmb->addItem(QString::fromUtf8(column->getName()),
													QVariant::fromValue<void *>(column));
		}
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void IndexWidget::showElementData(IndexElement elem, int elem_idx)
{
	if(elem.getColumn())
	{
		elements_tab->setCellText(QString::fromUtf8(elem.getColumn()->getName()), elem_idx, 0);
		elements_tab->setCellText(QString::fromUtf8(elem.getColumn()->getTypeName()), elem_idx, 1);
	}
	else
	{
		elements_tab->setCellText(QString::fromUtf8(elem.getExpression()), elem_idx, 0);
		elements_tab->setCellText(trUtf8("Expressão"), elem_idx, 1);
	}

	if(elem.getOperatorClass())
		elements_tab->setCellText(QString::fromUtf8(elem.getOperatorClass()->getName(true)), elem_idx, 2);
	else
		elements_tab->setCellText("-", elem_idx, 2);

	if(elem.getSortAttribute(IndexElement::ASC_ORDER))
		elements_tab->setCellText(ascending_rb->text(), elem_idx, 3);
	else
		elements_tab->setCellText(descending_rb->text(), elem_idx, 3);

	if(elem.getSortAttribute(IndexElement::NULLS_FIRST))
		elements_tab->setCellText(trUtf8("Sim"), elem_idx, 4);
	else
		elements_tab->setCellText(trUtf8("Não"), elem_idx, 4);

	elements_tab->setRowData(QVariant::fromValue<IndexElement>(elem), elem_idx);
}

void IndexWidget::handleElement(int elem_idx)
{
	if(column_rb->isChecked() ||
		 (expression_rb->isChecked() && !elem_expr_txt->toPlainText().isEmpty()))
	{
		IndexElement elem;

		elem.setSortAttribute(IndexElement::NULLS_FIRST, nulls_first_chk->isChecked());
		elem.setSortAttribute(IndexElement::ASC_ORDER, ascending_rb->isChecked());
		elem.setOperatorClass(dynamic_cast<OperatorClass *>(op_class_sel->getSelectedObject()));

		if(expression_rb->isChecked())
			elem.setExpression(elem_expr_txt->toPlainText());
		else
			elem.setColumn(reinterpret_cast<Column *>(column_cmb->itemData(column_cmb->currentIndex()).value<void *>()));

		showElementData(elem, elem_idx);

		elem_expr_txt->clear();
		ascending_rb->setChecked(true);
		op_class_sel->clearSelector();
		nulls_first_chk->setChecked(false);
	}
	else if(elements_tab->getCellText(elem_idx,0).isEmpty())
		elements_tab->removeRow(elem_idx);
}

void IndexWidget::editElement(int elem_idx)
{
	IndexElement elem;

	elem=elements_tab->getRowData(elem_idx).value<IndexElement>();

	if(elem.getColumn())
	{
		column_rb->setChecked(true);
		column_cmb->setCurrentIndex(column_cmb->findText(QString::fromUtf8(elem.getColumn()->getName())));
	}
	else
	{
		expression_rb->setChecked(true);
		elem_expr_txt->setPlainText(elem.getExpression());
	}

	if(elem.getSortAttribute(IndexElement::ASC_ORDER))
		ascending_rb->setChecked(true);
	else
		descending_rb->setChecked(true);
	nulls_first_chk->setChecked(elem.getSortAttribute(IndexElement::NULLS_FIRST));

	op_class_sel->setSelectedObject(elem.getOperatorClass());
}

void IndexWidget::selectElementObject(void)
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

void IndexWidget::setAttributes(DatabaseModel *model, Table *parent_obj, OperationList *op_list, Index *index)
{
	unsigned i, count;

	if(!parent_obj)
		throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	BaseObjectWidget::setAttributes(model, op_list, index, parent_obj);

	op_class_sel->setModel(model);
	updateColumnsCombo();

	if(index)
	{
		indexing_cmb->setCurrentIndex(indexing_cmb->findText(~index->getIndexingType()));
		fill_factor_sb->setValue(index->getFillFactor());
		concurrent_chk->setChecked(index->getIndexAttribute(Index::CONCURRENT));
		fast_update_chk->setChecked(index->getIndexAttribute(Index::FAST_UPDATE));
		unique_chk->setChecked(index->getIndexAttribute(Index::UNIQUE));
		cond_expr_txt->setPlainText(index->getConditionalExpression());

		elements_tab->blockSignals(true);
		count=index->getElementCount();
		for(i=0; i < count; i++)
		{
			elements_tab->addRow();
			showElementData(index->getElement(i), i);
		}
		elements_tab->blockSignals(false);
	}
}

void IndexWidget::applyConfiguration(void)
{
	try
	{
		Index *index=NULL;
		unsigned i, count;
		IndexElement elem;

		startConfiguration<Index>();

		index=dynamic_cast<Index *>(this->object);
		index->setIndexAttribute(Index::FAST_UPDATE, fast_update_chk->isChecked());
		index->setIndexAttribute(Index::CONCURRENT, concurrent_chk->isChecked());
		index->setIndexAttribute(Index::UNIQUE, unique_chk->isChecked());
		index->setConditionalExpression(cond_expr_txt->toPlainText());
		index->setIndexingType(IndexingType(indexing_cmb->currentText()));
		index->setFillFactor(fill_factor_sb->value());

		index->removeElements();
		count=elements_tab->getRowCount();

		for(i=0; i < count; i++)
		{
			elem=elements_tab->getRowData(i).value<IndexElement>();

			if(elem.getColumn())
				index->addElement(elem.getColumn(), elem.getOperatorClass(),
													 elem.getSortAttribute(IndexElement::ASC_ORDER),
													 elem.getSortAttribute(IndexElement::NULLS_FIRST));
			else
				index->addElement(elem.getExpression(), elem.getOperatorClass(),
													 elem.getSortAttribute(IndexElement::ASC_ORDER),
													 elem.getSortAttribute(IndexElement::NULLS_FIRST));
		}

		BaseObjectWidget::applyConfiguration();
		finishConfiguration();
	}
	catch(Exception &e)
	{
		cancelConfiguration();
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

