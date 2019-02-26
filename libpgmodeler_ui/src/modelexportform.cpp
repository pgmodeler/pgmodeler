/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2019 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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
#include "pgmodeleruins.h"

ModelExportForm::ModelExportForm(QWidget *parent, Qt::WindowFlags f) : QDialog(parent, f)
{
	model=nullptr;
	viewp=nullptr;
	setupUi(this);

	htmlitem_del=new HtmlItemDelegate(this);
	output_trw->setItemDelegateForColumn(0, htmlitem_del);

	export_thread=new QThread(this);
	export_hlp.moveToThread(export_thread);

	pgsqlvers_ht=new HintTextWidget(pgsqlvers_hint, this);
	pgsqlvers_ht->setText(pgsqlvers_chk->statusTip());

	drop_ht=new HintTextWidget(drop_hint, this);
	drop_ht->setText(drop_chk->statusTip());

	ignore_dup_ht=new HintTextWidget(ignore_dup_hint, this);
	ignore_dup_ht->setText(ignore_dup_chk->statusTip());

	page_by_page_ht=new HintTextWidget(page_by_page_hint, this);
	page_by_page_ht->setText(page_by_page_chk->statusTip());

	ignore_error_codes_ht=new HintTextWidget(ignore_extra_errors_hint, this);
	ignore_error_codes_ht->setText(ignore_error_codes_chk->statusTip());

	connect(export_to_file_rb, SIGNAL(clicked()), this, SLOT(selectExportMode(void)));
	connect(export_to_dbms_rb, SIGNAL(clicked()), this, SLOT(selectExportMode(void)));
	connect(export_to_img_rb, SIGNAL(clicked()), this, SLOT(selectExportMode(void)));
	connect(pgsqlvers_chk, SIGNAL(toggled(bool)), pgsqlvers1_cmb, SLOT(setEnabled(bool)));
	connect(close_btn, SIGNAL(clicked(bool)), this, SLOT(close(void)));
	connect(select_file_tb, SIGNAL(clicked(void)), this, SLOT(selectOutputFile(void)));
	connect(select_img_tb, SIGNAL(clicked(void)), this, SLOT(selectOutputFile(void)));
	connect(export_btn, SIGNAL(clicked(void)), this, SLOT(exportModel(void)));
	connect(drop_chk, SIGNAL(toggled(bool)), drop_db_rb, SLOT(setEnabled(bool)));
	connect(drop_chk, SIGNAL(toggled(bool)), drop_objs_rb, SLOT(setEnabled(bool)));

	connect(export_thread, &QThread::started,
	[&](){

		output_trw->setUniformRowHeights(true);

		if(export_to_dbms_rb->isChecked())
			export_hlp.exportToDBMS();
		else if(export_to_img_rb->isChecked())
		{
			if(png_rb->isChecked())
				export_hlp.exportToPNG();
			else
				export_hlp.exportToSVG();
		}
		else
			export_hlp.exportToSQL();
	});

	connect(export_thread, &QThread::finished, [&](){
		output_trw->setUniformRowHeights(false);
	});

	connect(&export_hlp, SIGNAL(s_progressUpdated(int,QString,ObjectType,QString,bool)), this, SLOT(updateProgress(int,QString,ObjectType,QString,bool)), Qt::BlockingQueuedConnection);
	connect(&export_hlp, SIGNAL(s_exportFinished(void)), this, SLOT(handleExportFinished(void)));
	connect(&export_hlp, SIGNAL(s_exportCanceled(void)), this, SLOT(handleExportCanceled(void)));
	connect(&export_hlp, SIGNAL(s_errorIgnored(QString,QString,QString)), this, SLOT(handleErrorIgnored(QString,QString,QString)));
	connect(&export_hlp, SIGNAL(s_exportAborted(Exception)), this, SLOT(captureThreadError(Exception)));
	connect(cancel_btn, SIGNAL(clicked(bool)), this, SLOT(cancelExport(void)));
	connect(connections_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(editConnections(void)));
	connect(svg_rb, SIGNAL(toggled(bool)), zoom_cmb, SLOT(setDisabled(bool)));
	connect(svg_rb, SIGNAL(toggled(bool)), zoom_lbl, SLOT(setDisabled(bool)));
	connect(svg_rb, SIGNAL(toggled(bool)), page_by_page_chk, SLOT(setDisabled(bool)));
	connect(ignore_error_codes_chk, SIGNAL(toggled(bool)), error_codes_edt, SLOT(setEnabled(bool)));

	pgsqlvers_cmb->addItems(PgSqlVersions::AllVersions);
	pgsqlvers1_cmb->addItems(PgSqlVersions::AllVersions);

	double values[]={ ModelWidget::MinimumZoom, 0.10f, 0.25f, 0.5f, 0.75f, 1, 1.25f, 1.50f, 1.75f, 2,
										2.25, 2.50, 2.75, 3, 3.25, 3.50, 3.75, ModelWidget::MaximumZoom };
	unsigned cnt=sizeof(values)/sizeof(double);

	for(unsigned i=0; i < cnt; i++)
		zoom_cmb->addItem(QString("%1%").arg(values[i] * 100), QVariant(values[i]));

	zoom_cmb->setCurrentText(QString("100%"));

	settings_tbw->setTabEnabled(1, false);
}

void ModelExportForm::exec(ModelWidget *model)
{
	if(model)
	{
		this->model=model;
		ConnectionsConfigWidget::fillConnectionsComboBox(connections_cmb, true, Connection::OpExport);
		selectExportMode();
		QDialog::exec();
	}
}

void ModelExportForm::handleErrorIgnored(QString err_code, QString err_msg, QString cmd)
{
	QTreeWidgetItem *item=nullptr;

	item=PgModelerUiNs::createOutputTreeItem(output_trw, trUtf8("Error code <strong>%1</strong> found and ignored. Proceeding with export.").arg(err_code),
																					 QPixmap(PgModelerUiNs::getIconPath("msgbox_alerta")), nullptr, false);

	PgModelerUiNs::createOutputTreeItem(output_trw, PgModelerUiNs::formatMessage(err_msg),
																			QPixmap(PgModelerUiNs::getIconPath("msgbox_alerta")),	item, false);

	PgModelerUiNs::createOutputTreeItem(output_trw, cmd, QPixmap(), item, false);
}

void ModelExportForm::updateProgress(int progress, QString msg, ObjectType obj_type, QString cmd, bool is_code_gen)
{
	QTreeWidgetItem *item=nullptr;
	QString text=PgModelerUiNs::formatMessage(msg);
	QPixmap ico;

	progress_lbl->setText(text);
	progress_pb->setValue(progress);

	if(obj_type!=ObjectType::BaseObject)
		ico=QPixmap(PgModelerUiNs::getIconPath(obj_type));
	else if(!cmd.isEmpty())
		ico=QPixmap(PgModelerUiNs::getIconPath("codigosql"));
	else
		ico=QPixmap(PgModelerUiNs::getIconPath("msgbox_info"));

	ico_lbl->setPixmap(ico);

	if(!is_code_gen)
	{
		item=PgModelerUiNs::createOutputTreeItem(output_trw, text, ico, nullptr, false);

		if(!cmd.isEmpty())
			PgModelerUiNs::createOutputTreeItem(output_trw, cmd, QPixmap(), item, false);
	}
}

void ModelExportForm::exportModel(void)
{
	try
	{
		output_trw->clear();
		settings_tbw->setTabEnabled(1, true);
		settings_tbw->setCurrentIndex(1);
		enableExportModes(false);
		cancel_btn->setEnabled(true);

		//Export to png
		if(export_to_img_rb->isChecked())
		{
			viewp=new QGraphicsView(model->scene);

			if(png_rb->isChecked())
				export_hlp.setExportToPNGParams(model->scene, viewp, image_edt->text(),
																				zoom_cmb->itemData(zoom_cmb->currentIndex()).toDouble(),
																				show_grid_chk->isChecked(), show_delim_chk->isChecked(),
																				page_by_page_chk->isChecked());
			else
				export_hlp.setExportToSVGParams(model->scene, image_edt->text(),
																				show_grid_chk->isChecked(),
																				show_delim_chk->isChecked());

			export_thread->start();
		}
		else
		{
			progress_lbl->setText(trUtf8("Initializing model export..."));

			//Exporting to sql file
			if(export_to_file_rb->isChecked())
			{
				progress_lbl->setText(trUtf8("Saving file '%1'").arg(file_edt->text()));
				export_hlp.setExportToSQLParams(model->db_model, file_edt->text(), pgsqlvers_cmb->currentText());
				export_thread->start();
			}
			//Exporting directly to DBMS
			else
			{
				QString version;
				Connection *conn=reinterpret_cast<Connection *>(connections_cmb->itemData(connections_cmb->currentIndex()).value<void *>());

				//If the user chose a specific version
				if(pgsqlvers1_cmb->isEnabled())
					version=pgsqlvers1_cmb->currentText();

				export_hlp.setExportToDBMSParams(model->db_model, conn, version, ignore_dup_chk->isChecked(),
												 drop_chk->isChecked() && drop_db_rb->isChecked(),
												 drop_chk->isChecked() && drop_objs_rb->isChecked());

				if(ignore_error_codes_chk->isChecked())
					export_hlp.setIgnoredErrors(error_codes_edt->text().simplified().split(' '));

				export_thread->start();
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

void ModelExportForm::selectExportMode(void)
{
	QList<QRadioButton *> radios={ export_to_dbms_rb, export_to_img_rb, export_to_file_rb};
	QWidgetList wgts={ export_to_dbms_wgt, export_to_img_wgt, export_to_file_wgt };
	int i=0;

	for(QRadioButton *rb : radios)
	{
		rb->blockSignals(true);
		rb->setChecked((!sender() && rb==export_to_dbms_rb) || (sender()==rb));
		wgts[i++]->setEnabled(rb->isChecked());
		rb->blockSignals(false);
	}

	pgsqlvers1_cmb->setEnabled(export_to_dbms_rb->isChecked() && pgsqlvers_chk->isChecked());
	export_btn->setEnabled((export_to_dbms_rb->isChecked() && connections_cmb->currentIndex() > 0 && connections_cmb->currentIndex()!=connections_cmb->count()-1) ||
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
		file_dlg.setNameFilter(trUtf8("SQL script (*.sql);;All files (*.*)"));
		file_dlg.selectFile(model->getDatabaseModel()->getName() + QString(".sql"));
	}
	else
	{
		if(png_rb->isChecked())
		{
			file_dlg.setNameFilter(trUtf8("Portable Network Graphics (*.png);;All files (*.*)"));
			file_dlg.selectFile(model->getDatabaseModel()->getName() + QString(".png"));
		}
		else
		{
			file_dlg.setNameFilter(trUtf8("Scalable Vector Graphics (*.svg);;All files (*.*)"));
			file_dlg.selectFile(model->getDatabaseModel()->getName() + QString(".svg"));
		}
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
	QTreeWidgetItem *item=PgModelerUiNs::createOutputTreeItem(output_trw, PgModelerUiNs::formatMessage(e.getErrorMessage()),
																														QPixmap(PgModelerUiNs::getIconPath("msgbox_erro")), nullptr, false, true);

	PgModelerUiNs::createExceptionsTree(output_trw, e, item);

	ico_lbl->setPixmap(QPixmap(PgModelerUiNs::getIconPath("msgbox_erro")));
	finishExport(trUtf8("Exporting process aborted!"));

	throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
}

void ModelExportForm::cancelExport(void)
{
	export_hlp.cancelExport();
	cancel_btn->setEnabled(false);
}

void ModelExportForm::handleExportCanceled(void)
{
	QPixmap ico=QPixmap(PgModelerUiNs::getIconPath("msgbox_alerta"));
	QString msg=trUtf8("Exporting process canceled by user!");

	finishExport(msg);
	ico_lbl->setPixmap(ico);
	PgModelerUiNs::createOutputTreeItem(output_trw, msg, ico);
}

void ModelExportForm::handleExportFinished(void)
{
	QPixmap ico=QPixmap(PgModelerUiNs::getIconPath("msgbox_info"));
	QString msg=trUtf8("Exporting process sucessfuly ended!");

	finishExport(msg);
	ico_lbl->setPixmap(ico);
	PgModelerUiNs::createOutputTreeItem(output_trw, msg, ico);
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

	if(viewp)
	{
		export_thread->wait();
		delete(viewp);
		viewp=nullptr;
	}
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

void ModelExportForm::editConnections(void)
{
	try
	{
		if(connections_cmb->currentIndex()==connections_cmb->count()-1 &&
				ConnectionsConfigWidget::openConnectionsConfiguration(connections_cmb, true))
			emit s_connectionsUpdateRequest();
	}
	catch(Exception &e)
	{
		Messagebox msg_box;
		msg_box.show(e);
	}

	export_btn->setEnabled(export_to_dbms_rb->isChecked() &&
						   connections_cmb->currentIndex() > 0 &&
						   connections_cmb->currentIndex()!=connections_cmb->count()-1);
}
