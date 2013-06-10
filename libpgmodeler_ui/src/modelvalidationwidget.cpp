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
	vector<QString> vers;

	setupUi(this);
	this->setModel(nullptr);

	connect(validate_btn, SIGNAL(clicked(bool)), this, SLOT(validateModel(void)));
	connect(&validation_helper, SIGNAL(s_validationInfoGenerated(ValidationInfo)), this, SLOT(updateValidation(ValidationInfo)));
	connect(&validation_helper, SIGNAL(s_progressUpdated(int,QString)), this, SLOT(updateProgress(int,QString)));
	connect(hide_tb, SIGNAL(clicked(bool)), this, SLOT(hide(void)));
	connect(fix_btn, SIGNAL(clicked(bool)), this, SLOT(applyFix(void)));
	connect(clear_btn, SIGNAL(clicked(bool)), this, SLOT(clearOutput(void)));
	connect(options_btn, SIGNAL(toggled(bool)), options_frm, SLOT(setVisible(bool)));
	connect(sql_validation_chk, SIGNAL(toggled(bool)), connections_cmb, SLOT(setEnabled(bool)));
	connect(sql_validation_chk, SIGNAL(toggled(bool)), version_cmb, SLOT(setEnabled(bool)));
	connect(fix_steps_chk, SIGNAL(toggled(bool)), fix_steps_sb, SLOT(setEnabled(bool)));

	SchemaParser::getPgSQLVersions(vers);
	version_cmb->addItem(trUtf8("Autodetect"));
	while(!vers.empty())
	{
		version_cmb->addItem(vers.back());
		vers.pop_back();
	}

	options_frm->setVisible(false);
	curr_step=0;
}

void ModelValidationWidget::hide(void)
{
	QWidget::hide();
	emit s_visibilityChanged(false);
}

void ModelValidationWidget::setModel(ModelWidget *model_wgt)
{
	bool enable=model_wgt!=nullptr;

	this->model_wgt=model_wgt;
	output_trw->setEnabled(enable);
	validate_btn->setEnabled(enable);
	options_btn->setEnabled(enable);
	options_frm->setEnabled(enable);
	fix_btn->setEnabled(false);
	curr_step=0;
	clearOutput();
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
	QTreeWidgetItem *item=new QTreeWidgetItem, *item1=nullptr, *item2=nullptr;
	QLabel *label=new QLabel, *label1=nullptr, *label2=nullptr;
	vector<BaseObject *> refs;
	BaseTable *table=nullptr;
	TableObject *tab_obj=nullptr;
	QString ref_name;

	if(val_info.getValidationType()==ValidationInfo::BROKEN_REFERENCE)
		label->setText(trUtf8("The object <strong>%1</strong> <em>(%2)</em> [id: %3] is being referenced by <strong>%4</strong> object(s) before its creation.")
									.arg(Utf8String::create(val_info.getObject()->getName(true).remove("\"")))
									.arg(val_info.getObject()->getTypeName())
									.arg(val_info.getObject()->getObjectId())
									.arg(val_info.getReferences().size()));
	else if(val_info.getValidationType()==ValidationInfo::NO_UNIQUE_NAME)
	{
		tab_obj=dynamic_cast<TableObject *>(val_info.getObject());

		if(tab_obj)
		{
			table=tab_obj->getParentTable();
			ref_name=table->getName(true).remove("\"") + "." + val_info.getObject()->getName(true).remove("\"");
		}
		else
			ref_name=val_info.getObject()->getName(true).remove("\"");

		label->setText(trUtf8("The object <strong>%1</strong> <em>(%2)</em> has a name that conflicts with <strong>%3</strong> object's name(s).")
									 .arg(Utf8String::create(ref_name))
									 .arg(val_info.getObject()->getTypeName())
									 .arg(val_info.getReferences().size()));

	}
	else if(val_info.getValidationType()==ValidationInfo::SQL_VALIDATION_ERR)
		label->setText(trUtf8("SQL validation failed due to error(s) below."));
	else
		label->setText(val_info.getErrors().at(0));

	output_trw->addTopLevelItem(item);

	if(val_info.getValidationType()==ValidationInfo::SQL_VALIDATION_ERR ||
		 val_info.getValidationType()==ValidationInfo::VALIDATION_ABORTED)
	{
		QStringList errors=val_info.getErrors();
		QFont fnt;
		item->setIcon(0, QPixmap(QString(":/icones/icones/msgbox_alerta.png")));
		validation_prog_pb->setValue(validation_prog_pb->maximum());

		if(val_info.getValidationType()==ValidationInfo::SQL_VALIDATION_ERR)
		{
			//Adding all the sql errors into the output pane
			while(!errors.isEmpty())
			{
				item1=new QTreeWidgetItem(item);
				label1=new QLabel;
				label1->setText(errors.back());
				fnt=label1->font();
				fnt.setPointSizeF(8.0f);
				label1->setFont(fnt);
				output_trw->setItemWidget(item1, 0, label1);
				errors.pop_back();
			}
		}
	}
	else
	{
		item->setIcon(0, QPixmap(QString(":/icones/icones/msgbox_erro.png")));

		//Listing the referrer object on output pane
		refs=val_info.getReferences();
		while(!refs.empty())
		{
			item1=new QTreeWidgetItem(item);
			label1=new QLabel;
			item1->setIcon(0, QPixmap(QString(":/icones/icones/") + refs.back()->getSchemaName() + QString(".png")));


			if(val_info.getValidationType()==ValidationInfo::NO_UNIQUE_NAME)
			{
				TableObject *tab_obj=dynamic_cast<TableObject *>(refs.back());
				ref_name=refs.back()->getName(true);

				//If the referrer object is a table object, concatenates the parent table name
				if(tab_obj)
				{
					ref_name=dynamic_cast<TableObject *>(refs.back())->getParentTable()->getName(true) + "." + ref_name;

					if(tab_obj->isAddedByRelationship())
					{
						QPalette pal;
						item2=new QTreeWidgetItem(item1);
						label2=new QLabel;
						pal.setColor(QPalette::Text, QColor(255,0,0));
						label2->setPalette(pal);
						label2->setText(trUtf8("<em>The above object was created by a relationship. Change the name pattern on it's generator relationship. Fix will not be applied!</em>"));
						output_trw->setItemWidget(item2, 0, label2);
						item1->setExpanded(true);
					}
				}

				label1->setText(trUtf8("Conflicting object: <strong>%1</strong> <em>(%2)</em>.")
												.arg(Utf8String::create(ref_name).remove("\""))
												.arg(Utf8String::create(refs.back()->getTypeName())));
			}
			else
			{
				label1->setText(trUtf8("Referrer object: <strong>%1</strong> <em>(%2)</em> [id: %3].")
												.arg(Utf8String::create(refs.back()->getName(true)))
												.arg(Utf8String::create(refs.back()->getTypeName()))
												.arg(refs.back()->getObjectId()));
			}

			output_trw->setItemWidget(item1, 0, label1);
			refs.pop_back();
		}
	}

	output_trw->setItemWidget(item, 0, label);

	item->setExpanded(false);

	//Stores the validatin on the current tree item
	item->setData(0, Qt::UserRole, QVariant::fromValue<ValidationInfo>(val_info));

	warn_count_lbl->setText(QString("%1").arg(validation_helper.getWarningCount()));
	error_count_lbl->setText(QString("%1").arg(validation_helper.getErrorCount()));
	output_trw->scrollToBottom();
}

void ModelValidationWidget::validateModel(void)
{
	try
	{
		DBConnection *conn=nullptr;
		QString ver;

		//Get the connection only the checkbox is checked.
		if(sql_validation_chk->isChecked() && connections_cmb->count() > 0)
		{
			ver=(version_cmb->currentIndex() > 0 ? version_cmb->currentText() : "");
			conn=reinterpret_cast<DBConnection *>(connections_cmb->itemData(connections_cmb->currentIndex()).value<void *>());
		}

		clearOutput();
		prog_info_wgt->setVisible(true);
		validate_btn->setEnabled(false);
		model_wgt->setEnabled(false);

		clear_btn->setEnabled(true);
		validation_helper.validateModel(model_wgt->getDatabaseModel(), conn, ver);

		model_wgt->setEnabled(true);
		validate_btn->setEnabled(true);
		fix_btn->setEnabled(model_wgt->getDatabaseModel()->isInvalidated());
	}
	catch(Exception &e)
	{
		output_trw->scrollToBottom();
		model_wgt->setEnabled(true);
		validate_btn->setEnabled(true);
		throw Exception(ERR_VALIDATION_FAILURE,__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void ModelValidationWidget::applyFix(void)
{
	try
	{
		ValidationInfo val_info;

		val_info=output_trw->topLevelItem(0)->data(0, Qt::UserRole).value<ValidationInfo>();
		validation_helper.resolveConflict(val_info);
		model_wgt->setModified(true);

		//Every time the user apply some fix is necessary to revalidate the model
		validateModel();

		if(fix_steps_chk->isChecked() && curr_step < fix_steps_sb->value() &&
			 validation_helper.getErrorCount() > 0)
		{
			curr_step++;
			this->applyFix();
		}
		else
			curr_step=0;
	}
	catch(Exception &e)
	{
		QTreeWidgetItem *item=new QTreeWidgetItem;
		QLabel *label=new QLabel;

		label->setText(trUtf8("The validation process failed due to external errors! You can restart the validation by clicking <strong>Validate</strong> again."));
		item->setIcon(0, QPixmap(QString(":/icones/icones/msgbox_erro.png")));

		curr_step=0;
		output_trw->clear();
		output_trw->insertTopLevelItem(0, item);
		output_trw->setItemWidget(item, 0, label);

		warn_count_lbl->setText(QString("%1").arg(0));
		error_count_lbl->setText(QString("%1").arg(0));
		fix_btn->setEnabled(false);

		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void ModelValidationWidget::updateProgress(int prog, QString msg)
{
	QTreeWidgetItem *item=nullptr;
	QLabel *label=nullptr;

	validation_prog_pb->setValue(prog);

	if(prog >= 100 &&
		 validation_helper.getErrorCount()==0 && validation_helper.getWarningCount()==0)
	{
		item=new QTreeWidgetItem;
		label=new QLabel;

		label->setText(trUtf8("The validation process ended up without any issues."));
		item->setIcon(0, QPixmap(QString(":/icones/icones/msgbox_info.png")));
		warn_count_lbl->setText(QString("%1").arg(0));
		error_count_lbl->setText(QString("%1").arg(0));
		fix_btn->setEnabled(false);
		output_trw->addTopLevelItem(item);
		output_trw->setItemWidget(item, 0, label);
	}
	else if(!msg.isEmpty())
	{
		int idx=msg.indexOf('`');
		item=new QTreeWidgetItem;
		label=new QLabel;

		if(idx > 0)
		{
			msg.replace("`","<strong>");
			msg.replace("'","</strong>");
			msg.replace(" ("," <em>(");
			msg.replace(")",")</em>");
		}

		label->setText(msg);
		item->setIcon(0, QPixmap(QString(":/icones/icones/msgbox_info.png")));
		output_trw->addTopLevelItem(item);
		output_trw->setItemWidget(item, 0, label);
	}

	output_trw->scrollToBottom();
	this->repaint();
}

void ModelValidationWidget::clearOutput(void)
{
	output_trw->clear();
	clear_btn->setEnabled(false);
	prog_info_wgt->setVisible(false);
	fix_btn->setEnabled(false);
	validation_prog_pb->setValue(0);
	warn_count_lbl->setText(QString("%1").arg(0));
	error_count_lbl->setText(QString("%1").arg(0));
}
