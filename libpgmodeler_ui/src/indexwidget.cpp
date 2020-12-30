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

#include "indexwidget.h"

IndexWidget::IndexWidget(QWidget *parent): BaseObjectWidget(parent, ObjectType::Index)
{
	try
	{
		QGridLayout *grid=nullptr;
		map<QString, vector<QWidget *> > fields_map;
		map<QWidget *, vector<QString> > values_map;
		QFrame *frame=nullptr;

		Ui_IndexWidget::setupUi(this);

		predicate_hl=new SyntaxHighlighter(predicate_txt, false, true);
		predicate_hl->loadConfiguration(GlobalAttributes::getSQLHighlightConfPath());

		elements_tab = new ElementsTableWidget(this);

		grid=new QGridLayout;
		grid->setContentsMargins(4,4,4,4);
		grid->addWidget(elements_tab,0,0);
		tabWidget->widget(1)->setLayout(grid);

		configureFormLayout(index_grid, ObjectType::Index);
		indexing_cmb->addItems(IndexingType::getTypes());

		fields_map[BaseObjectWidget::generateVersionsInterval(BaseObjectWidget::AfterVersion, PgSqlVersions::PgSqlVersion92)].push_back(buffering_chk);
		fields_map[BaseObjectWidget::generateVersionsInterval(BaseObjectWidget::AfterVersion, PgSqlVersions::PgSqlVersion95)].push_back(indexing_lbl);
		values_map[indexing_lbl].push_back(~IndexingType(IndexingType::Brin));

		frame=BaseObjectWidget::generateVersionWarningFrame(fields_map, &values_map);
		frame->setParent(this);
		grid=dynamic_cast<QGridLayout *>(tabWidget->widget(0)->layout());
		grid->addWidget(frame, grid->count(), 0, 1, 5);

		connect(indexing_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(selectIndexingType()));
		connect(fill_factor_chk, SIGNAL(toggled(bool)), fill_factor_sb, SLOT(setEnabled(bool)));

		configureTabOrder();
		selectIndexingType();

		setMinimumSize(570, 550);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void IndexWidget::selectIndexingType()
{
	fast_update_chk->setEnabled(IndexingType(indexing_cmb->currentText())==IndexingType::Gin);
	buffering_chk->setEnabled(IndexingType(indexing_cmb->currentText())==IndexingType::Gist);
	fill_factor_sb->setEnabled(fill_factor_chk->isChecked() && fill_factor_chk->isEnabled());
}

/*void IndexWidget::enableSortingOptions()
{
	elements_tab->sorting_chk->setEnabled(IndexingType(indexing_cmb->currentText())==IndexingType::btree);
	elements_tab->ascending_rb->setEnabled(elements_tab->sorting_chk->isEnabled());
	elements_tab->descending_rb->setEnabled(elements_tab->sorting_chk->isEnabled());
	elements_tab->nulls_first_chk->setEnabled(elements_tab->sorting_chk->isEnabled());

	if(!elements_tab->sorting_chk->isEnabled())
	{
		elements_tab->sorting_chk->setChecked(false);
		elements_tab->nulls_first_chk->setChecked(false);
	}
}*/

void IndexWidget::setAttributes(DatabaseModel *model, OperationList *op_list, BaseTable *parent_obj, Index *index)
{
	vector<IndexElement> idx_elems;

	if(!parent_obj)
		throw Exception(ErrorCode::AsgNotAllocattedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	BaseObjectWidget::setAttributes(model, op_list, index, parent_obj);

	if(index)
	{
		idx_elems = index->getIndexElements();

		indexing_cmb->setCurrentIndex(indexing_cmb->findText(~index->getIndexingType()));

		fill_factor_chk->setChecked(index->getFillFactor() >= 10);

		if(fill_factor_chk->isChecked())
			fill_factor_sb->setValue(index->getFillFactor());
		else
			fill_factor_sb->setValue(90);

		concurrent_chk->setChecked(index->getIndexAttribute(Index::Concurrent));
		fast_update_chk->setChecked(index->getIndexAttribute(Index::FastUpdate));
		unique_chk->setChecked(index->getIndexAttribute(Index::Unique));
		buffering_chk->setChecked(index->getIndexAttribute(Index::Buffering));
		predicate_txt->setPlainText(index->getPredicate());

		selectIndexingType();
	}

	elements_tab->setAttributes<IndexElement>(model, parent_obj);
	elements_tab->setElements<IndexElement>(idx_elems);
}

void IndexWidget::applyConfiguration()
{
	try
	{
		Index *index=nullptr;
		vector<IndexElement> idx_elems;

		startConfiguration<Index>();

		index=dynamic_cast<Index *>(this->object);

		BaseObjectWidget::applyConfiguration();

		index->setIndexAttribute(Index::FastUpdate, fast_update_chk->isChecked());
		index->setIndexAttribute(Index::Concurrent, concurrent_chk->isChecked());
		index->setIndexAttribute(Index::Unique, unique_chk->isChecked());
		index->setIndexAttribute(Index::Buffering, buffering_chk->isChecked());
		index->setPredicate(predicate_txt->toPlainText().toUtf8());
		index->setIndexingType(IndexingType(indexing_cmb->currentText()));

		if(fill_factor_chk->isChecked())
			index->setFillFactor(fill_factor_sb->value());
		else
			index->setFillFactor(0);

		elements_tab->getElements<IndexElement>(idx_elems);
		index->addIndexElements(idx_elems);

		finishConfiguration();
	}
	catch(Exception &e)
	{
		cancelConfiguration();
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

