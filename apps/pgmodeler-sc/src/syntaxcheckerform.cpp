/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2021 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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

#include "syntaxcheckerform.h"
#include "pgmodeleruins.h"
#include "globalattributes.h"
#include "generalconfigwidget.h"
#include "pgmodeleruins.h"
#include "sourceeditorwidget.h"
#include "aboutwidget.h"
#include "baseform.h"

const QString SyntaxCheckerForm::UntitledFile = QT_TR_NOOP("(untitled)");

SyntaxCheckerForm::SyntaxCheckerForm(QWidget *parent) : QWidget(parent)
{
	QToolButton *btn = nullptr;
	QFont fnt;

	setupUi(this);
	setWindowTitle(windowTitle() + " " + GlobalAttributes::PgModelerVersion);

	for(auto &obj : bnts_parent_wgt->children())
	{
		btn = dynamic_cast<QToolButton *>(obj);
		if(!btn) continue;

		fnt = btn->font();
		fnt.setBold(true);
		btn->setFont(fnt);
		PgModelerUiNs::createDropShadow(btn);
		PgModelerUiNs::configureWidgetFont(btn, PgModelerUiNs::SmallFontFactor);

		if(!btn->toolTip().isEmpty() && !btn->shortcut().toString().isEmpty())
			btn->setToolTip(btn->toolTip() + QString(" (%1)").arg(btn->shortcut().toString()));
	}

	GeneralConfigWidget general_conf_wgt;
	general_conf_wgt.loadConfiguration();

	alert_frm->setVisible(false);

	syntax_txt = PgModelerUiNs::createNumberedTextEditor(syntax_wgt);
	syntax_hl = new SyntaxHighlighter(syntax_txt);
	syntax_hl->loadConfiguration(GlobalAttributes::getXMLHighlightConfPath());

	SourceEditorWidget::setDefaultEditorPalette(syntax_txt->palette());

	syntax_conf_sel = new FileSelectorWidget(syntax_conf_wgt);
	syntax_conf_sel->setReadOnly(true);

	QVBoxLayout *vbox = new QVBoxLayout(syntax_conf_wgt);
	vbox->addWidget(syntax_conf_sel);
	vbox->setContentsMargins(0, 0, 0, 0);

	syntax_conf_sel->setSelectedFile(GlobalAttributes::getSchHighlightConfPath());
	syntax_conf_sel->setNameFilters({ tr("Syntax highlight config file (*.conf)") });

	QAction *act = nullptr;
	stx_action_grp = new QActionGroup(&syntax_cfg_menu);

	act = syntax_cfg_menu.addAction("Schema file", this, SLOT(loadSyntaxConfig()));
	stx_action_grp->addAction(act);
	act->setCheckable(true);
	act->setChecked(true);
	act->setData(GlobalAttributes::SchHighlightConf);

	act = syntax_cfg_menu.addAction("XML script", this, SLOT(loadSyntaxConfig()));
	stx_action_grp->addAction(act);
	act->setCheckable(true);
	act->setChecked(false);
	act->setData(GlobalAttributes::XMLHighlightConf);

	act = syntax_cfg_menu.addAction("SQL script", this, SLOT(loadSyntaxConfig()));
	stx_action_grp->addAction(act);
	act->setData(GlobalAttributes::SQLHighlightConf);
	act->setCheckable(true);
	act->setChecked(false);

	syntax_tb->setMenu(&syntax_cfg_menu);
	syntax_tb->setStyleSheet("QToolButton::menu-indicator { \
image: url(':/styles/styles/h_menu_indicator.png'); \
subcontrol-position: right center; }");

	syntax_cfg_menu.installEventFilter(this);

	connect(apply_conf_tb, SIGNAL(clicked(bool)), this, SLOT(applySyntaxConfig()));
	connect(save_conf_tb, SIGNAL(clicked(bool)), this, SLOT(saveSyntaxConfig()));
	connect(reload_conf_tb, SIGNAL(clicked(bool)), this, SLOT(loadSyntaxConfig()));
	connect(new_tb, SIGNAL(clicked(bool)), this, SLOT(addEditorTab()));
	connect(load_tb, SIGNAL(clicked(bool)), this, SLOT(loadFile()));
	connect(exit_tb, SIGNAL(clicked(bool)), this, SLOT(close()));
	connect(save_tb, SIGNAL(clicked(bool)), this, SLOT(saveFile()));
	connect(editors_tbw, SIGNAL(tabCloseRequested(int)), this, SLOT(closeEditorTab(int)));
	connect(use_tmpl_file_chk, SIGNAL(toggled(bool)), this, SLOT(loadSyntaxConfig()));

	connect(syntax_txt, &NumberedTextEditor::textChanged, [&](){
		alert_frm->setVisible(true);
	});

	connect(save_as_tb, &QToolButton::clicked, [&](){
		saveFile(true);
	});

	connect(about_tb, &QToolButton::clicked, [&](){
		AboutWidget *info_wgt = new AboutWidget;
		BaseForm base_frm;
		base_frm.setMainWidget(info_wgt);
		base_frm.exec();
	});
}

SyntaxCheckerForm::~SyntaxCheckerForm()
{
	while(editors_tbw->count() > 0)
		closeEditorTab(0, false);
}

void SyntaxCheckerForm::showEvent(QShowEvent *)
{
	h_splitter->setSizes({ width()/2, width()/3});
	loadSyntaxConfig();
}

void SyntaxCheckerForm::closeEvent(QCloseEvent *event)
{
	bool editors_modified = false;
	SourceEditorWidget *editor = nullptr;

	for(int tab = 0; tab < editors_tbw->count(); tab++)
	{
		editor = dynamic_cast<SourceEditorWidget *>(editors_tbw->widget(tab));

		if(editor->isModified())
		{
			editors_modified = true;
			break;
		}
	}

	if(alert_frm->isVisible() || editors_modified)
	{
		Messagebox msgbox;

		msgbox.show(tr("There are modified files! Do you want to exit without saving them?"), Messagebox::ConfirmIcon, Messagebox::YesNoButtons);

		if(msgbox.result() == QDialog::Rejected)
			event->ignore();
	}
}

bool SyntaxCheckerForm::eventFilter(QObject *object, QEvent *event)
{
	if(object == &syntax_cfg_menu && event->type() == QEvent::Show)
	{
		syntax_cfg_menu.move(mapToGlobal(syntax_tb->pos() + QPoint(syntax_tb->width(), 0)));
		syntax_cfg_menu.show();
		return true;
	}

	return QWidget::eventFilter(object, event);
}

void SyntaxCheckerForm::loadSyntaxConfig()
{
	QAction *act = stx_action_grp->checkedAction();
	QFile input;
	QString filename;

	if(!act)
		filename = GlobalAttributes::getSchHighlightConfPath();
	else
	{
		if(!use_tmpl_file_chk->isChecked())
			filename = GlobalAttributes::getConfigurationFilePath(act->data().toString());
		else
			filename = GlobalAttributes::getTmplConfigurationFilePath("", act->data().toString() + GlobalAttributes::ConfigurationExt);
	}

	try
	{
		input.setFileName(filename);
		input.open(QFile::ReadOnly);

		if(!input.isOpen())
		{
			throw Exception(Exception::getErrorMessage(ErrorCode::FileDirectoryNotAccessed).arg(filename),
											ErrorCode::FileDirectoryNotAccessed, __PRETTY_FUNCTION__, __FILE__, __LINE__);
		}

		syntax_txt->setPlainText(input.readAll());
		input.close();

		syntax_conf_sel->setSelectedFile(filename);
		save_conf_tb->setEnabled(true);
		apply_conf_tb->setEnabled(true);
		reload_conf_tb->setEnabled(true);
		alert_frm->setVisible(false);
		applySyntaxConfig(false);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(), __PRETTY_FUNCTION__, __FILE__, __LINE__, &e);
	}
}

void SyntaxCheckerForm::applySyntaxConfig(bool from_temp_file)
{
	QTemporaryFile tmp_file;
	QString filename;

	if(from_temp_file)
	{
		tmp_file.setAutoRemove(false);
		tmp_file.setFileTemplate(GlobalAttributes::getTemporaryFilePath("temp_XXXXXX.conf"));
		tmp_file.open();
		filename = tmp_file.fileName();

		if(!tmp_file.isOpen())
		{
			throw Exception(Exception::getErrorMessage(ErrorCode::FileDirectoryNotAccessed).arg(filename),
											ErrorCode::FileDirectoryNotAccessed, __PRETTY_FUNCTION__, __FILE__, __LINE__);
		}

		tmp_file.write(syntax_txt->toPlainText().toUtf8());
		tmp_file.close();
	}
	else if(stx_action_grp->checkedAction())
	{
		if(!use_tmpl_file_chk->isChecked())
			filename = GlobalAttributes::getConfigurationFilePath(stx_action_grp->checkedAction()->data().toString());
		else
			filename = GlobalAttributes::getTmplConfigurationFilePath("", stx_action_grp->checkedAction()->data().toString() + GlobalAttributes::ConfigurationExt);
	}

	try
	{
		/* Testing the temp file contents against a dummy syntax highlighter before
		 * applying to the open editors */
		QPlainTextEdit dummy_txt;
		SyntaxHighlighter stx_hl(&dummy_txt);
		stx_hl.loadConfiguration(filename);

		SourceEditorWidget *editor = nullptr;
		for(int tab = 0; tab < editors_tbw->count(); tab++)
		{
			editor = dynamic_cast<SourceEditorWidget *>(editors_tbw->widget(tab));
			editor->loadSyntaxConfig(filename);
		}
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(), __PRETTY_FUNCTION__, __FILE__, __LINE__, &e);
	}

	if(from_temp_file)
		tmp_file.remove();
}

void SyntaxCheckerForm::saveSyntaxConfig()
{
	QFile input;
	QString filename = syntax_conf_sel->getSelectedFile();

	input.setFileName(filename);
	input.open(QFile::WriteOnly);

	if(!input.isOpen())
	{
		throw Exception(Exception::getErrorMessage(ErrorCode::FileDirectoryNotAccessed).arg(filename),
										ErrorCode::FileDirectoryNotAccessed, __PRETTY_FUNCTION__, __FILE__, __LINE__);
	}

	input.write(syntax_txt->toPlainText().toUtf8());
	input.close();
	alert_frm->setVisible(false);
	applySyntaxConfig(true);
}

void SyntaxCheckerForm::saveFile(bool save_as)
{
	SourceEditorWidget *editor = dynamic_cast<SourceEditorWidget *>(editors_tbw->widget(editors_tbw->currentIndex()));
	QString filename = editor->getFilename();

	if(save_as || filename.isEmpty())
	{
		QStringList files = showFileDialog(true);

		if(files.isEmpty())
			return;

		filename = files.at(0);
	}

	editor->saveFile(filename);
	QFileInfo fi(filename);
	editors_tbw->setTabText(editors_tbw->currentIndex(), fi.fileName());
	editors_tbw->setTabToolTip(editors_tbw->currentIndex(), fi.absoluteFilePath());
}

void SyntaxCheckerForm::setTabModified(bool modified)
{
	QString tab_text = editors_tbw->tabText(editors_tbw->currentIndex());

	if(modified)
		tab_text += '*';
	else
		tab_text.remove('*');

	editors_tbw->setTabText(editors_tbw->currentIndex(), tab_text);
}

QStringList SyntaxCheckerForm::showFileDialog(bool save_mode)
{
	QFileDialog file_dlg;
	QStringList files;

	file_dlg.setNameFilters({ tr("Schema file (*.sch)"),
														tr("Database model file (*.dbm)"),
														tr("pgModeler config file (*.conf)"),
														tr("Objects metadata file (*.omf)"),
														tr("SQL script file (*.sql)"),
														tr("XML file (*.xml)"),
														tr("All files (*.*)") });

	file_dlg.setWindowTitle(save_mode ? tr("Save file") : tr("Load file"));
	file_dlg.setFileMode(save_mode ? QFileDialog::AnyFile : QFileDialog::ExistingFiles);
	file_dlg.setAcceptMode(save_mode ? QFileDialog::AcceptSave : QFileDialog::AcceptOpen);

	if(save_mode)
	{
		file_dlg.setDefaultSuffix(".sch");
		connect(&file_dlg, &QFileDialog::filterSelected, [&](QString filter){
			filter.remove(QRegExp("(.)+(\\*)"));
			filter.remove(")");
			file_dlg.setDefaultSuffix(filter);
		});
	}

	if(file_dlg.exec() == QFileDialog::Accepted)
		files = file_dlg.selectedFiles();

	return files;
}

void SyntaxCheckerForm::loadFile()
{
	try
	{
		QStringList files = showFileDialog(false);

		if(!files.isEmpty())
			loadFiles(files);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(), __PRETTY_FUNCTION__, __FILE__, __LINE__, &e);
	}
}

void SyntaxCheckerForm::loadFiles(const QStringList &filenames)
{
	try
	{
		for(auto &file : filenames)
			addEditorTab(file);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(), __PRETTY_FUNCTION__, __FILE__, __LINE__, &e);
	}
}

void SyntaxCheckerForm::addEditorTab(const QString &filename)
{
	SourceEditorWidget *editor_wgt = nullptr;
	QFileInfo fi(filename);

	try
	{
		editor_wgt = new SourceEditorWidget;
		editor_wgt->loadSyntaxConfig(syntax_conf_sel->getSelectedFile());

		if(!filename.isEmpty())
			editor_wgt->loadFile(filename);

		connect(editor_wgt, SIGNAL(s_editorModified(bool)), this, SLOT(setTabModified(bool)));
	}
	catch(Exception &e)
	{
		delete editor_wgt;
		throw Exception(e.getErrorMessage(), e.getErrorCode(), __PRETTY_FUNCTION__, __FILE__, __LINE__, &e);
	}

	editors_tbw->addTab(editor_wgt, filename.isEmpty() ? UntitledFile : fi.fileName());
	editors_tbw->setTabToolTip(editors_tbw->count() - 1, filename.isEmpty() ? "" : fi.absoluteFilePath());
	editors_tbw->setCurrentIndex(editors_tbw->count() - 1);
	save_as_tb->setEnabled(true);
	save_tb->setEnabled(true);
}

void SyntaxCheckerForm::closeEditorTab(int idx, bool confirm_close)
{
	SourceEditorWidget *editor_wgt = dynamic_cast<SourceEditorWidget *>(editors_tbw->widget(idx));
	Messagebox msgbox;

	if(editor_wgt->isModified() && confirm_close)
		msgbox.show(tr("The code was modified! Do you really want to close it without save?"), Messagebox::ConfirmIcon, Messagebox::YesNoButtons);

	if(confirm_close && msgbox.result() == QDialog::Rejected)
		return;

	editors_tbw->removeTab(idx);
	delete(editor_wgt);

	bool enable = editors_tbw->count() > 0;
	save_as_tb->setEnabled(enable);
	save_tb->setEnabled(enable);
}
