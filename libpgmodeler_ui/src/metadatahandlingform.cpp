/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2016 - Raphael Araújo e Silva <raphael@pgmodeler.com.br>
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

#include "metadatahandlingform.h"
#include "pgmodeleruins.h"

MetadataHandlingForm::MetadataHandlingForm(QWidget *parent, Qt::WindowFlags f) : QDialog(parent, f)
{
	setupUi(this);

	model_wgt=nullptr;
	settings_tbw->setTabEnabled(1, false);
	apply_btn->setEnabled(false);

	db_metadata_ht=new HintTextWidget(db_metadata_hint, this);
	db_metadata_ht->setText(db_metadata_chk->statusTip());

	objs_positioning_ht=new HintTextWidget(objs_positioning_hint, this);
	objs_positioning_ht->setText(objs_positioning_chk->statusTip());

	objs_protection_ht=new HintTextWidget(objs_protection_hint, this);
	objs_protection_ht->setText(objs_protection_chk->statusTip());

	objs_sql_disabled_ht=new HintTextWidget(objs_sql_disabled_hint, this);
	objs_sql_disabled_ht->setText(objs_sql_disabled_chk->statusTip());

	custom_sql_ht=new HintTextWidget(custom_sql_hint, this);
	custom_sql_ht->setText(custom_sql_chk->statusTip());

	textbox_objs_ht=new HintTextWidget(textbox_objs_hint, this);
	textbox_objs_ht->setText(textbox_objs_chk->statusTip());

	tag_objs_ht=new HintTextWidget(tag_objs_hint, this);
	tag_objs_ht->setText(tag_objs_chk->statusTip());

	custom_colors_ht=new HintTextWidget(custom_colors_hint, this);
	custom_colors_ht->setText(custom_colors_chk->statusTip());

	htmlitem_deleg=new HtmlItemDelegate;
	output_trw->setItemDelegateForColumn(0, htmlitem_deleg);

	connect(cancel_btn, SIGNAL(clicked()), this, SLOT(reject()));
	connect(apply_btn, SIGNAL(clicked()), this, SLOT(handleObjectsMetada()));

	connect(select_file_tb, &QToolButton::clicked,
					[=](){	selectFile(save_metadata_rb->isChecked()); });

	connect(file_edt, &QLineEdit::textChanged,
					[=](){ apply_btn->setDisabled(file_edt->text().isEmpty()); });
}

void MetadataHandlingForm::setModelWidget(ModelWidget *model_wgt)
{
	this->model_wgt=model_wgt;
}

void MetadataHandlingForm::handleObjectsMetada(void)
{
	try
	{
		unsigned options=0;

		output_trw->clear();
		settings_tbw->setTabEnabled(1, true);
		settings_tbw->setCurrentIndex(1);

		options+=(db_metadata_chk->isChecked() ? DatabaseModel::META_DB_ATTRIBUTES : 0);
		options+=(custom_colors_chk->isChecked() ? DatabaseModel::META_OBJS_CUSTOMCOLORS : 0);
		options+=(custom_sql_chk->isChecked() ? DatabaseModel::META_OBJS_CUSTOMSQL : 0);
		options+=(objs_positioning_chk->isChecked() ? DatabaseModel::META_OBJS_POSITIONING : 0);
		options+=(objs_protection_chk->isChecked() ? DatabaseModel::META_OBJS_PROTECTION : 0);
		options+=(objs_sql_disabled_chk->isChecked() ? DatabaseModel::META_OBJS_SQLDISABLED : 0);
		options+=(tag_objs_chk->isChecked() ? DatabaseModel::META_TAG_OBJS : 0);
		options+=(textbox_objs_chk->isChecked() ? DatabaseModel::META_TEXTBOX_OBJS : 0);

		if(save_metadata_rb->isChecked())
		{
			PgModelerUiNS::createOutputTreeItem(output_trw,
																					PgModelerUiNS::formatMessage(trUtf8("Extracting metadata to file `%1'").arg(file_edt->text())),
																					QPixmap(QString(":/icones/icones/msgbox_info.png")), nullptr);

			model_wgt->getDatabaseModel()->saveObjectsMetadata(file_edt->text(), options);
			emit s_metadataSaved();
		}
		else
		{
			PgModelerUiNS::createOutputTreeItem(output_trw,
																					PgModelerUiNS::formatMessage(trUtf8("Applying metadata from file `%1'").arg(file_edt->text())),
																					QPixmap(QString(":/icones/icones/msgbox_info.png")), nullptr);

			model_wgt->setUpdatesEnabled(false);
			model_wgt->getDatabaseModel()->loadObjectsMetadata(file_edt->text(), options);
			model_wgt->adjustSceneSize();
			model_wgt->restoreLastCanvasPosition();
			model_wgt->setUpdatesEnabled(true);
			model_wgt->setModified(true);

			emit s_metadataLoaded();
		}

		this->accept();
	}
	catch(Exception &e)
	{
		QPixmap icon=QPixmap(QString(":/icones/icones/msgbox_erro.png"));

		PgModelerUiNS::createOutputTreeItem(output_trw,
																				PgModelerUiNS::formatMessage(e.getErrorMessage()),
																				icon, nullptr);

		ico_lbl->setPixmap(icon);
		progress_lbl->setText(trUtf8("Metadata processing aborted!"));

		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void MetadataHandlingForm::showEvent(QShowEvent *)
{
	if(!model_wgt)
	{
		apply_btn->setEnabled(false);
		settings_tbw->setEnabled(false);
	}

	if(model_wgt)
		connect(model_wgt->getDatabaseModel(), SIGNAL(s_objectLoaded(int,QString,unsigned)), this, SLOT(updateProgress(int,QString,unsigned)));
}

void MetadataHandlingForm::closeEvent(QCloseEvent *)
{
	if(model_wgt)
		disconnect(model_wgt->getDatabaseModel(), nullptr, this, nullptr);
}

void MetadataHandlingForm::selectFile(bool is_output)
{
	QFileDialog file_dlg;

	file_dlg.setNameFilter(trUtf8("Objects metadata file (*.omf);;All files (*.*)"));
	file_dlg.setWindowTitle(trUtf8("Select file"));

	if(!is_output)
	{
		file_dlg.setFileMode(QFileDialog::ExistingFiles);
		file_dlg.setAcceptMode(QFileDialog::AcceptOpen);
		file_dlg.selectFile(file_edt->text());
	}
	else
	{
		file_dlg.setConfirmOverwrite(true);
		file_dlg.setFileMode(QFileDialog::AnyFile);
		file_dlg.setAcceptMode(QFileDialog::AcceptSave);
		file_dlg.selectFile(model_wgt->getDatabaseModel()->getName() + QString(".omf"));
	}

	if(file_dlg.exec()==QFileDialog::Accepted && !file_dlg.selectedFiles().isEmpty())
		file_edt->setText(file_dlg.selectedFiles().at(0));
}

void MetadataHandlingForm::updateProgress(int progress, QString msg, unsigned int type_id)
{
	ObjectType obj_type=static_cast<ObjectType>(type_id);
	QString fmt_msg=PgModelerUiNS::formatMessage(msg);
	QPixmap icon;

	if(obj_type==BASE_OBJECT)
	{
		if(progress==100)
			icon=QPixmap(QString(":/icones/icones/msgbox_info.png"));
		else
			icon=QPixmap(QString(":/icones/icones/msgbox_alerta.png"));
	}
	else
		icon=QPixmap(QString(":/icones/icones/") + BaseObject::getSchemaName(obj_type) + QString(".png"));

	PgModelerUiNS::createOutputTreeItem(output_trw, fmt_msg, icon, nullptr);
	progress_lbl->setText(fmt_msg);
	ico_lbl->setPixmap(icon);
	progress_pb->setValue(progress);
}
