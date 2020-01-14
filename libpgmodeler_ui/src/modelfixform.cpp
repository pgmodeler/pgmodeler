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

const QString ModelFixForm::PgModelerCli("pgmodeler-cli");

ModelFixForm::ModelFixForm(QWidget *parent, Qt::WindowFlags f) : QDialog(parent, f)
{
	map<QString, attribs_map> confs=GeneralConfigWidget::getConfigurationParams();

	setupUi(this);

	cli_fsel_wgt=new FileSelectorWidget(fsel_cli, this, new FileSelector{FileSelector::In,FileSelector::Cli});
	in_fsel_wgt=new FileSelectorWidget(fsel_i, this, new FileSelector{FileSelector::In,FileSelector::DatabaseModel});
	out_fsel_wgt=new FileSelectorWidget(fsel_o, this, new FileSelector{FileSelector::Out,FileSelector::DatabaseModel});

	hideEvent(nullptr);

	PgModelerUiNs::configureWidgetFont(invalid_cli_lbl, PgModelerUiNs::MediumFontFactor);
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
	connect(cli_fsel_wgt, SIGNAL(s_fileSelected(QString)), this, SLOT(enableFix()));
	connect(in_fsel_wgt, SIGNAL(s_fileSelected(QString)), this, SLOT(enableFix()));
	connect(out_fsel_wgt, SIGNAL(s_fileSelected(QString)), this, SLOT(enableFix()));
	connect(close_btn, SIGNAL(clicked()), this, SLOT(reject()));
}

void ModelFixForm::hideEvent(QHideEvent *)
{
	message_frm->setVisible(false);
	pgmodeler_cli_lbl->setVisible(false);
	fsel_cli->setVisible(false);
	invalid_cli_lbl->setVisible(false);
	output_txt->setPlainText(tr("Waiting process to start..."));
	load_model_chk->setChecked(true);
}

int ModelFixForm::exec()
{
	QFileInfo fi(GlobalAttributes::getPgModelerCLIPath());

	//Show a warning if the cli command doesn't exist
	if(!fi.exists())
	{
		not_found_lbl->setText(tr("Could not locate <strong>%1</strong> tool on <strong>%2</strong>. The fix process can't continue! Please check pgModeler installation or try to manually specify the command below.")
							   .arg(PgModelerCli).arg(fi.absoluteDir().absolutePath()));
		message_frm->setVisible(true);
		pgmodeler_cli_lbl->setVisible(true);
		fsel_cli->setVisible(true);
	}
	else
		cli_fsel_wgt->setText(GlobalAttributes::getPgModelerCLIPath());

	return QDialog::exec();
}

void ModelFixForm::enableFix()
{
	if(!cli_fsel_wgt->text().isEmpty())
	{
		QFileInfo fi(cli_fsel_wgt->text());
		bool visible=!fi.exists() || fi.baseName()!=PgModelerCli;

		invalid_cli_lbl->setVisible(visible);
		message_frm->setVisible(visible);
	}
	else
	{
		invalid_cli_lbl->setVisible(false);
		message_frm->setVisible(false);
	}

	fix_btn->setEnabled(!cli_fsel_wgt->text().isEmpty() &&
						!in_fsel_wgt->text().isEmpty() &&
						!out_fsel_wgt->text().isEmpty() &&
						!invalid_cli_lbl->isVisible());
}

void ModelFixForm::fixModel()
{
	QString cmd=QString("\"%1\"");

#ifdef Q_OS_MAC
	cmd+=QString(" pgmodeler-cli");
#endif

	cmd+=QString(" --fix-model --fix-tries=%2 --input=\"%3\" --output=\"%4\"");
	cmd=cmd.arg(cli_fsel_wgt->text())
		.arg(fix_tries_sb->value())
		.arg(in_fsel_wgt->text())
		.arg(out_fsel_wgt->text());

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
		emit s_modelLoadRequested(out_fsel_wgt->text());
		this->close();
	}

	pgmodeler_cli_proc.blockSignals(true);
}

