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

#include "metadatahandlingform.h"
#include "pgmodeleruins.h"

MetadataHandlingForm::MetadataHandlingForm(QWidget *parent, Qt::WindowFlags f) : QDialog(parent, f)
{
	setupUi(this);

	root_item=nullptr;
	model_wgt=nullptr;
	settings_tbw->setTabEnabled(1, false);
	apply_btn->setEnabled(false);

	htmlitem_deleg=new HtmlItemDelegate(this);
	output_trw->setItemDelegateForColumn(0, htmlitem_deleg);

	backup_file_sel = new FileSelectorWidget(this);
	backup_file_sel->setNameFilters({tr("Objects metadata file (*.omf)"), tr("All files (*.*)")});
	backup_file_sel->setWindowTitle(tr("Select backup file"));
	settings_grid->addWidget(backup_file_sel, 6, 2);

	connect(cancel_btn, SIGNAL(clicked()), this, SLOT(reject()));
	connect(apply_btn, SIGNAL(clicked()), this, SLOT(handleObjectsMetada()));

	connect(extract_from_cmb, &QComboBox::currentTextChanged,
					[&](){ apply_btn->setDisabled(extract_from_cmb->count() == 0); });

	connect(extract_from_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(enableMetadataHandling()));
	connect(backup_file_sel, SIGNAL(s_selectorChanged(bool)), this, SLOT(enableMetadataHandling()));
	connect(restore_rb, SIGNAL(toggled(bool)), this, SLOT(configureSelector()));
	connect(restore_rb, SIGNAL(toggled(bool)), this, SLOT(enableMetadataHandling()));
	connect(extract_restore_rb, SIGNAL(toggled(bool)), this, SLOT(configureSelector()));
	connect(extract_restore_rb, SIGNAL(toggled(bool)), this, SLOT(enableMetadataHandling()));
	connect(extract_only_rb, SIGNAL(toggled(bool)), this, SLOT(configureSelector()));
	connect(extract_only_rb, SIGNAL(toggled(bool)), this, SLOT(enableMetadataHandling()));
	connect(select_all_btn, SIGNAL(clicked(bool)), this, SLOT(selectAllOptions()));
	connect(clear_all_btn, SIGNAL(clicked(bool)), this, SLOT(selectAllOptions()));

	configureSelector();
}

void MetadataHandlingForm::enableMetadataHandling()
{
	extract_from_cmb->setVisible(!restore_rb->isChecked());
	extract_from_lbl->setVisible(!restore_rb->isChecked());
	apply_to_lbl->setVisible(!extract_only_rb->isChecked());
	apply_to_edt->setVisible(!extract_only_rb->isChecked());

	apply_btn->setEnabled(model_wgt &&
												(((extract_restore_rb->isChecked() && extract_from_cmb->count() > 0) ||
													(extract_only_rb->isChecked() && extract_from_cmb->count() > 0 && !backup_file_sel->getSelectedFile().isEmpty() && !backup_file_sel->hasWarning()) ||
													(restore_rb->isChecked() && !backup_file_sel->getSelectedFile().isEmpty() && !backup_file_sel->hasWarning()))));
}

void MetadataHandlingForm::selectAllOptions()
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
													.arg(model_wgt->getFilename().isEmpty() ? tr("model not saved yet") : model_wgt->getFilename()));
	}
}

void MetadataHandlingForm::setModelWidgets(QList<ModelWidget *> models)
{
	extract_from_cmb->clear();

	for(ModelWidget *model : models)
	{
		extract_from_cmb->addItem(QString("%1 (%2)").arg(model->getDatabaseModel()->getName())
															.arg(model->getFilename().isEmpty() ? tr("model not saved yet") : model->getFilename()),
															QVariant::fromValue<void *>(reinterpret_cast<void *>(model->getDatabaseModel())));
	}
}

void MetadataHandlingForm::handleObjectsMetada()
{
	if(!backup_file_sel->getSelectedFile().isEmpty() &&
		 backup_file_sel->getSelectedFile() == model_wgt->getFilename())
		throw Exception(tr("The backup file cannot be the same as the input model!"),
										ErrorCode::Custom,	__PRETTY_FUNCTION__,__FILE__,__LINE__);

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

		options+=(db_metadata_chk->isChecked() ? DatabaseModel::MetaDbAttributes : 0);
		options+=(custom_colors_chk->isChecked() ? DatabaseModel::MetaObjsCustomColors : 0);
		options+=(custom_sql_chk->isChecked() ? DatabaseModel::MetaObjsCustomSql : 0);
		options+=(objs_positioning_chk->isChecked() ? DatabaseModel::MetaObjsPositioning : 0);
		options+=(objs_protection_chk->isChecked() ? DatabaseModel::MetaObjsProtection : 0);
		options+=(objs_sql_disabled_chk->isChecked() ? DatabaseModel::MetaObjsSqlDisabled : 0);
		options+=(tag_objs_chk->isChecked() ? DatabaseModel::MetaTagObjs : 0);
		options+=(textbox_objs_chk->isChecked() ? DatabaseModel::MetaTextboxObjs : 0);
		options+=(objs_fadedout_chk->isChecked() ? DatabaseModel::MetaObjsFadeOut : 0);
		options+=(objs_collapse_mode_chk->isChecked() ? DatabaseModel::MetaObjsCollapseMode : 0);
		options+=(generic_sql_objs_chk->isChecked() ? DatabaseModel::MetaGenericSqlObjs : 0);
		options+=(objs_aliases_chk->isChecked() ? DatabaseModel::MetaObjsAliases : 0);
		options+=(objs_z_stack_value_chk->isChecked() ? DatabaseModel::MetaObjsZStackValue : 0);

		connect(model_wgt->getDatabaseModel(), SIGNAL(s_objectLoaded(int,QString,unsigned)), this, SLOT(updateProgress(int,QString,unsigned)), Qt::UniqueConnection);

		if(extract_restore_rb->isChecked() || extract_only_rb->isChecked())
		{
			extract_model=reinterpret_cast<DatabaseModel *>(extract_from_cmb->currentData(Qt::UserRole).value<void *>());

			if(extract_only_rb->isChecked())
				metadata_file = backup_file_sel->getSelectedFile();
			else
			{
				//Configuring the temporary metadata file
				tmp_file.setFileTemplate(GlobalAttributes::getTemporaryFilePath(
																	 QString("%1_metadata_XXXXXX.%2")
																	 .arg(extract_model->getName())
																	 .arg(QString("omf"))));

				tmp_file.open();
				metadata_file=tmp_file.fileName();
				tmp_file.close();
			}

			connect(extract_model, SIGNAL(s_objectLoaded(int,QString,unsigned)), this, SLOT(updateProgress(int,QString,unsigned)), Qt::UniqueConnection);

			root_item=PgModelerUiNs::createOutputTreeItem(output_trw,
																										PgModelerUiNs::formatMessage(tr("Extracting metadata to file `%1'").arg(metadata_file)),
																										QPixmap(PgModelerUiNs::getIconPath("msgbox_info")), nullptr);

			extract_model->saveObjectsMetadata(metadata_file, options);

			if(extract_restore_rb->isChecked() && !backup_file_sel->getSelectedFile().isEmpty())
			{
				root_item->setExpanded(false);
				root_item=PgModelerUiNs::createOutputTreeItem(output_trw,
																											PgModelerUiNs::formatMessage(tr("Saving backup metadata to file `%1'").arg(backup_file_sel->getSelectedFile())),
																											QPixmap(PgModelerUiNs::getIconPath("msgbox_info")), nullptr);

				model_wgt->getDatabaseModel()->saveObjectsMetadata(backup_file_sel->getSelectedFile());
			}
		}
		else
		{
			metadata_file = backup_file_sel->getSelectedFile();
		}

		if(root_item)
			root_item->setExpanded(false);

		if(!extract_only_rb->isChecked())
		{
			root_item=PgModelerUiNs::createOutputTreeItem(output_trw,
																										PgModelerUiNs::formatMessage(tr("Applying metadata from file `%1'").arg(metadata_file)),
																										QPixmap(PgModelerUiNs::getIconPath("msgbox_info")), nullptr);

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
		QPixmap icon=QPixmap(PgModelerUiNs::getIconPath("msgbox_erro"));

		disconnect(model_wgt->getDatabaseModel(), nullptr, this, nullptr);

		if(extract_model)
			disconnect(extract_model, nullptr, this, nullptr);

		PgModelerUiNs::createOutputTreeItem(output_trw,
																				PgModelerUiNs::formatMessage(e.getErrorMessage()),
																				icon, nullptr);

		ico_lbl->setPixmap(icon);
		progress_lbl->setText(tr("Metadata processing aborted!"));

		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
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

void MetadataHandlingForm::configureSelector()
{
	if(extract_restore_rb->isChecked() || extract_only_rb->isChecked())
	{
		backup_file_sel->setFileDialogTitle(tr("Save backup file"));
		backup_file_sel->setFileMode(QFileDialog::AnyFile);
		backup_file_sel->setAcceptMode(QFileDialog::AcceptSave);
	}
	else
	{
		backup_file_sel->setFileDialogTitle(tr("Load backup file"));
		backup_file_sel->setFileMode(QFileDialog::ExistingFiles);
		backup_file_sel->setAcceptMode(QFileDialog::AcceptOpen);
	}
}

void MetadataHandlingForm::updateProgress(int progress, QString msg, unsigned int type_id)
{
	ObjectType obj_type=static_cast<ObjectType>(type_id);
	QString fmt_msg=PgModelerUiNs::formatMessage(msg);
	QPixmap icon;

	if(obj_type==ObjectType::BaseObject)
	{
		if(progress==100)
			icon=QPixmap(PgModelerUiNs::getIconPath("msgbox_info"));
		else
			icon=QPixmap(PgModelerUiNs::getIconPath("msgbox_alerta"));
	}
	else
		icon=QPixmap(PgModelerUiNs::getIconPath(obj_type));

	PgModelerUiNs::createOutputTreeItem(output_trw, fmt_msg, icon, root_item);
	progress_lbl->setText(fmt_msg);
	ico_lbl->setPixmap(icon);
	progress_pb->setValue(progress);
}
