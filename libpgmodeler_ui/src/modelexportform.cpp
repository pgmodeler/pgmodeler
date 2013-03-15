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

extern TaskProgressWidget *task_prog_wgt;
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
		QString host;

		this->model=model;

		//Get the current connections configured on the connections widget
		dynamic_cast<ConnectionsConfigWidget *>(configuration_form->getConfigurationWidget(ConfigurationForm::CONNECTIONS_CONF_WGT))->getConnections(connections);

		connections_cmb->clear();
		itr=connections.begin();

		while(itr!=connections.end())
		{
			host=(itr->second)->getConnectionParam(DBConnection::PARAM_SERVER_FQDN);

			if(host.isEmpty())
				host=(itr->second)->getConnectionParam(DBConnection::PARAM_SERVER_IP);

			connections_cmb->addItem(itr->first + QString(" (%1)").arg(host),
														QVariant::fromValue<void *>(itr->second));
			itr++;
		}

		this->hideExportProgress();
		QDialog::show();
	}
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
		this->resize(this->maximumSize());

		progress_pb->setValue(0);
		ln2_frm->setVisible(true);
		progress_lbl->setVisible(true);
		progress_pb->setVisible(true);
		ico_lbl->setVisible(false);

		if(export_to_img_rb->isChecked())
		{
			QPixmap pix;
			QRectF ret=model->scene->itemsBoundingRect();
			bool exibir_grade, exibir_lim, alin_objs;

			ObjectsScene::getGridOptions(exibir_grade, alin_objs, exibir_lim);
			ObjectsScene::setGridOptions(show_grid_chk->isChecked(), false, show_delim_chk->isChecked());

			pix=QPixmap(ret.size().toSize());
			QPainter p(&pix);
			model->scene->clearSelection();
			model->scene->update();
			model->scene->render(&p, QRectF(QPointF(0,0), pix.size()), ret);

			ObjectsScene::setGridOptions(exibir_grade, alin_objs, exibir_lim);
			model->scene->update();

			if(!pix.save(image_edt->text()))
				throw Exception(Exception::getErrorMessage(ERR_FILE_NOT_WRITTEN).arg(Utf8String::create(image_edt->text())),
												ERR_FILE_NOT_WRITTEN,__PRETTY_FUNCTION__,__FILE__,__LINE__);



		}
		else
		{
			int type_id;
			QString  version, sql_buf, sql_cmd;
			DBConnection *conn=NULL, new_db_conn;
			unsigned i, count;
			bool db_created=false;
			int objs_idx[]={-1, -1};
			ObjectType types[]={OBJ_ROLE, OBJ_TABLESPACE};
			BaseObject *object=NULL;
			vector<Exception> errors;

			/* Error codes treated in this method
					42P04 	duplicate_database
					42723 	duplicate_function
					42P06 	duplicate_schema
					42P07 	duplicate_table
					42710 	duplicate_object

				 Reference:
					http://www.postgresql.org/docs/current/static/errcodes-appendix.html*/
			QString error_codes[]={"42P04", "42723", "42P06", "42P07", "42710"};
			vector<QString> err_codes_vect;

			try
			{
				for(i=0; i < 5; i++)
					err_codes_vect.push_back(error_codes[i]);

				task_prog_wgt->setWindowTitle(trUtf8("Generating source code..."));
				task_prog_wgt->show();
				connect(this->model->db_model, SIGNAL(s_objetoCarregado(int,QString,unsigned)),
								task_prog_wgt, SLOT(updateProgress(int,QString,unsigned)));

				progress_lbl->setText(trUtf8("Initializing model export..."));
				progress_lbl->repaint();

				if(export_to_file_rb->isChecked())
				{
					//Sets the SQL version for the output code
					SchemaParser::setPgSQLVersion(pgsqlvers_cmb->currentText());
					progress_lbl->setText(trUtf8("Saving file '%1'").arg(file_edt->text()));
					model->db_model->saveModel(file_edt->text(), SchemaParser::SQL_DEFINITION);
					progress_pb->setValue(25);
				}
				//Exporting directly to DBMS
				else
				{
					//Get the selected connection to database
					conn=reinterpret_cast<DBConnection *>(connections_cmb->itemData(connections_cmb->currentIndex()).value<void *>());
					conn->connect();

					//Retrive the DBMS version in order to generate the correct code
					version=(conn->getDBMSVersion()).mid(0,3);

					//Overriding the DBMS version case the checkbox is marked
					if(pgsqlvers_chk->isChecked())
						SchemaParser::setPgSQLVersion(pgsqlvers1_cmb->currentText());
					else
						SchemaParser::setPgSQLVersion(version);


					//Creates the roles and tablespaces separately from the other objects
					for(type_id=0; type_id < 2; type_id++)
					{
						count=model->db_model->getObjectCount(types[type_id]);
						for(i=0; i < count; i++)
						{
							object=model->db_model->getObject(i, types[type_id]);
							progress_lbl->setText(trUtf8("Creating object '%1' (%2)...").arg(Utf8String::create(object->getName())).arg(object->getTypeName()));
							progress_lbl->repaint();

							try
							{
								conn->executeDDLCommand(object->getCodeDefinition(SchemaParser::SQL_DEFINITION));
							}
							catch(Exception &e)
							{
								if(!ignorar_dup_chk->isChecked() ||
									 (ignorar_dup_chk->isChecked() &&
										std::find(err_codes_vect.begin(), err_codes_vect.end(), e.getExtraInfo())==err_codes_vect.end()))
									throw Exception(e.getErrorMessage(),
																	e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
								else
									errors.push_back(e);
							}

							progress_pb->setValue((10 * (type_id+1)) + ((i/static_cast<float>(count)) * 10));
							objs_idx[type_id]++;
						}
					}

					//Creating the database on the DBMS
					progress_lbl->setText(trUtf8("Creating database '%1'...").arg(Utf8String::create(model->db_model->getName())));
					progress_lbl->repaint();

					try
					{
						conn->executeDDLCommand(model->db_model->__getCodeDefinition(SchemaParser::SQL_DEFINITION));
						db_created=true;
					}
					catch(Exception &e)
					{
						if(!ignorar_dup_chk->isChecked() ||
							 (ignorar_dup_chk->isChecked() &&
								std::find(err_codes_vect.begin(), err_codes_vect.end(), e.getExtraInfo())==err_codes_vect.end()))
							throw Exception(e.getErrorMessage(),
															e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
						else
							errors.push_back(e);
					}

					progress_pb->setValue(30);

					new_db_conn=(*conn);
					new_db_conn.setConnectionParam(DBConnection::PARAM_DB_NAME, model->db_model->getName());
					progress_lbl->setText(trUtf8("Connecting to database '%1'...").arg(Utf8String::create(model->db_model->getName())));
					progress_lbl->repaint();
					new_db_conn.connect();
					progress_pb->setValue(50);

					//Creating the other object types
					progress_lbl->setText(trUtf8("Creating objects on database '%1'...").arg(Utf8String::create(model->db_model->getName())));
					progress_lbl->repaint();

					//Gera o código SQL de todo o banco
					sql_buf=model->db_model->getCodeDefinition(SchemaParser::SQL_DEFINITION, false);

					/* Extract each SQL command from the buffer and execute them separately. This is done
					 to permit the user, in case of error, identify what object is wrongly configured. */
					i=0;
					count=sql_buf.size();

					while(i < count)
					{
						try
						{
							sql_cmd.clear();

							//Extract the characters until the end of buffer or until the end of ddl-end token
							while(i < count)
							{
								if(sql_cmd.indexOf(ParsersAttributes::DDL_END_TOKEN) >= 0)
								{
									sql_cmd.remove(ParsersAttributes::DDL_END_TOKEN);
									sql_cmd.simplified();
									break;
								}
								else
									sql_cmd+=sql_buf.at(i);
								i++;
							}

							//Executes the extracted command
							if(!sql_cmd.isEmpty())
							{
								i++;
								new_db_conn.executeDDLCommand(sql_cmd);
							}

							progress_pb->setValue(50 + ((i/static_cast<float>(count)) * 10));
						}
						catch(Exception &e)
						{
							if(!ignorar_dup_chk->isChecked() ||
								 (ignorar_dup_chk->isChecked() &&
									std::find(err_codes_vect.begin(), err_codes_vect.end(), e.getExtraInfo())==err_codes_vect.end()))
								throw Exception(Exception::getErrorMessage(ERR_EXPORT_FAILURE).arg(Utf8String::create(sql_cmd)),
																ERR_EXPORT_FAILURE,__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
							else
								errors.push_back(e);
						}
					}
				}

				task_prog_wgt->close();
				disconnect(this->model->db_model, NULL, task_prog_wgt, NULL);
			}
			catch(Exception &e)
			{
				QString drop_cmd=QString("DROP %1 %2;");

				task_prog_wgt->close();
				disconnect(this->model->db_model, NULL, task_prog_wgt, NULL);

				//In case of error during the export all created object are removed
				if(db_created || objs_idx[0] >= 0 || objs_idx[1] >= 0)
				{
					if(new_db_conn.isStablished())
						new_db_conn.close();

					if(db_created)
						conn->executeDDLCommand(drop_cmd
																			 .arg(model->db_model->getSQLName())
																			 .arg(model->db_model->getName(true)));

					for(type_id=1; type_id >=0; type_id--)
					{
						while(objs_idx[type_id] >= 0)
						{
							object=model->db_model->getObject(objs_idx[type_id], types[type_id]);

							try
							{
								conn->executeDDLCommand(drop_cmd
																					 .arg(object->getSQLName())
																					 .arg(object->getName(true)));
							}
							catch(Exception &e)
							{}

							objs_idx[type_id]--;
						}
					}
				}

				if(errors.empty())
					throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
				else
				{
					errors.push_back(e);
					throw Exception(e.getErrorMessage(),__PRETTY_FUNCTION__,__FILE__,__LINE__, errors);
				}
			}
		}

		progress_pb->setValue(100);
		progress_lbl->setText(trUtf8("Exporting process sucessfuly ended!"));
		progress_lbl->repaint();
		ico_lbl->setPixmap(QPixmap(QString(":/icones/icones/msgbox_info.png")));
		ico_lbl->setVisible(true);

		QTimer::singleShot(5000, this, SLOT(hideExportProgress(void)));
	}
	catch(Exception &e)
	{
		MessageBox msg_box;

		progress_lbl->setText(trUtf8("Error on export!"));
		progress_lbl->repaint();
		ico_lbl->setPixmap(QPixmap(QString(":/icones/icones/msgbox_erro.png")));
		ico_lbl->setVisible(true);

		QTimer::singleShot(5000, this, SLOT(hideExportProgress(void)));
		msg_box.show(e);
	}
}

void ModelExportForm::hideExportProgress(void)
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

