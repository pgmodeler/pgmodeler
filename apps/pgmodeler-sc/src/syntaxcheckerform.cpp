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

const QString SyntaxCheckerForm::UntitledFile = QT_TR_NOOP("(untitled)");

QPalette SyntaxCheckerForm::def_editor_pal;

SyntaxCheckerForm::SyntaxCheckerForm(QWidget *parent) : QWidget(parent)
{
	QToolButton *btn = nullptr;
	QFont fnt;

	setupUi(this);

	for(auto &obj : bnts_parent_wgt->children())
	{
		btn = dynamic_cast<QToolButton *>(obj);
		if(!btn) continue;

		fnt = btn->font();
		fnt.setBold(true);
		btn->setFont(fnt);
		PgModelerUiNs::createDropShadow(btn);
		PgModelerUiNs::configureWidgetFont(btn, PgModelerUiNs::SmallFontFactor);
	}

	GeneralConfigWidget general_conf_wgt;
	general_conf_wgt.loadConfiguration();

	alert_frm->setVisible(false);

	syntax_txt = PgModelerUiNs::createNumberedTextEditor(syntax_wgt);
	syntax_hl = new SyntaxHighlighter(syntax_txt);
	syntax_hl->loadConfiguration(GlobalAttributes::getXMLHighlightConfPath());

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
	act->setData(GlobalAttributes::getSchHighlightConfPath());

	act = syntax_cfg_menu.addAction("XML script", this, SLOT(loadSyntaxConfig()));
	stx_action_grp->addAction(act);
	act->setCheckable(true);
	act->setChecked(false);
	act->setData(GlobalAttributes::getXMLHighlightConfPath());

	act = syntax_cfg_menu.addAction("SQL script", this, SLOT(loadSyntaxConfig()));
	stx_action_grp->addAction(act);
	act->setData(GlobalAttributes::getSQLHighlightConfPath());
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
	connect(syntax_txt, SIGNAL(undoAvailable(bool)), alert_frm, SLOT(setVisible(bool)));
	connect(new_tb, SIGNAL(clicked(bool)), this, SLOT(addEditorTab()));
	connect(load_tb, SIGNAL(clicked(bool)), this, SLOT(loadFile()));
	connect(exit_tb, SIGNAL(clicked(bool)), this, SLOT(close()));
	connect(validate_tb, SIGNAL(clicked(bool)), this, SLOT(validateSyntax()));
	connect(save_tb, SIGNAL(clicked(bool)), this, SLOT(saveFile()));
	connect(editors_tbw, SIGNAL(tabCloseRequested(int)), this, SLOT(closeEditorTab(int)));

	connect(save_as_tb, &QToolButton::clicked, [&](){
		saveFile(true);
	});

	connect(editors_tbw, &QTabWidget::currentChanged, [&](){
		validate_tb->setEnabled(
				editors_tbw->tabText(editors_tbw->currentIndex()) == UntitledFile ||
				editors_tbw->tabText(editors_tbw->currentIndex()).endsWith(GlobalAttributes::SchemaExt));
	});
}

SyntaxCheckerForm::~SyntaxCheckerForm()
{
	while(editors_tbw->count() > 0)
		closeEditorTab(0);
}

void SyntaxCheckerForm::showEvent(QShowEvent *)
{
	h_splitter->setSizes({ width()/2, width()/3});
	loadSyntaxConfig();
}

void SyntaxCheckerForm::closeEvent(QCloseEvent *event)
{
	if(alert_frm->isVisible())
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
	QString filename = !act ? GlobalAttributes::getSchHighlightConfPath() : act->data().toString();

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
		filename = stx_action_grp->checkedAction()->data().toString();

	try
	{
		/* Testing the temp file contents against a dummy syntax highlighter before
		 * applying to the open editors */
		QPlainTextEdit dummy_txt;
		SyntaxHighlighter stx_hl(&dummy_txt);
		stx_hl.loadConfiguration(filename);

		for(auto &hl : highlighters)
		{
			hl->loadConfiguration(filename);
			hl->rehighlight();
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
}

void SyntaxCheckerForm::saveFile(bool save_as)
{
	QFile input;
	QString filename = editors_tbw->tabToolTip(editors_tbw->currentIndex());

	if(save_as || filename.isEmpty())
	{
		QStringList files = showFileDialog(true);

		if(!files.isEmpty())
			filename = files.at(0);
	}

	input.setFileName(filename);
	input.open(QFile::WriteOnly);

	if(!input.isOpen())
	{
		throw Exception(Exception::getErrorMessage(ErrorCode::FileDirectoryNotAccessed).arg(filename),
										ErrorCode::FileDirectoryNotAccessed, __PRETTY_FUNCTION__, __FILE__, __LINE__);
	}

	input.write(syntax_txt->toPlainText().toUtf8());
	input.close();

	QFileInfo fi(filename);
	editors_tbw->setTabText(editors_tbw->currentIndex(), fi.baseName());
	editors_tbw->setTabToolTip(editors_tbw->currentIndex(), fi.absoluteFilePath());
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

	file_dlg.setWindowIcon(QPixmap(PgModelerUiNs::getIconPath("pgmodeler_sch.png")));
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

void SyntaxCheckerForm::validateSyntax()
{
	SchemaParser schparser;
	int tab_idx = editors_tbw->currentIndex();
	NumberedTextEditor *editor_txt = dynamic_cast<NumberedTextEditor *>(highlighters[tab_idx]->parent());
	Messagebox msgbox;

	try
	{
		editor_txt->setPalette(def_editor_pal);
		schparser.ignoreEmptyAttributes(true);
		schparser.ignoreUnkownAttributes(true);
		schparser.loadBuffer(editor_txt->toPlainText());
		schparser.getCodeDefinition({});

		msgbox.show(tr("No lexical or sytactical errors found."), Messagebox::InfoIcon);
	}
	catch(Exception &e)
	{
		QTextCursor cursor(editor_txt->document()->findBlockByLineNumber(schparser.getCurrentLine() - 1));
		cursor.movePosition(QTextCursor::StartOfLine);
		cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, schparser.getCurrentColumn() - 1);

		editor_txt->blockSignals(true);
		editor_txt->setTextCursor(cursor);
		editor_txt->blockSignals(false);

		QPalette pal = editor_txt->palette();
		pal.setColor(QPalette::Highlight, QColor("#f00000"));
		pal.setColor(QPalette::HighlightedText, QColor("#ffffff"));
		editor_txt->setPalette(pal);

		msgbox.show(e);
	}
}

void SyntaxCheckerForm::restoreEditorPalette()
{
	NumberedTextEditor *editor = dynamic_cast<NumberedTextEditor *>(sender());
	editor->setPalette(def_editor_pal);
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
	NumberedTextEditor *editor_txt = nullptr;
	SyntaxHighlighter *editor_hl = nullptr;
	QWidget *page_wgt = nullptr;
	QVBoxLayout *vbox = nullptr;
	QFileInfo fi(filename);
	QFile input;
	QByteArray buffer;

	if(!filename.isEmpty())
	{
		input.setFileName(filename);
		input.open(QFile::ReadOnly);

		if(!input.isOpen())
		{
			throw Exception(Exception::getErrorMessage(ErrorCode::FileDirectoryNotAccessed).arg(filename),
											ErrorCode::FileDirectoryNotAccessed, __PRETTY_FUNCTION__, __FILE__, __LINE__);
		}

		buffer = input.readAll();
		input.close();
	}

	page_wgt = new QWidget;
	vbox = new QVBoxLayout(page_wgt);
	vbox->setContentsMargins(4,4,4,4);

	editor_txt = PgModelerUiNs::createNumberedTextEditor(page_wgt);
	editor_hl = new SyntaxHighlighter(editor_txt);
	vbox->addWidget(editor_txt);
	def_editor_pal = editor_txt->palette();

	try
	{
		if(!syntax_conf_sel->getSelectedFile().isEmpty())
			editor_hl->loadConfiguration(syntax_conf_sel->getSelectedFile());

		editor_txt->setPlainText(buffer);		
		highlighters.append(editor_hl);

		connect(editor_txt, SIGNAL(modificationChanged(bool)), this, SLOT(restoreEditorPalette()));
		connect(editor_txt, SIGNAL(cursorPositionChanged()), this, SLOT(restoreEditorPalette()));
	}
	catch(Exception &e)
	{
		Messagebox msgbox;
		msgbox.show(e);
	}

	editors_tbw->addTab(page_wgt, filename.isEmpty() ? UntitledFile : fi.fileName());
	editors_tbw->setTabToolTip(editors_tbw->count() - 1, filename.isEmpty() ? "" : fi.absoluteFilePath());
	editors_tbw->setCurrentIndex(editors_tbw->count() - 1);
	save_as_tb->setEnabled(true);
	save_tb->setEnabled(true);
}

void SyntaxCheckerForm::closeEditorTab(int idx)
{
	QWidget *page = editors_tbw->widget(idx);

	editors_tbw->removeTab(idx);
	highlighters.removeAt(idx);
	delete(page);

	bool enable = editors_tbw->count() > 0;
	save_as_tb->setEnabled(enable);
	save_tb->setEnabled(enable);
	validate_tb->setEnabled(enable);
}
