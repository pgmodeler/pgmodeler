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

#include "modelfixform.h"
#include "configurationform.h"
#include <iostream>
#include "pgmodeleruins.h"

const QString ModelFixForm::PgModelerCli=QString("pgmodeler-cli");

ModelFixForm::ModelFixForm(QWidget *parent, Qt::WindowFlags f) : QDialog(parent, f)
{
	map<QString, attribs_map> confs=GeneralConfigWidget::getConfigurationParams();

	setupUi(this);
	hideEvent(nullptr);

	PgModelerUiNs::configureWidgetFont(invalid_cli_lbl, PgModelerUiNs::MediumFontFactor);
	PgModelerUiNs::configureWidgetFont(message_lbl, PgModelerUiNs::MediumFontFactor);
	PgModelerUiNs::configureWidgetFont(not_found_lbl, PgModelerUiNs::MediumFontFactor);

	//Configuring font style for output widget
	if(!confs[Attributes::Configuration][Attributes::CodeFont].isEmpty())
	{
		double size=confs[Attributes::Configuration][Attributes::CodeFontSize].toDouble();
		if(size < 5.0f) size=5.0f;

		output_txt->setFontFamily(confs[Attributes::Configuration][Attributes::CodeFont]);
		output_txt->setFontPointSize(size);
	}

	connect(&pgmodeler_cli_proc, SIGNAL(readyReadStandardOutput()), this, SLOT(updateOutput()));
	connect(&pgmodeler_cli_proc, SIGNAL(readyReadStandardError()), this, SLOT(updateOutput()));
	connect(&pgmodeler_cli_proc, SIGNAL(finished(int)), this, SLOT(handleProcessFinish(int)));
	connect(fix_btn, SIGNAL(clicked()), this, SLOT(fixModel()));
	connect(sel_cli_exe_tb, SIGNAL(clicked()), this, SLOT(selectFile()));
	connect(sel_in_file_tb, SIGNAL(clicked()), this, SLOT(selectFile()));
	connect(sel_out_file_tb, SIGNAL(clicked()), this, SLOT(selectFile()));
	connect(input_file_edt, SIGNAL(textChanged(QString)), this, SLOT(enableFix()));
	connect(output_file_edt, SIGNAL(textChanged(QString)), this, SLOT(enableFix()));
	connect(pgmodeler_cli_edt, SIGNAL(textChanged(QString)), this, SLOT(enableFix()));
	connect(close_btn, SIGNAL(clicked()), this, SLOT(reject()));
}

void ModelFixForm::hideEvent(QHideEvent *)
{
	message_frm->setVisible(false);
	pgmodeler_cli_lbl->setVisible(false);
	pgmodeler_cli_edt->setVisible(false);
	sel_cli_exe_tb->setVisible(false);
	invalid_cli_lbl->setVisible(false);
	input_file_edt->clear();
	output_file_edt->clear();
	output_txt->setPlainText(trUtf8("Waiting process to start..."));
	load_model_chk->setChecked(true);
}

int ModelFixForm::exec(void)
{
	QFileInfo fi(GlobalAttributes::PgModelerCLIPath);

	//Show an warning if the cli command doesn't exists
	if(!fi.exists())
	{
		not_found_lbl->setText(trUtf8("Could not locate <strong>%1</strong> tool on <strong>%2</strong>. The fix process can't continue! Please check pgModeler installation or try to manually specify the command below.")
							   .arg(PgModelerCli).arg(fi.absoluteDir().absolutePath()));
		message_frm->setVisible(true);
		pgmodeler_cli_lbl->setVisible(true);
		pgmodeler_cli_edt->setVisible(true);
		sel_cli_exe_tb->setVisible(true);
	}
	else
		pgmodeler_cli_edt->setText(GlobalAttributes::PgModelerCLIPath);

	return(QDialog::exec());
}

void ModelFixForm::enableFix(void)
{
	if(!pgmodeler_cli_edt->text().isEmpty())
	{
		QFileInfo fi(pgmodeler_cli_edt->text());
		bool visible=!fi.exists() || fi.baseName()!=PgModelerCli;

		invalid_cli_lbl->setVisible(visible);
		message_frm->setVisible(visible);
	}
	else
	{
		invalid_cli_lbl->setVisible(false);
		message_frm->setVisible(false);
	}

	fix_btn->setEnabled(!input_file_edt->text().isEmpty() &&
						!output_file_edt->text().isEmpty() &&
						!pgmodeler_cli_edt->text().isEmpty() &&
						!invalid_cli_lbl->isVisible());
}

void ModelFixForm::fixModel(void)
{
	QString cmd=QString("\"%1\"");

#ifdef Q_OS_MAC
	cmd+=QString(" pgmodeler-cli");
#endif

	cmd+=QString(" --fix-model --fix-tries=%2 --input=\"%3\" --output=\"%4\"");
	cmd=cmd.arg(pgmodeler_cli_edt->text())
		.arg(fix_tries_sb->value())
		.arg(input_file_edt->text())
		.arg(output_file_edt->text());

	output_txt->clear();
	pgmodeler_cli_proc.blockSignals(false);
	pgmodeler_cli_proc.start(cmd);
}

void ModelFixForm::selectFile(void)
{
	QObject *sender_obj=sender();
	QFileDialog file_dlg;
	QLineEdit *txt=nullptr;

	if(sender_obj==sel_cli_exe_tb)
	{
		QString cli_cmd=PgModelerCli;
		txt=pgmodeler_cli_edt;

#ifdef Q_OS_WIN
		cli_cmd+=QString(".exe");
#endif

		file_dlg.selectFile(cli_cmd);
		file_dlg.setFileMode(QFileDialog::ExistingFile);
		file_dlg.setNameFilter(trUtf8("pgModeler command line tool (%1)").arg(cli_cmd));
		file_dlg.setWindowTitle(QString("Browse pgmodeler-cli command..."));
	}
	else
	{
		if(sender_obj==sel_in_file_tb)
			txt=input_file_edt;
		else
			txt=output_file_edt;

		file_dlg.setWindowTitle(QString("Select model file..."));
	}

	file_dlg.exec();

	if(file_dlg.result()==QDialog::Accepted && !file_dlg.selectedFiles().isEmpty())
		txt->setText(file_dlg.selectedFiles().at(0));
}

void ModelFixForm::updateOutput(void)
{
	QTextCursor cursor;
	QString txt=output_txt->toPlainText();

	//Append both stdout and stderr
	txt.append(pgmodeler_cli_proc.readAllStandardOutput());
	txt.append(pgmodeler_cli_proc.readAllStandardError());
	output_txt->setPlainText(txt);

	//Moving the output to the last line
	cursor=output_txt->textCursor();
	cursor.movePosition(QTextCursor::End);
	output_txt->setTextCursor(cursor);
}

void ModelFixForm::handleProcessFinish(int res)
{
	/* If the model as sucessfully fixed and user
	 requested the loading */
	if(res==0 && load_model_chk->isChecked())
	{
		//Emit a signal indicating the file to be loaded
		emit s_modelLoadRequested(output_file_edt->text());
		this->close();
	}

	pgmodeler_cli_proc.blockSignals(true);
}

