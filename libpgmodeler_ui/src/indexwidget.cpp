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

#include "indexwidget.h"

IndexWidget::IndexWidget(QWidget *parent): BaseObjectWidget(parent, OBJ_INDEX)
{
	try
	{
		QStringList list;
		QGridLayout *grid=nullptr;
		map<QString, vector<QWidget *> > fields_map;
		QFrame *frame=nullptr;

		Ui_IndexWidget::setupUi(this);

    predicate_hl=new SyntaxHighlighter(predicate_txt, false);
    predicate_hl->loadConfiguration(GlobalAttributes::SQL_HIGHLIGHT_CONF_PATH);

		elements_wgt = new ElementsWidget(this);

		grid=new QGridLayout;
		grid->setContentsMargins(4,4,4,4);
		grid->addWidget(elements_wgt,0,0);
		tabWidget->widget(1)->setLayout(grid);

		configureFormLayout(index_grid, OBJ_INDEX);
		parent_form->setMinimumSize(600, 600);

		IndexingType::getTypes(list);
		indexing_cmb->addItems(list);

    fields_map[BaseObjectWidget::generateVersionsInterval(BaseObjectWidget::AFTER_VERSION, PgSQLVersions::PGSQL_VERSION_92)].push_back(buffering_chk);
		frame=BaseObjectWidget::generateVersionWarningFrame(fields_map);
		frame->setParent(this);
		grid=dynamic_cast<QGridLayout *>(tabWidget->widget(0)->layout());
		grid->addWidget(frame, grid->count(), 0, 1, 5);

		connect(parent_form->apply_ok_btn,SIGNAL(clicked(bool)), this, SLOT(applyConfiguration(void)));
		connect(indexing_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(selectIndexingType(void)));
		connect(fill_factor_chk, SIGNAL(toggled(bool)), fill_factor_sb, SLOT(setEnabled(bool)));

    configureTabOrder();
		selectIndexingType();
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void IndexWidget::hideEvent(QHideEvent *event)
{
	BaseObjectWidget::hideEvent(event);

  predicate_txt->clear();
	concurrent_chk->setChecked(false);
	unique_chk->setChecked(false);
	buffering_chk->setChecked(false);
	indexing_cmb->setCurrentIndex(0);
	fill_factor_sb->setValue(90);
	tabWidget->setCurrentIndex(0);
	elements_wgt->clear();
}

void IndexWidget::selectIndexingType(void)
{
	fast_update_chk->setEnabled(IndexingType(indexing_cmb->currentText())==IndexingType::gin);
  //fill_factor_chk->setEnabled(IndexingType(indexing_cmb->currentText())==IndexingType::btree);
	buffering_chk->setEnabled(IndexingType(indexing_cmb->currentText())==IndexingType::gist);
	fill_factor_sb->setEnabled(fill_factor_chk->isChecked() && fill_factor_chk->isEnabled());
}

void IndexWidget::setAttributes(DatabaseModel *model, Table *parent_obj, OperationList *op_list, Index *index)
{
	vector<IndexElement> idx_elems;

	if(!parent_obj)
		throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

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

		concurrent_chk->setChecked(index->getIndexAttribute(Index::CONCURRENT));
		fast_update_chk->setChecked(index->getIndexAttribute(Index::FAST_UPDATE));
		unique_chk->setChecked(index->getIndexAttribute(Index::UNIQUE));
		buffering_chk->setChecked(index->getIndexAttribute(Index::BUFFERING));
    predicate_txt->setPlainText(Utf8String::create(index->getPredicate()));

		selectIndexingType();
	}

	elements_wgt->setAttributes(model, parent_obj, idx_elems);
}

void IndexWidget::applyConfiguration(void)
{
	try
	{
		Index *index=nullptr;
		vector<IndexElement> idx_elems;

		startConfiguration<Index>();

		index=dynamic_cast<Index *>(this->object);

		BaseObjectWidget::applyConfiguration();

		index->setIndexAttribute(Index::FAST_UPDATE, fast_update_chk->isChecked());
		index->setIndexAttribute(Index::CONCURRENT, concurrent_chk->isChecked());
		index->setIndexAttribute(Index::UNIQUE, unique_chk->isChecked());
		index->setIndexAttribute(Index::BUFFERING, buffering_chk->isChecked());
    index->setPredicate(predicate_txt->toPlainText().toUtf8());
		index->setIndexingType(IndexingType(indexing_cmb->currentText()));

		if(fill_factor_chk->isChecked())
			index->setFillFactor(fill_factor_sb->value());
		else
			index->setFillFactor(0);

		elements_wgt->getElements(idx_elems);
		index->addIndexElements(idx_elems);

		finishConfiguration();
	}
	catch(Exception &e)
	{
		cancelConfiguration();
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

