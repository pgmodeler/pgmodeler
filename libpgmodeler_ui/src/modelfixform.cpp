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

#include "modelfixform.h"
#include "configurationform.h"
#include <iostream>
#include "pgmodeleruins.h"

const QString ModelFixForm::PgModelerCli("pgmodeler-cli");

ModelFixForm::ModelFixForm(QWidget *parent, Qt::WindowFlags f) : QDialog(parent, f)
{
	map<QString, attribs_map> confs=GeneralConfigWidget::getConfigurationParams();

	setupUi(this);

	input_file_sel = new FileSelectorWidget(this);
	input_file_sel->setFileMode(QFileDialog::ExistingFile);
	input_file_sel->setNameFilters({tr("Database model (*.dbm)"), tr("All files (*.*)")});
	input_file_sel->setAcceptMode(QFileDialog::AcceptOpen);
	input_file_sel->setAllowFilenameInput(true);
	input_file_sel->setWindowTitle(tr("Select input file"));
	model_fix_grid->addWidget(input_file_sel, 2, 2);

	output_file_sel = new FileSelectorWidget(this);
	output_file_sel->setFileMode(QFileDialog::AnyFile);
	output_file_sel->setNameFilters({tr("Database model (*.dbm)"), tr("All files (*.*)")});
	output_file_sel->setDefaultSuffix("dbm");
	output_file_sel->setAcceptMode(QFileDialog::AcceptSave);
	output_file_sel->setAllowFilenameInput(true);
	input_file_sel->setWindowTitle(tr("Select output file"));
	model_fix_grid->addWidget(output_file_sel, 3, 2);

	pgmodeler_cli_sel = new FileSelectorWidget(this);
	pgmodeler_cli_sel->setFileMode(QFileDialog::ExistingFile);
	pgmodeler_cli_sel->setNameFilters({tr("pgModeler command line tool (%1)").arg(PgModelerCli)});
	pgmodeler_cli_sel->setAcceptMode(QFileDialog::AcceptOpen);
	pgmodeler_cli_sel->setAllowFilenameInput(true);
	pgmodeler_cli_sel->setWindowTitle(tr("Select pgmodeler-cli executable"));
	pgmodeler_cli_sel->setVisible(false);
	model_fix_grid->addWidget(pgmodeler_cli_sel, 1, 2);

	PgModelerUiNs::configureWidgetFont(message_lbl, PgModelerUiNs::MediumFontFactor);
	PgModelerUiNs::configureWidgetFont(not_found_lbl, PgModelerUiNs::MediumFontFactor);

	//Configuring font style for output widget
	if(!confs[Attributes::Configuration][Attributes::CodeFont].isEmpty())
	{
		double size=confs[Attributes::Configuration][Attributes::CodeFontSize].toDouble();
		if(size < 5.0) size=5.0;

		output_txt->setFontFamily(confs[Attributes::Configuration][Attributes::CodeFont]);
		output_txt->setFontPointSize(size);
	}

	connect(&pgmodeler_cli_proc, SIGNAL(readyReadStandardOutput()), this, SLOT(updateOutput()));
	connect(&pgmodeler_cli_proc, SIGNAL(readyReadStandardError()), this, SLOT(updateOutput()));
	connect(&pgmodeler_cli_proc, SIGNAL(finished(int)), this, SLOT(handleProcessFinish(int)));
	connect(fix_btn, SIGNAL(clicked()), this, SLOT(fixModel()));
	connect(input_file_sel, SIGNAL(s_selectorChanged(bool)), this, SLOT(enableFix()));
	connect(output_file_sel, SIGNAL(s_selectorChanged(bool)), this, SLOT(enableFix()));
	connect(pgmodeler_cli_sel, SIGNAL(s_selectorChanged(bool)), this, SLOT(enableFix()));
	connect(close_btn, SIGNAL(clicked()), this, SLOT(reject()));

	hideEvent(nullptr);
}

void ModelFixForm::hideEvent(QHideEvent *)
{
	message_frm->setVisible(false);
	pgmodeler_cli_lbl->setVisible(false);
	pgmodeler_cli_sel->setVisible(false);
	input_file_sel->clearSelector();
	output_file_sel->clearSelector();
	output_txt->setPlainText(tr("Waiting process to start..."));
	load_model_chk->setChecked(true);
}

int ModelFixForm::exec()
{
	QFileInfo fi(GlobalAttributes::getPgModelerCLIPath());

	//Show an warning if the cli command doesn't exists
	if(!fi.exists())
	{
		not_found_lbl->setText(tr("Could not locate <strong>%1</strong> tool on <strong>%2</strong>. The fix process can't continue! Please check pgModeler installation or try to manually specify the command below.")
							   .arg(PgModelerCli).arg(fi.absoluteDir().absolutePath()));
		message_frm->setVisible(true);
		pgmodeler_cli_lbl->setVisible(true);
		pgmodeler_cli_sel->setVisible(true);
	}
	else
		pgmodeler_cli_sel->setSelectedFile(GlobalAttributes::getPgModelerCLIPath());

	return QDialog::exec();
}

void ModelFixForm::enableFix()
{
	if(!pgmodeler_cli_sel->getSelectedFile().isEmpty())
	{
		QFileInfo fi(pgmodeler_cli_sel->getSelectedFile());
		bool invalid_cli = !fi.exists() || fi.baseName() != PgModelerCli;

		if(invalid_cli)
			pgmodeler_cli_sel->setCustomWarning(tr("The specified file is not the pgModeler command line tool (pgmodeler-cli)."));
		else
			pgmodeler_cli_sel->clearCustomWarning();

		message_frm->setVisible(invalid_cli);
	}
	else
	{
		pgmodeler_cli_sel->clearCustomWarning();
		message_frm->setVisible(false);
	}

	fix_btn->setEnabled(!input_file_sel->hasWarning() && !input_file_sel->getSelectedFile().isEmpty() &&
											!output_file_sel->hasWarning() && !output_file_sel->getSelectedFile().isEmpty() &&
											!pgmodeler_cli_sel->hasWarning() && !pgmodeler_cli_sel->getSelectedFile().isEmpty());
}

void ModelFixForm::fixModel()
{
	QString cmd=QString("\"%1\"");

#ifdef Q_OS_MAC
	cmd+=QString(" pgmodeler-cli");
#endif

	cmd+=QString(" --fix-model --fix-tries=%2 --input=\"%3\" --output=\"%4\"");
	cmd=cmd.arg(pgmodeler_cli_sel->getSelectedFile())
		.arg(fix_tries_sb->value())
		.arg(input_file_sel->getSelectedFile())
		.arg(output_file_sel->getSelectedFile());

	output_txt->clear();
	pgmodeler_cli_proc.blockSignals(false);
	pgmodeler_cli_proc.start(cmd);
}

void ModelFixForm::updateOutput()
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
		emit s_modelLoadRequested(output_file_sel->getSelectedFile());
		this->close();
	}

	pgmodeler_cli_proc.blockSignals(true);
}

