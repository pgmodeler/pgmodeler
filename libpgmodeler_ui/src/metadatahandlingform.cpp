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

#include "metadatahandlingform.h"
#include "pgmodeleruins.h"

MetadataHandlingForm::MetadataHandlingForm(QWidget *parent, Qt::WindowFlags f) : QDialog(parent, f)
{
	setupUi(this);

	root_item=nullptr;
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

	objs_fadedout_ht=new HintTextWidget(objs_fadedout_hint, this);
	objs_fadedout_ht->setText(objs_fadedout_chk->statusTip());

	objs_extattribs_ht=new HintTextWidget(objs_extattribs_hint, this);
	objs_extattribs_ht->setText(objs_extattribs_chk->statusTip());

	custom_sql_ht=new HintTextWidget(custom_sql_hint, this);
	custom_sql_ht->setText(custom_sql_chk->statusTip());

	textbox_objs_ht=new HintTextWidget(textbox_objs_hint, this);
	textbox_objs_ht->setText(textbox_objs_chk->statusTip());

	tag_objs_ht=new HintTextWidget(tag_objs_hint, this);
	tag_objs_ht->setText(tag_objs_chk->statusTip());

	custom_colors_ht=new HintTextWidget(custom_colors_hint, this);
	custom_colors_ht->setText(custom_colors_chk->statusTip());

	extract_restore_ht=new HintTextWidget(extract_restore_hint, this);
	extract_restore_ht->setText(extract_restore_rb->statusTip());

	extract_only_ht=new HintTextWidget(extract_only_hint, this);
	extract_only_ht->setText(extract_only_rb->statusTip());

	restore_ht=new HintTextWidget(restore_hint, this);
	restore_ht->setText(restore_rb->statusTip());

	generic_sql_objs_ht=new HintTextWidget(generic_sql_objs_hint, this);
	generic_sql_objs_ht->setText(generic_sql_objs_chk->statusTip());

	objs_aliases_ht=new HintTextWidget(objs_aliases_hint, this);
	objs_aliases_ht->setText(objs_aliases_chk->statusTip());

	htmlitem_deleg=new HtmlItemDelegate(this);
	output_trw->setItemDelegateForColumn(0, htmlitem_deleg);

	connect(cancel_btn, SIGNAL(clicked()), this, SLOT(reject()));
	connect(apply_btn, SIGNAL(clicked()), this, SLOT(handleObjectsMetada()));

	connect(extract_from_cmb, &QComboBox::currentTextChanged,
					[&](){ apply_btn->setDisabled(extract_from_cmb->count() == 0); });

	connect(select_file_tb, &QToolButton::clicked,
					[&](){	selectFile(extract_restore_rb->isChecked() ||
														 extract_only_rb->isChecked()); });

	connect(extract_from_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(enableMetadataHandling()));
	connect(backup_file_edt, SIGNAL(textChanged(QString)), this, SLOT(enableMetadataHandling()));
	connect(restore_rb, SIGNAL(toggled(bool)), this, SLOT(enableMetadataHandling()));
	connect(extract_restore_rb, SIGNAL(toggled(bool)), this, SLOT(enableMetadataHandling()));
	connect(extract_only_rb, SIGNAL(toggled(bool)), this, SLOT(enableMetadataHandling()));
	connect(select_all_btn, SIGNAL(clicked(bool)), this, SLOT(selectAllOptions()));
	connect(clear_all_btn, SIGNAL(clicked(bool)), this, SLOT(selectAllOptions()));
}

void MetadataHandlingForm::enableMetadataHandling(void)
{
	extract_from_cmb->setVisible(!restore_rb->isChecked());
	extract_from_lbl->setVisible(!restore_rb->isChecked());
	apply_to_lbl->setVisible(!extract_only_rb->isChecked());
	apply_to_edt->setVisible(!extract_only_rb->isChecked());

	apply_btn->setEnabled(model_wgt &&
												(((extract_restore_rb->isChecked() && extract_from_cmb->count() > 0) ||
													(extract_only_rb->isChecked() && extract_from_cmb->count() > 0 && !backup_file_edt->text().isEmpty()) ||
													(restore_rb->isChecked() && !backup_file_edt->text().isEmpty()))));
}

void MetadataHandlingForm::selectAllOptions(void)
{
	bool check = sender() == select_all_btn;
	QCheckBox *checkbox = nullptr;

	for(auto &obj : options_grp->children())
	{
		checkbox = dynamic_cast<QCheckBox *>(obj);

		if(checkbox)
			checkbox->setChecked(check);
	}
}

void MetadataHandlingForm::setModelWidget(ModelWidget *model_wgt)
{
	this->model_wgt=model_wgt;

	apply_to_edt->clear();

	if(model_wgt)
	{
		apply_to_edt->setText(QString("%1 (%2)").arg(model_wgt->getDatabaseModel()->getName())
													.arg(model_wgt->getFilename().isEmpty() ? trUtf8("model not saved yet") : model_wgt->getFilename()));
	}
}

void MetadataHandlingForm::setModelWidgets(QList<ModelWidget *> models)
{
	extract_from_cmb->clear();

	for(ModelWidget *model : models)
	{
		extract_from_cmb->addItem(QString("%1 (%2)").arg(model->getDatabaseModel()->getName())
															.arg(model->getFilename().isEmpty() ? trUtf8("model not saved yet") : model->getFilename()),
															QVariant::fromValue<void *>(reinterpret_cast<void *>(model->getDatabaseModel())));
	}
}

void MetadataHandlingForm::handleObjectsMetada(void)
{
	if(!backup_file_edt->text().isEmpty() &&
		 backup_file_edt->text() == model_wgt->getFilename())
		throw Exception(trUtf8("The backup file cannot be the same as the input model!"),
										ERR_CUSTOM,	__PRETTY_FUNCTION__,__FILE__,__LINE__);

	QTemporaryFile tmp_file;
	QString metadata_file;
	unsigned options=0;
	DatabaseModel *extract_model=nullptr;

	try
	{
		root_item=nullptr;
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
		options+=(objs_fadedout_chk->isChecked() ? DatabaseModel::META_OBJS_FADEDOUT : 0);
		options+=(objs_extattribs_chk->isChecked() ? DatabaseModel::META_OBJS_EXTATTRIBS : 0);
		options+=(generic_sql_objs_chk->isChecked() ? DatabaseModel::META_GENERIC_SQL_OBJS : 0);
		options+=(objs_aliases_chk->isChecked() ? DatabaseModel::META_OBJS_ALIASES : 0);

		connect(model_wgt->getDatabaseModel(), SIGNAL(s_objectLoaded(int,QString,unsigned)), this, SLOT(updateProgress(int,QString,unsigned)), Qt::UniqueConnection);

		if(extract_restore_rb->isChecked() || extract_only_rb->isChecked())
		{
			extract_model=reinterpret_cast<DatabaseModel *>(extract_from_cmb->currentData(Qt::UserRole).value<void *>());

			if(extract_only_rb->isChecked())
				metadata_file = backup_file_edt->text();
			else
			{
				//Configuring the temporary metadata file
				tmp_file.setFileTemplate(GlobalAttributes::TEMPORARY_DIR +
																 GlobalAttributes::DIR_SEPARATOR +
																 QString("%1_metadata_XXXXXX.%2").arg(extract_model->getName()).arg(QString("omf")));

				tmp_file.open();
				metadata_file=tmp_file.fileName();
				tmp_file.close();
			}

			connect(extract_model, SIGNAL(s_objectLoaded(int,QString,unsigned)), this, SLOT(updateProgress(int,QString,unsigned)), Qt::UniqueConnection);

			root_item=PgModelerUiNS::createOutputTreeItem(output_trw,
																										PgModelerUiNS::formatMessage(trUtf8("Extracting metadata to file `%1'").arg(metadata_file)),
																										QPixmap(PgModelerUiNS::getIconPath("msgbox_info")), nullptr);

			extract_model->saveObjectsMetadata(metadata_file, options);

			if(extract_restore_rb->isChecked() && !backup_file_edt->text().isEmpty())
			{
				root_item->setExpanded(false);
				root_item=PgModelerUiNS::createOutputTreeItem(output_trw,
																											PgModelerUiNS::formatMessage(trUtf8("Saving backup metadata to file `%1'").arg(backup_file_edt->text())),
																											QPixmap(PgModelerUiNS::getIconPath("msgbox_info")), nullptr);

				model_wgt->getDatabaseModel()->saveObjectsMetadata(backup_file_edt->text());
			}
		}
		else
		{
			metadata_file=backup_file_edt->text();
		}

		if(root_item)
			root_item->setExpanded(false);

		if(!extract_only_rb->isChecked())
		{
			root_item=PgModelerUiNS::createOutputTreeItem(output_trw,
																										PgModelerUiNS::formatMessage(trUtf8("Applying metadata from file `%1'").arg(metadata_file)),
																										QPixmap(PgModelerUiNS::getIconPath("msgbox_info")), nullptr);

			model_wgt->setUpdatesEnabled(false);
			model_wgt->getDatabaseModel()->loadObjectsMetadata(metadata_file, options);
			model_wgt->adjustSceneSize();
			model_wgt->restoreLastCanvasPosition();
			model_wgt->setUpdatesEnabled(true);
			model_wgt->setModified(true);
			model_wgt->updateObjectsOpacity();
		}

		disconnect(model_wgt->getDatabaseModel(), nullptr, this, nullptr);

		if(extract_model)
			disconnect(extract_model, nullptr, this, nullptr);

		emit s_metadataHandled();
	}
	catch(Exception &e)
	{
		QPixmap icon=QPixmap(PgModelerUiNS::getIconPath("msgbox_erro"));

		disconnect(model_wgt->getDatabaseModel(), nullptr, this, nullptr);

		if(extract_model)
			disconnect(extract_model, nullptr, this, nullptr);

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
		file_dlg.selectFile(backup_file_edt->text());
	}
	else
	{
		file_dlg.setConfirmOverwrite(true);
		file_dlg.setFileMode(QFileDialog::AnyFile);
		file_dlg.setAcceptMode(QFileDialog::AcceptSave);
		file_dlg.selectFile(model_wgt->getDatabaseModel()->getName() + QString(".omf"));
	}

	if(file_dlg.exec()==QFileDialog::Accepted && !file_dlg.selectedFiles().isEmpty())
		backup_file_edt->setText(file_dlg.selectedFiles().at(0));
}

void MetadataHandlingForm::updateProgress(int progress, QString msg, unsigned int type_id)
{
	ObjectType obj_type=static_cast<ObjectType>(type_id);
	QString fmt_msg=PgModelerUiNS::formatMessage(msg);
	QPixmap icon;

	if(obj_type==BASE_OBJECT)
	{
		if(progress==100)
			icon=QPixmap(PgModelerUiNS::getIconPath("msgbox_info"));
		else
			icon=QPixmap(PgModelerUiNS::getIconPath("msgbox_alerta"));
	}
	else
		icon=QPixmap(PgModelerUiNS::getIconPath(obj_type));

	PgModelerUiNS::createOutputTreeItem(output_trw, fmt_msg, icon, root_item);
	progress_lbl->setText(fmt_msg);
	ico_lbl->setPixmap(icon);
	progress_pb->setValue(progress);
}
