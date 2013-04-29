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

#include "modelexportform.h"
#include "taskprogresswidget.h"
#include "configurationform.h"

extern ConfigurationForm *configuration_form;

ModelExportForm::ModelExportForm(QWidget *parent, Qt::WindowFlags f) : QDialog(parent, f)
{
	vector<QString> versions;

	model=NULL;

	setupUi(this);
	connect(export_to_file_rb, SIGNAL(toggled(bool)), this, SLOT(enableExportType(void)));
	connect(export_to_dbms_rb, SIGNAL(toggled(bool)), this, SLOT(enableExportType(void)));
	connect(export_to_img_rb, SIGNAL(toggled(bool)), this, SLOT(enableExportType(void)));
	connect(pgsqlvers_chk, SIGNAL(toggled(bool)), pgsqlvers1_cmb, SLOT(setEnabled(bool)));
	connect(close_btn, SIGNAL(clicked(bool)), this, SLOT(close(void)));
	connect(select_file_tb, SIGNAL(clicked(void)), this, SLOT(selectOutputFile(void)));
	connect(select_img_tb, SIGNAL(clicked(void)), this, SLOT(selectOutputFile(void)));
	connect(export_btn, SIGNAL(clicked(void)), this, SLOT(exportModel(void)));

	SchemaParser::getPgSQLVersions(versions);
	pgsqlvers_cmb->addItems(QStringList(QList<QString>::fromVector(QVector<QString>::fromStdVector(versions))));
	pgsqlvers1_cmb->addItems(QStringList(QList<QString>::fromVector(QVector<QString>::fromStdVector(versions))));

	#ifdef Q_OS_WIN
		this->frame->setFrameShape(QFrame::WinPanel);
	#endif
}

void ModelExportForm::show(ModelWidget *model)
{
	if(model)
	{
		map<QString, DBConnection *> connections;
		map<QString, DBConnection *>::iterator itr;

		this->model=model;

		//Get the current connections configured on the connections widget
		dynamic_cast<ConnectionsConfigWidget *>(configuration_form->getConfigurationWidget(ConfigurationForm::CONNECTIONS_CONF_WGT))->getConnections(connections, true);

		connections_cmb->clear();
		itr=connections.begin();

		//Add the connections to the combo
		while(itr!=connections.end())
		{
			connections_cmb->addItem(itr->first, QVariant::fromValue<void *>(itr->second));
			itr++;
		}

		this->hideProgress();
		QDialog::show();
	}
}

void ModelExportForm::updateProgress(int progress, QString msg)
{
 progress_lbl->setText(msg);
 progress_pb->setValue(progress);
 this->repaint();
}

void ModelExportForm::hideEvent(QHideEvent *)
{
	this->model=NULL;
	file_edt->clear();
	export_to_file_rb->setChecked(true);
	export_btn->setEnabled(false);
}

void ModelExportForm::exportModel(void)
{
	try
	{
		ModelExportHelper export_hlp(this->model);

		this->resize(this->maximumSize());

		progress_pb->setValue(0);
		ln2_frm->setVisible(true);
		progress_lbl->setVisible(true);
		progress_pb->setVisible(true);
		ico_lbl->setVisible(false);

		//Export to png
		if(export_to_img_rb->isChecked())
			export_hlp.exportToPNG(model->scene, image_edt->text(), show_grid_chk->isChecked(), show_delim_chk->isChecked());
		else
		{
			try
			{
				progress_lbl->setText(trUtf8("Initializing model export..."));
				progress_lbl->repaint();
				connect(&export_hlp, SIGNAL(s_progressUpdated(int,QString)), this, SLOT(updateProgress(int,QString)));

				//Exporting to sql file
				if(export_to_file_rb->isChecked())
				{
					progress_lbl->setText(trUtf8("Saving file '%1'").arg(file_edt->text()));
					export_hlp.exportToSQL(model->db_model, file_edt->text(), pgsqlvers_cmb->currentText());
				}
				//Exporting directly to DBMS
				else
				{
					QString version;
					DBConnection conn=*(reinterpret_cast<DBConnection *>(connections_cmb->itemData(connections_cmb->currentIndex()).value<void *>()));

					//If the user chose a specific version
					if(pgsqlvers1_cmb->isEnabled())
						version=pgsqlvers1_cmb->currentText();

					export_hlp.exportToDBMS(model->db_model, conn, version, ignorar_dup_chk->isChecked());
				}
			}
			catch(Exception &e)
			{		
				disconnect(&export_hlp, NULL, this, NULL);
				throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
			}
		}

		progress_pb->setValue(100);
		progress_lbl->setText(trUtf8("Exporting process sucessfuly ended!"));
		progress_lbl->repaint();
		ico_lbl->setPixmap(QPixmap(QString(":/icones/icones/msgbox_info.png")));
		ico_lbl->setVisible(true);
		QTimer::singleShot(5000, this, SLOT(hideProgress(void)));
		disconnect(&export_hlp, NULL, this, NULL);
	}
	catch(Exception &e)
	{
		MessageBox msg_box;

		progress_lbl->setText(trUtf8("Error on export!"));
		progress_lbl->repaint();
		ico_lbl->setPixmap(QPixmap(QString(":/icones/icones/msgbox_erro.png")));
		ico_lbl->setVisible(true);

		QTimer::singleShot(5000, this, SLOT(hideProgress(void)));
		msg_box.show(e);
	}
}

void ModelExportForm::hideProgress(void)
{
	ln2_frm->setVisible(false);
	progress_lbl->setVisible(false);
	progress_pb->setVisible(false);
	ico_lbl->setVisible(false);
	this->resize(this->minimumSize());
}

void ModelExportForm::enableExportType(void)
{
	bool exp_file=(sender()==export_to_file_rb), exp_png=(sender()==export_to_img_rb);

	model_sql_lbl->setEnabled(exp_file);
	file_lbl->setEnabled(exp_file);
	file_edt->setEnabled(exp_file);
	select_file_tb->setEnabled(exp_file);
	pgsql_lbl->setEnabled(exp_file);
	pgsqlvers_cmb->setEnabled(exp_file);
	hint_lbl->setEnabled(exp_file);

	model_png_lbl->setEnabled(exp_png);
	image_lbl->setEnabled(exp_png);
	image_edt->setEnabled(exp_png);
	select_img_tb->setEnabled(exp_png);
	hint1_lbl->setEnabled(exp_png);
	show_grid_chk->setEnabled(exp_png);
	show_delim_chk->setEnabled(exp_png);
	options_lbl->setEnabled(exp_png);

	modelo_sgbd->setEnabled(!exp_file && !exp_png);
	connections_lbl->setEnabled(!exp_file && !exp_png);
	connections_cmb->setEnabled(!exp_file && !exp_png);
	pgsqlvers_chk->setEnabled(!exp_file && !exp_png );
	pgsqlvers1_cmb->setEnabled(!exp_file && !exp_png && pgsqlvers_chk->isChecked());
	hint2_lbl->setEnabled(!exp_file && !exp_png);
	hint3_lbl->setEnabled(!exp_file && !exp_png);
	ignorar_dup_chk->setEnabled(!exp_file && !exp_png);

	export_btn->setEnabled((export_to_dbms_rb->isChecked() && connections_cmb->count() > 0) ||
													 (export_to_file_rb->isChecked() && !file_edt->text().isEmpty()) ||
													 (export_to_img_rb->isChecked() && !image_edt->text().isEmpty()));
}

void ModelExportForm::selectOutputFile(void)
{
	QFileDialog file_dlg;

	file_dlg.setWindowTitle(trUtf8("Export model as..."));

	if(export_to_file_rb->isChecked())
		file_dlg.setFilter(trUtf8("SQL code (*.sql);;All files (*.*)"));
	else
		file_dlg.setFilter(trUtf8("PNG image (*.png);;All files (*.*)"));

	file_dlg.setFileMode(QFileDialog::AnyFile);
	file_dlg.setAcceptMode(QFileDialog::AcceptSave);
	file_dlg.setModal(true);

	if(file_dlg.exec()==QFileDialog::Accepted)
	{
		QString file;

		if(!file_dlg.selectedFiles().isEmpty())
			file = file_dlg.selectedFiles().at(0);

		if(export_to_file_rb->isChecked())
			file_edt->setText(file);
		else
			image_edt->setText(file);
	}

	export_btn->setEnabled(!file_edt->text().isEmpty() || !image_edt->text().isEmpty());
}

