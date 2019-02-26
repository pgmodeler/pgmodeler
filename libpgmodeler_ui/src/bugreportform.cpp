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

#include "bugreportform.h"
#include "exception.h"
#include "globalattributes.h"
#include "messagebox.h"
#include "pgmodeleruins.h"

BugReportForm::BugReportForm(QWidget *parent, Qt::WindowFlags f) : QDialog(parent, f)
{
	setupUi(this);
	setWindowFlags(Qt::Dialog |
				   Qt::WindowTitleHint |
				   Qt::WindowMinMaxButtonsHint |
				   Qt::WindowCloseButtonHint);

	PgModelerUiNs::configureWidgetFont(hint_lbl, PgModelerUiNs::MediumFontFactor);

	connect(cancel_btn, SIGNAL(clicked(void)), this, SLOT(close(void)));
	connect(create_btn, SIGNAL(clicked(void)), this, SLOT(generateReport(void)));
	connect(attach_mod_chk, SIGNAL(toggled(bool)), attach_tb, SLOT(setEnabled(bool)));
	connect(attach_tb, SIGNAL(clicked()), this, SLOT(attachModel()));
	connect(output_tb, SIGNAL(clicked()), this, SLOT(selectOutput()));
	connect(actions_txt, SIGNAL(textChanged()), this, SLOT(enableGeneration()));
	connect(output_edt, SIGNAL(textChanged(QString)), this, SLOT(enableGeneration()));

	output_edt->setText(QFileInfo(GlobalAttributes::TemporaryDir).absoluteFilePath());

	//Installs a syntax highlighter on model_txt widget
	hl_model_txt=new SyntaxHighlighter(model_txt);
	hl_model_txt->loadConfiguration(GlobalAttributes::XMLHighlightConfPath);

	QDir tmp_dir=QDir(GlobalAttributes::TemporaryDir, QString("*.dbm"), QDir::Name, QDir::Files | QDir::NoDotAndDotDot);
	tmp_dir.setSorting(QDir::Time);
	QStringList list=tmp_dir.entryList();

	if(!list.isEmpty())
	{
		QFile input;

		//Opens the last modified model file showing it on the proper widget
		input.setFileName(GlobalAttributes::TemporaryDir +
						  GlobalAttributes::DirSeparator + list[0]);

		input.open(QFile::ReadOnly);
		model_txt->setPlainText(QString(input.readAll()));
		input.close();
	}
}

QByteArray BugReportForm::generateReportBuffer(void)
{
	QByteArray buf;

	buf.append(actions_txt->toPlainText().toUtf8());
	buf.append(CharDelimiter);

	if(attach_mod_chk->isChecked())
		buf.append(model_txt->toPlainText().toUtf8());
	buf.append(CharDelimiter);

	return(buf);
}

void BugReportForm::generateReport(void)
{ 
	generateReport(generateReportBuffer());
	this->accept();
}

void BugReportForm::enableGeneration(void)
{
	create_btn->setEnabled(!output_edt->text().isEmpty() && !actions_txt->toPlainText().isEmpty());
}

void BugReportForm::generateReport(const QByteArray &buf)
{
	Messagebox msgbox;
	QFile output;
	QString filename=QFileInfo(QString(output_edt->text() +
									   GlobalAttributes::DirSeparator +
									   GlobalAttributes::BugReportFile)
							   .arg(QDateTime::currentDateTime().toString(QString("_yyyyMMdd_hhmm")))).absoluteFilePath();

	//Opens the file for writting
	output.setFileName(filename);
	output.open(QFile::WriteOnly);

	if(!output.isOpen())
		msgbox.show(Exception::getErrorMessage(ErrorCode::FileDirectoryNotWritten).arg(filename), Messagebox::ErrorIcon);
	else
	{
		QByteArray comp_buf;

		//Compress the buffer (using zlib) in a compression rate at 8
		comp_buf=qCompress(buf, 8);

		//Saves the buffer
		output.write(comp_buf.data(), comp_buf.size());
		output.close();

		msgbox.show(trUtf8("Bug report successfuly generated! Please, send the file <strong>%1</strong> to <em>%2</em> in order be analyzed. Thank you for the collaboration!").arg(filename).arg(GlobalAttributes::BugReportEmail),
					Messagebox::InfoIcon);
	}
}

void BugReportForm::attachModel(void)
{
	QFileDialog file_dlg;

	try
	{
		file_dlg.setDefaultSuffix(QString("dbm"));
		file_dlg.setWindowTitle(trUtf8("Load model"));
		file_dlg.setNameFilter(trUtf8("Database model (*.dbm);;All files (*.*)"));
		file_dlg.setFileMode(QFileDialog::AnyFile);
		file_dlg.setModal(true);

		if(file_dlg.exec()==QFileDialog::Accepted)
		{
			QFile input(file_dlg.selectedFiles().at(0));
			QByteArray buf;

			input.open(QFile::ReadOnly);

			if(!input.isOpen())
				throw Exception(Exception::getErrorMessage(ErrorCode::FileDirectoryNotAccessed).arg(file_dlg.selectedFiles().at(0)),
												ErrorCode::FileDirectoryNotAccessed,__PRETTY_FUNCTION__,__FILE__,__LINE__);

			buf=input.readAll();
			model_txt->setPlainText(QString(buf));
			input.close();
		}
	}
	catch(Exception &e)
	{
		Messagebox msgbox;
		msgbox.show(e);
	}
}

void BugReportForm::selectOutput(void)
{
	QFileDialog file_dlg;

	file_dlg.setWindowTitle(trUtf8("Select report output folder"));
	file_dlg.setFileMode(QFileDialog::DirectoryOnly);
	file_dlg.setModal(true);

	if(file_dlg.exec()==QFileDialog::Accepted)
		output_edt->setText(file_dlg.selectedFiles().at(0));
}
