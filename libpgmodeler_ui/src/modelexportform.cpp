/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2014 - Raphael Araújo e Silva <rkhaotix@gmail.com>
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

	model=nullptr;
	setupUi(this);

	export_thread=new QThread(this);
	export_hlp.moveToThread(export_thread);

	connect(export_to_file_rb, SIGNAL(toggled(bool)), this, SLOT(enableExportMode(void)));
	connect(export_to_dbms_rb, SIGNAL(toggled(bool)), this, SLOT(enableExportMode(void)));
	connect(export_to_img_rb, SIGNAL(toggled(bool)), this, SLOT(enableExportMode(void)));
	connect(pgsqlvers_chk, SIGNAL(toggled(bool)), pgsqlvers1_cmb, SLOT(setEnabled(bool)));
	connect(close_btn, SIGNAL(clicked(bool)), this, SLOT(close(void)));
	connect(select_file_tb, SIGNAL(clicked(void)), this, SLOT(selectOutputFile(void)));
	connect(select_img_tb, SIGNAL(clicked(void)), this, SLOT(selectOutputFile(void)));
	connect(export_btn, SIGNAL(clicked(void)), this, SLOT(exportModel(void)));

	connect(&export_hlp, SIGNAL(s_progressUpdated(int,QString,ObjectType)), this, SLOT(updateProgress(int,QString,ObjectType)));
	connect(export_thread, SIGNAL(started(void)), &export_hlp, SLOT(exportToDBMS(void)));
	connect(&export_hlp, SIGNAL(s_exportFinished(void)), this, SLOT(handleExportFinished(void)));
	connect(&export_hlp, SIGNAL(s_exportCanceled(void)), this, SLOT(handleExportCanceled(void)));
	connect(&export_hlp, SIGNAL(s_exportAborted(Exception)), this, SLOT(captureThreadError(Exception)));
	connect(cancel_btn, SIGNAL(clicked(bool)), this, SLOT(cancelExport(void)));

	connect(&timer, SIGNAL(timeout(void)), this, SLOT(hideProgress()));

	SchemaParser::getPgSQLVersions(versions);
	pgsqlvers_cmb->addItems(QStringList(QList<QString>::fromVector(QVector<QString>::fromStdVector(versions))));
	pgsqlvers1_cmb->addItems(QStringList(QList<QString>::fromVector(QVector<QString>::fromStdVector(versions))));

	#ifdef Q_OS_WIN
		this->frame->setFrameShape(QFrame::WinPanel);
	#endif

	float values[]={ ModelWidget::MINIMUM_ZOOM, 0.5f, 0.75f, 1, 1.25f, 1.50f, 1.75f, 2,
									 2.25, 2.50, 2.75, 3, 3.25, 3.50, 3.75, ModelWidget::MAXIMUM_ZOOM };
	unsigned cnt=sizeof(values)/sizeof(float);

	for(unsigned i=0; i < cnt; i++)
		zoom_cmb->addItem(QString("%1%").arg(values[i] * 100), QVariant(values[i]));

	zoom_cmb->setCurrentIndex(zoom_cmb->findText("100%"));
}

void ModelExportForm::show(ModelWidget *model)
{
	if(model)
	{
		map<QString, Connection *> connections;
		map<QString, Connection *>::iterator itr;

		this->model=model;

		//Get the current connections configured on the connections widget
		dynamic_cast<ConnectionsConfigWidget *>(configuration_form->getConfigurationWidget(ConfigurationForm::CONNECTIONS_CONF_WGT))->getConnections(connections);

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

void ModelExportForm::updateProgress(int progress, QString msg, ObjectType obj_type)
{
	msg.replace("`","<strong>");
	msg.replace("'","</strong>");
	progress_lbl->setText(msg);
	progress_pb->setValue(progress);

	if(obj_type!=BASE_OBJECT)
		ico_lbl->setPixmap(QPixmap(QString(":/icones/icones/") + BaseObject::getSchemaName(obj_type) + QString(".png")));
	else
		ico_lbl->setPixmap(QPixmap(QString(":/icones/icones/msgbox_info.png")));

	this->repaint();
}

void ModelExportForm::hideEvent(QHideEvent *)
{
	this->model=nullptr;
	file_edt->clear();
	image_edt->clear();
	pgsqlvers_chk->setChecked(false);
	ignore_dup_chk->setChecked(false);
	drop_db_chk->setChecked(false);
	export_to_file_rb->setChecked(true);
	export_btn->setEnabled(false);
	export_to_dbms_rb->setChecked(true);
	pgsqlvers1_cmb->setCurrentIndex(0);
	pgsqlvers_cmb->setCurrentIndex(0);
	show_delim_chk->setChecked(false);
	show_grid_chk->setChecked(false);
  page_by_page_chk->setChecked(false);
	connections_cmb->setCurrentIndex(0);
	zoom_cmb->setCurrentIndex(zoom_cmb->findText("100%"));
}

void ModelExportForm::exportModel(void)
{
	try
	{
		this->resize(this->maximumSize());

		timer.stop();
		hideProgress(false);

		//Export to png
		if(export_to_img_rb->isChecked())
			export_hlp.exportToPNG(model->scene, image_edt->text(), zoom_cmb->itemData(zoom_cmb->currentIndex()).toFloat(),
                             show_grid_chk->isChecked(), show_delim_chk->isChecked(), page_by_page_chk->isChecked());
		else
		{
			progress_lbl->setText(trUtf8("Initializing model export..."));

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
				Connection *conn=reinterpret_cast<Connection *>(connections_cmb->itemData(connections_cmb->currentIndex()).value<void *>());

				//If the user chose a specific version
				if(pgsqlvers1_cmb->isEnabled())
					version=pgsqlvers1_cmb->currentText();

				export_hlp.setExportToDBMSParams(model->db_model, conn, version, ignore_dup_chk->isChecked(), drop_db_chk->isChecked());
				export_thread->start();
				enableExportModes(false);
				cancel_btn->setEnabled(true);
			}
		}
	}
	catch(Exception &e)
	{
		Messagebox msg_box;

		finishExport(trUtf8("Exporting process aborted!"));
		msg_box.show(e);
	}
}

void ModelExportForm::hideProgress(bool value)
{
	ln2_frm->setHidden(value);
	progress_lbl->setHidden(value);
	progress_pb->setHidden(value);
	cancel_btn->setHidden(value);
	progress_pb->setValue(0);
	ico_lbl->setHidden(value);

	if(value)
		this->resize(this->minimumSize());
}

void ModelExportForm::enableExportMode(void)
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
  page_by_page_chk->setEnabled(exp_png);
	options_lbl->setEnabled(exp_png);
	zoom_cmb->setEnabled(exp_png);
	zoom_lbl->setEnabled(exp_png);

	modelo_sgbd->setEnabled(!exp_file && !exp_png);
	connections_lbl->setEnabled(!exp_file && !exp_png);
	connections_cmb->setEnabled(!exp_file && !exp_png);
	pgsqlvers_chk->setEnabled(!exp_file && !exp_png );
	pgsqlvers1_cmb->setEnabled(!exp_file && !exp_png && pgsqlvers_chk->isChecked());
	hint2_lbl->setEnabled(!exp_file && !exp_png);
	hint3_lbl->setEnabled(!exp_file && !exp_png);
	hint4_lbl->setEnabled(!exp_file && !exp_png);
	ignore_dup_chk->setEnabled(!exp_file && !exp_png);
	drop_db_chk->setEnabled(!exp_file && !exp_png);

	export_btn->setEnabled((export_to_dbms_rb->isChecked() && connections_cmb->count() > 0) ||
													 (export_to_file_rb->isChecked() && !file_edt->text().isEmpty()) ||
													 (export_to_img_rb->isChecked() && !image_edt->text().isEmpty()));
}

void ModelExportForm::selectOutputFile(void)
{
	QFileDialog file_dlg;

	file_dlg.setWindowTitle(trUtf8("Export model as..."));

	file_dlg.setFileMode(QFileDialog::AnyFile);
	file_dlg.setAcceptMode(QFileDialog::AcceptSave);
	file_dlg.setModal(true);

	if(export_to_file_rb->isChecked())
	{
		file_dlg.setNameFilter(trUtf8("SQL code (*.sql);;All files (*.*)"));
		file_dlg.selectFile(model->getDatabaseModel()->getName() + ".sql");
	}
	else
	{
		file_dlg.setNameFilter(trUtf8("PNG image (*.png);;All files (*.*)"));
		file_dlg.selectFile(model->getDatabaseModel()->getName() + ".png");
	}


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

void ModelExportForm::captureThreadError(Exception e)
{
	finishExport(trUtf8("Exporting process aborted!"));
	ico_lbl->setPixmap(QPixmap(QString(":/icones/icones/msgbox_erro.png")));
	throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
}

void ModelExportForm::cancelExport(void)
{
	export_hlp.cancelExport();
	cancel_btn->setEnabled(false);
}

void ModelExportForm::handleExportCanceled(void)
{
	finishExport(trUtf8("Exporting process canceled by user!"));
	ico_lbl->setPixmap(QPixmap(QString(":/icones/icones/msgbox_alerta.png")));
}

void ModelExportForm::handleExportFinished(void)
{
	finishExport(trUtf8("Exporting process sucessfuly ended!"));
	ico_lbl->setPixmap(QPixmap(QString(":/icones/icones/msgbox_info.png")));
}

void ModelExportForm::finishExport(const QString &msg)
{
	if(export_thread->isRunning())
		export_thread->quit();

	enableExportModes(true);

	cancel_btn->setEnabled(false);
	progress_pb->setValue(100);
	progress_lbl->setText(msg);
	progress_lbl->repaint();
	timer.start(5000);
}

void ModelExportForm::enableExportModes(bool value)
{
	export_to_dbms_rb->setEnabled(value);
	export_to_file_rb->setEnabled(value);
	export_to_img_rb->setEnabled(value);
	export_btn->setEnabled(value);
	close_btn->setEnabled(value);
}

void ModelExportForm::closeEvent(QCloseEvent *event)
{
	/* Ignore the close event when the thread is running this avoid
	close the form and make thread execute in background */
	if(export_thread->isRunning())
		event->ignore();
}
