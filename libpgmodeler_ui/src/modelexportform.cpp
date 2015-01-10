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

#include "modelexportform.h"
#include "taskprogresswidget.h"
#include "configurationform.h"
#include "pgmodelerns.h"

ModelExportForm::ModelExportForm(QWidget *parent, Qt::WindowFlags f) : QDialog(parent, f)
{
	model=nullptr;
	setupUi(this);

	export_thread=new QThread(this);
	export_hlp.moveToThread(export_thread);

  pgsqlvers_ht=new HintTextWidget(pgsqlvers_hint, this);
  pgsqlvers_ht->setText(pgsqlvers_chk->statusTip());

  drop_db_ht=new HintTextWidget(drop_db_hint, this);
  drop_db_ht->setText(drop_db_chk->statusTip());

  ignore_dup_ht=new HintTextWidget(ignore_dup_hint, this);
  ignore_dup_ht->setText(ignore_dup_chk->statusTip());

  page_by_page_ht=new HintTextWidget(page_by_page_hint, this);
  page_by_page_ht->setText(page_by_page_chk->statusTip());

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
	connect(export_thread, &QThread::started, [=](){ export_thread->setPriority(QThread::HighPriority); });

  pgsqlvers_cmb->addItems(PgSQLVersions::ALL_VERSIONS);
  pgsqlvers1_cmb->addItems(PgSQLVersions::ALL_VERSIONS);

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

void ModelExportForm::exec(ModelWidget *model)
{
	if(model)
	{
		this->model=model;    
    ConnectionsConfigWidget::fillConnectionsComboBox(connections_cmb);
    hideProgress();
    enableExportMode();
		QDialog::exec();
	}
}

void ModelExportForm::updateProgress(int progress, QString msg, ObjectType obj_type)
{
  progress_lbl->setText(PgModelerNS::formatString(msg));
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
  export_to_file_rb->setChecked(false);
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
  progress_wgt->setHidden(value);
	progress_pb->setValue(0);

	if(value)
		this->resize(this->minimumSize());
}

void ModelExportForm::enableExportMode(void)
{
  bool exp_file=(export_to_file_rb->isChecked()),
       exp_png=(export_to_img_rb->isChecked()),
       exp_dbms=(!exp_file && !exp_png);

  export_to_dbms_wgt->setEnabled(exp_dbms);
  export_to_file_wgt->setEnabled(exp_file);
  export_to_img_wgt->setEnabled(exp_png);

  pgsqlvers1_cmb->setEnabled(exp_dbms && pgsqlvers_chk->isChecked());
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
