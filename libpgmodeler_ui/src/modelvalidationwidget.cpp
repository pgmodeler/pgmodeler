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

#include "modelvalidationwidget.h"
#include "configurationform.h"

ModelValidationWidget::ModelValidationWidget(QWidget *parent): QWidget(parent)
{
	setupUi(this);
	this->setModel(NULL);
	connect(sql_validation_chk, SIGNAL(toggled(bool)), connection_lbl, SLOT(setEnabled(bool)));
	connect(sql_validation_chk, SIGNAL(toggled(bool)), connections_cmb, SLOT(setEnabled(bool)));
	connect(validate_btn, SIGNAL(clicked(bool)), this, SLOT(validateModel(void)));
	connect(&validation_helper, SIGNAL(s_validationInfoGenerated(ValidationInfo)), this, SLOT(updateValidation(ValidationInfo)));
	connect(&validation_helper, SIGNAL(s_updateProgress(int)), validation_prog_pb, SLOT(setValue(int)));
	connect(hide_tb, SIGNAL(clicked(bool)), this, SLOT(hide(void)));
}

void ModelValidationWidget::hide(void)
{
	QWidget::hide();
	emit s_visibilityChanged(false);
}

void ModelValidationWidget::setModel(ModelWidget *model_wgt)
{
	bool enable=model_wgt!=NULL;

	this->model_wgt=model_wgt;

	output_trw->setEnabled(enable);
	validate_btn->setEnabled(enable);
	sql_validation_chk->setEnabled(enable);

	if(sql_validation_chk->isChecked())
	{
		connection_lbl->setEnabled(enable);
		connections_cmb->setEnabled(enable);
	}

	output_trw->clear();
	validation_prog_pb->setValue(0);
	prog_info_wgt->setVisible(false);
	fix_btn->setEnabled(false);
}

void ModelValidationWidget::updateConnections(map<QString, DBConnection *> &conns)
{
	map<QString, DBConnection *>::iterator itr=conns.begin();
	connections_cmb->clear();

	//Add the connections to the combo
	while(itr!=conns.end())
	{
		connections_cmb->addItem(itr->first, QVariant::fromValue<void *>(itr->second));
		itr++;
	}
}

void ModelValidationWidget::updateValidation(ValidationInfo val_info)
{
	QTreeWidgetItem *item=new QTreeWidgetItem, *item1=NULL;
	QLabel *label=new QLabel, *label1=NULL;
	vector<BaseObject *> refs;

	if(val_info.getValidationType()==ValidationInfo::BROKEN_REFERENCE)
		label->setText(trUtf8("The object <strong>%1</strong> <em>(%2)</em> [id: %3] is being referenced by <strong>%4</strong> object(s) before its creation.")
									.arg(Utf8String::create(val_info.getObject()->getName(true)))
									.arg(Utf8String::create(val_info.getObject()->getTypeName()))
									.arg(val_info.getObject()->getObjectId())
									.arg(val_info.getReferences().size()));
	else if(val_info.getValidationType()==ValidationInfo::NO_UNIQUE_NAME)
		label->setText(trUtf8("The object <strong>%1</strong> <em>(%2)</em> [id: %3] has a name that conflicts with <strong>%4</strong> object's name(s).")
									.arg(Utf8String::create(val_info.getObject()->getName(true)))
									.arg(Utf8String::create(val_info.getObject()->getTypeName()))
									.arg(val_info.getObject()->getObjectId())
									.arg(val_info.getReferences().size()));
	else
		label->setText(trUtf8("Validation failed to execute the DDL command below:"));

	output_trw->addTopLevelItem(item);

	if(val_info.getValidationType()==ValidationInfo::SQL_VALIDATION_ERR)
		item->setIcon(0, QPixmap(QString(":/icones/icones/msgbox_erro.png")));
	else
		item->setIcon(0, QPixmap(QString(":/icones/icones/msgbox_alerta.png")));

	refs=val_info.getReferences();
	while(!refs.empty())
	{
		item1=new QTreeWidgetItem(item);
		label1=new QLabel;
		item1->setIcon(0, QPixmap(QString(":/icones/icones/") + refs.back()->getSchemaName() + QString(".png")));
		label1->setText(trUtf8("Referrer object: <strong>%1</strong> <em>(%2)</em> [id: %3].")
									.arg(Utf8String::create(refs.back()->getName()))
									.arg(Utf8String::create(refs.back()->getTypeName()))
									.arg(refs.back()->getObjectId()));
		output_trw->setItemWidget(item1, 0, label1);
		refs.pop_back();
	}

	output_trw->setItemWidget(item, 0, label);
	item->setExpanded(false);

	warn_count_lbl->setText(QString("%1").arg(validation_helper.getWarningCount()));
	error_count_lbl->setText(QString("%1").arg(validation_helper.getErrorCount()));

	fix_btn->setEnabled(validation_helper.getWarningCount() > 0);
}

void ModelValidationWidget::validateModel(void)
{
	try
	{
		output_trw->clear();
		validation_prog_pb->setValue(0);
		prog_info_wgt->setVisible(true);
		validate_btn->setEnabled(false);
		model_wgt->setEnabled(false);

		validation_helper.validateModel(model_wgt->getDatabaseModel(), NULL);

		model_wgt->setEnabled(true);
		validate_btn->setEnabled(true);

		if(output_trw->topLevelItemCount()==0)
		{
			QTreeWidgetItem *item=new QTreeWidgetItem;
			QLabel *label=new QLabel;
			label->setText(trUtf8("The validation process ended up without any issues."));
			item->setIcon(0, QPixmap(QString(":/icones/icones/msgbox_info.png")));
			output_trw->addTopLevelItem(item);
			output_trw->setItemWidget(item, 0, label);
		}
	}
	catch(Exception &e)
	{
		model_wgt->setEnabled(true);
		validate_btn->setEnabled(true);
		throw Exception(ERR_VALIDATION_FAILURE,__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}
