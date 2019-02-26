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

#include "crashhandlerform.h"
#include "messagebox.h"
#include "pgmodeleruins.h"

const QString CrashHandlerForm::AnalysisMode=QString("-analysis-mode");

CrashHandlerForm::CrashHandlerForm(bool analysis_mode, QWidget *parent, Qt::WindowFlags f) : BugReportForm(parent, f)
{
	QFile input;
	QString buf;
	QWidget *wgt=new QWidget;
	QHBoxLayout *layout=new QHBoxLayout;

	setWindowTitle(trUtf8("Crash Handler"));

	stack_txt=new QPlainTextEdit(this);
	stack_txt->setReadOnly(true);
	stack_txt->setFont(QFont("Source Code Pro"));
	stack_txt->setLineWrapMode(QPlainTextEdit::NoWrap);

	layout->addWidget(stack_txt);
	layout->setContentsMargins(4,4,4,4);
	wgt->setLayout(layout);

	logo_lbl->setPixmap(QPixmap(QString(":/imagens/imagens/crashhandler.png")));
	report_twg->addTab(wgt, trUtf8("Stack trace"));

	//Open for reading the stack trace file generated on the last crash
	input.setFileName(GlobalAttributes::TemporaryDir +
					  GlobalAttributes::DirSeparator +
					  GlobalAttributes::StacktraceFile);
	input.open(QFile::ReadOnly);

	if(input.isOpen())
	{
		buf=input.readAll();
		input.close();

		//Removes the stack trace file
		QDir stack_file;
		stack_file.remove(GlobalAttributes::TemporaryDir +
						  GlobalAttributes::DirSeparator +
						  GlobalAttributes::StacktraceFile);

		//Shows the stacktrace loaded on the widget
		stack_txt->setPlainText(buf);
	}


	//Creating an input field in order to select the input report file
	input_wgt=new QWidget(this);
	input_wgt->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

	layout=new QHBoxLayout(input_wgt);
	layout->setContentsMargins(0, 0, 0, 0);

	input_lbl=new QLabel(input_wgt);
	input_lbl->setText(trUtf8("Input:"));
	layout->addWidget(input_lbl);

	input_edt=new QLineEdit(input_wgt);
	input_edt->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	input_edt->setReadOnly(true);
	layout->addWidget(input_edt);

	load_tb=new QToolButton(input_wgt);
	load_tb->setIcon(QPixmap(PgModelerUiNs::getIconPath("abrir")));
	load_tb->setSizePolicy(output_tb->sizePolicy());
	load_tb->setToolButtonStyle(output_tb->toolButtonStyle());
	load_tb->setIconSize(output_tb->iconSize());
	load_tb->setToolTip(trUtf8("Load report file for analysis"));
	layout->addWidget(load_tb);

	save_tb=new QToolButton(input_wgt);
	save_tb->setIcon(QPixmap(PgModelerUiNs::getIconPath("salvar")));
	save_tb->setSizePolicy(output_tb->sizePolicy());
	save_tb->setToolButtonStyle(output_tb->toolButtonStyle());
	save_tb->setIconSize(output_tb->iconSize());
	save_tb->setToolTip(trUtf8("Save the attached model file on the filesystem"));
	attach_wgt->layout()->addWidget(save_tb);

	report_tab_lt->removeWidget(details_gb);
	report_tab_lt->removeWidget(output_wgt);
	report_tab_lt->removeWidget(message_frm);

	report_tab_lt->addWidget(input_wgt);
	report_tab_lt->addWidget(details_gb);
	report_tab_lt->addWidget(output_wgt);
	report_tab_lt->addWidget(message_frm);

	setAnalysisMode(analysis_mode);

	connect(load_tb, SIGNAL(clicked(void)), this, SLOT(loadReport(void)));
	connect(save_tb, SIGNAL(clicked(void)), this, SLOT(saveModel(void)));
}

void CrashHandlerForm::loadReport(const QString &filename)
{
	QFile input;
	QFileInfo fi;
	char *buf=nullptr;
	Messagebox msgbox;

	fi.setFile(filename);
	input.setFileName(filename);
	input.open(QFile::ReadOnly);

	//Raises an error if the file could not be opened
	if(!input.isOpen())
		msgbox.show(Exception::getErrorMessage(ErrorCode::FileDirectoryNotAccessed).arg(filename), Messagebox::ErrorIcon);
	else
	{
		QByteArray uncomp_buf;
		QString buf_aux, str_aux;
		int i, idx;
		QPlainTextEdit *txt_widgets[]={ actions_txt, model_txt , stack_txt};

		//Creates a text buffer
		buf=new char[fi.size()];

		//Reads the file storing it on the buffer
		input.read(buf, fi.size());
		input.close();

		//Uncompress the buffer
		uncomp_buf.append(buf, fi.size());
		uncomp_buf=qUncompress(uncomp_buf);

		delete[](buf);
		buf=nullptr;

		buf_aux=QString(uncomp_buf.data());
		i=idx=0;

		//Showing the sections of the uncompressed buffer on the respective widgets
		while(i < buf_aux.size() && idx <= 2)
		{
			if(buf_aux.at(i).toLatin1()!=CharDelimiter)
				str_aux.append(buf_aux.at(i));
			else
			{
				txt_widgets[idx++]->setPlainText(str_aux);
				str_aux.clear();
			}
			i++;
		}
	}
}

void CrashHandlerForm::loadReport(void)
{
	QFileDialog file_dlg;

	try
	{
		file_dlg.setNameFilter(trUtf8("pgModeler bug report (*.bug);;All files (*.*)"));
		file_dlg.setWindowTitle(trUtf8("Load report"));
		file_dlg.setFileMode(QFileDialog::ExistingFiles);
		file_dlg.setAcceptMode(QFileDialog::AcceptOpen);

		if(file_dlg.exec()==QFileDialog::Accepted)
		{
			loadReport(file_dlg.selectedFiles().at(0));
			input_edt->setText(file_dlg.selectedFiles().at(0));
		}
	}
	catch(Exception &e)
	{
		Messagebox msgbox;
		msgbox.show(e);
	}
}

void CrashHandlerForm::saveModel(void)
{
	QFileDialog file_dlg;

	try
	{
		file_dlg.setDefaultSuffix(QString("dbm"));
		file_dlg.setWindowTitle(trUtf8("Save model"));
		file_dlg.setNameFilter(trUtf8("Database model (*.dbm);;All files (*.*)"));
		file_dlg.setFileMode(QFileDialog::AnyFile);
		file_dlg.setAcceptMode(QFileDialog::AcceptSave);
		file_dlg.setModal(true);

		if(file_dlg.exec()==QFileDialog::Accepted)
		{
			QFile output(file_dlg.selectedFiles().at(0));
			QByteArray buf;

			output.open(QFile::WriteOnly);

			if(!output.isOpen())
				throw Exception(Exception::getErrorMessage(ErrorCode::FileDirectoryNotWritten).arg(file_dlg.selectedFiles().at(0)),
												ErrorCode::FileDirectoryNotWritten,__PRETTY_FUNCTION__,__FILE__,__LINE__);

			buf.append(model_txt->toPlainText());
			output.write(buf.data(),buf.size());
			output.close();
		}
	}
	catch(Exception &e)
	{
		Messagebox msgbox;
		msgbox.show(e);
	}
}

void CrashHandlerForm::setAnalysisMode(bool value)
{
	output_wgt->setEnabled(!value);
	attach_tb->setEnabled(!value);
	attach_mod_chk->setEnabled(!value);
	save_tb->setVisible(value);

	create_btn->setVisible(!value);
	input_wgt->setVisible(value);

	if(value)
	{
		title_lbl->setText(trUtf8("Crash handler"));
		msg_lbl->setText(trUtf8("Bug report analysis mode activated."));
	}
	else
	{
		title_lbl->setText(trUtf8("Oops! pgModeler just crashed!"));
		msg_lbl->setText(trUtf8("We apologize for what happened! It is clear that a nasty bug caused that. Please fill out the form below describing your actions before pgModeler quit unexpectedly. This will help on bug extermination and improve the software."));
	}
}

QByteArray CrashHandlerForm::generateReportBuffer(void)
{
	QByteArray buf=BugReportForm::generateReportBuffer();
	buf.append(stack_txt->toPlainText().toUtf8());
	buf.append(CharDelimiter);

	return(buf);
}
