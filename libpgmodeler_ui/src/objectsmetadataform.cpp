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

#include "objectsmetadataform.h"
#include "pgmodeleruins.h"

ObjectsMetadataForm::ObjectsMetadataForm(QWidget *parent, Qt::WindowFlags f) : QDialog(parent, f)
{
	setupUi(this);

	model_wgt=nullptr;
	settings_tbw->setTabEnabled(1, false);
	apply_btn->setEnabled(false);

	htmlitem_deleg=new HtmlItemDelegate;
	output_trw->setItemDelegateForColumn(0, htmlitem_deleg);

	connect(cancel_btn, SIGNAL(clicked()), this, SLOT(reject()));
	connect(apply_btn, SIGNAL(clicked()), this, SLOT(handleObjectsMetada()));

	connect(select_file_tb, &QToolButton::clicked,
					[=](){	selectFile(extract_from_model_rb->isChecked()); });
}

void ObjectsMetadataForm::setModelWidget(ModelWidget *model_wgt)
{
	this->model_wgt=model_wgt;
}

void ObjectsMetadataForm::handleObjectsMetada(void)
{
	try
	{
		output_trw->clear();
		apply_btn->setEnabled(true);
		settings_tbw->setTabEnabled(1, true);
		settings_tbw->setCurrentIndex(1);

		if(extract_from_model_rb->isChecked())
		{
			PgModelerUiNS::createOutputTreeItem(output_trw,
																					PgModelerUiNS::formatMessage(trUtf8("Extracting metadata to file `%1'").arg(file_edt->text())),
																					QPixmap(QString(":/icones/icones/msgbox_info.png")), nullptr);

			model_wgt->getDatabaseModel()->saveObjectsMetadata(file_edt->text());
		}
		else
		{
			PgModelerUiNS::createOutputTreeItem(output_trw,
																					PgModelerUiNS::formatMessage(trUtf8("Applying metadata from file `%1'").arg(file_edt->text())),
																					QPixmap(QString(":/icones/icones/msgbox_info.png")), nullptr);

			model_wgt->setUpdatesEnabled(false);
			model_wgt->getDatabaseModel()->loadObjectsMetadata(file_edt->text());
			model_wgt->adjustSceneSize();
			model_wgt->setUpdatesEnabled(true);
		}
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

void ObjectsMetadataForm::showEvent(QShowEvent *)
{
	apply_btn->setEnabled(model_wgt!=nullptr);
	settings_tbw->setEnabled(model_wgt!=nullptr);

	if(model_wgt)
		connect(model_wgt->getDatabaseModel(), SIGNAL(s_objectLoaded(int,QString,unsigned)), this, SLOT(updateProgress(int,QString,unsigned)));
}

void ObjectsMetadataForm::closeEvent(QCloseEvent *)
{
	if(model_wgt)
		disconnect(model_wgt->getDatabaseModel(), nullptr, this, nullptr);
}

void ObjectsMetadataForm::selectFile(bool is_output)
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

void ObjectsMetadataForm::updateProgress(int progress, QString msg, unsigned int type_id)
{
	ObjectType obj_type=static_cast<ObjectType>(type_id);
	QString fmt_msg=PgModelerUiNS::formatMessage(msg);
	QPixmap icon;

	if(obj_type==BASE_OBJECT)
		icon=QPixmap(QString(":/icones/icones/msgbox_alerta.png"));
	else
		icon=QPixmap(QString(":/icones/icones/") + BaseObject::getSchemaName(obj_type) + QString(".png"));

	PgModelerUiNS::createOutputTreeItem(output_trw, fmt_msg, icon, nullptr);
	progress_lbl->setText(fmt_msg);
	ico_lbl->setPixmap(icon);
	progress_pb->setValue(progress);

	if(progress==100)
	{
		icon=QPixmap(QString(":/icones/icones/msgbox_info.png"));
		fmt_msg=trUtf8("Metadata handling ended successfully!");
		PgModelerUiNS::createOutputTreeItem(output_trw, fmt_msg, icon, nullptr);
		ico_lbl->setPixmap(icon);
		progress_lbl->setText(fmt_msg);
	}
}
