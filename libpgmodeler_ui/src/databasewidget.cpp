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

#include "databasewidget.h"

DatabaseWidget::DatabaseWidget(QWidget *parent): BaseObjectWidget(parent, OBJ_DATABASE)
{
	map<QString, vector<QWidget *> > fields_map;
	QLocale loc;
	QStringList loc_list, encodings;

	Ui_DatabaseWidget::setupUi(this);

	connect(parent_form->apply_ok_btn,SIGNAL(clicked(bool)), this, SLOT(applyConfiguration(void)));
	configureFormLayout(database_grid, OBJ_DATABASE);

	parent_form->setMinimumWidth(530);
	parent_form->setMinimumHeight(360);
	parent_form->setMaximumHeight(360);

	//Configures the encoding combobox
	EncodingType::getTypes(encodings);
	encodings.push_front(trUtf8("Default"));
	encoding_cmb->addItems(encodings);

	//Configures the localizations combobox
	for(int i=QLocale::C; i <= QLocale::Chewa; i++)
	{
		for(int i1=QLocale::Afghanistan; i1 <= QLocale::Zimbabwe; i1++)
			loc_list.append(QLocale(static_cast<QLocale::Language>(i),static_cast<QLocale::Country>(i1)).name());
	}

	loc_list.removeDuplicates();
	loc_list.sort();
	loc_list.push_front(trUtf8("Default"));

	lccollate_cmb->addItems(loc_list);
	lcctype_cmb->addItems(loc_list);
}

void DatabaseWidget::setAttributes(DatabaseModel *model)
{
	if(model)
	{
		int idx;

		connlim_sb->setValue(model->getConnectionLimit());
		templatedb_edt->setText(Utf8String::create(model->getTemplateDB()));
		author_edt->setText(Utf8String::create(model->getAuthor()));

		idx=encoding_cmb->findText(~model->getEncoding());
		if(idx < 0) idx=0;
		encoding_cmb->setCurrentIndex(idx);

		idx=lccollate_cmb->findText(model->getLocalization(LC_COLLATE));
		if(idx < 0) idx=0;
		lccollate_cmb->setCurrentIndex(idx);

		idx=lcctype_cmb->findText(model->getLocalization(LC_CTYPE));
		if(idx < 0) idx=0;
		lcctype_cmb->setCurrentIndex(idx);

		BaseObjectWidget::setAttributes(model, model, nullptr);
	}
}

void DatabaseWidget::applyConfiguration(void)
{
	try
	{
		//Apply the basic configurations
		BaseObjectWidget::applyConfiguration();

		model->setAuthor(author_edt->text().toUtf8());

		if(encoding_cmb->currentIndex() > 0)
			model->setEncoding(EncodingType(encoding_cmb->currentText()));

		if(lccollate_cmb->currentIndex() > 0)
			model->setLocalization(LC_COLLATE, lccollate_cmb->currentText());
		else
			model->setLocalization(LC_COLLATE, "");

		if(lcctype_cmb->currentIndex() > 0)
			model->setLocalization(LC_CTYPE, lcctype_cmb->currentText());
		else
			model->setLocalization(LC_CTYPE, "");

		finishConfiguration();
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}
