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

#include "numberedtexteditor.h"
#include "generalconfigwidget.h"
#include <QTextBlock>
#include <QScrollBar>
#include <QDebug>
#include <QFileDialog>
#include <QTemporaryFile>
#include "pgmodeleruins.h"

bool NumberedTextEditor::line_nums_visible=true;
bool NumberedTextEditor::highlight_lines=true;
QColor NumberedTextEditor::line_hl_color=Qt::yellow;
QFont NumberedTextEditor::default_font=QFont(QString("Source Code Pro"), 10);
double NumberedTextEditor::tab_width=0;
QString NumberedTextEditor::src_editor_app=QString();
QString NumberedTextEditor::src_editor_app_args=QString();

NumberedTextEditor::NumberedTextEditor(QWidget * parent, bool handle_ext_files) : QPlainTextEdit(parent)
{
	this->handle_ext_files = handle_ext_files;
	line_number_wgt=new LineNumbersWidget(this);
	top_widget = nullptr;
	load_file_btn = clear_btn	 = nullptr;

	if(handle_ext_files)
	{
		QPalette pal;
		QHBoxLayout *hbox = new QHBoxLayout, *hbox1 = new QHBoxLayout;
		QFont font = this->font();

		font.setPointSizeF(font.pointSizeF() * 0.95);

		top_widget = new QWidget(this);
		top_widget->setAutoFillBackground(true);

		pal.setColor(QPalette::Window, LineNumbersWidget::getBackgroundColor());
		top_widget->setPalette(pal);
		top_widget->setVisible(handle_ext_files);
		top_widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

		hbox->setContentsMargins(2,2,2,2);
		hbox1->setContentsMargins(0,0,0,0);

		QLabel *ico = new QLabel(this);
		msg_lbl = new QLabel(this);
		msg_lbl->setTextInteractionFlags(Qt::TextSelectableByMouse);

		ico->setMaximumSize(22,22);
		ico->setPixmap(QPixmap(PgModelerUiNs::getIconPath("msgbox_alerta")));
		ico->setScaledContents(true);

		editor_alert_wgt = new QWidget(this);
		editor_alert_wgt->setFont(font);
		hbox1->addWidget(ico);
		hbox1->addWidget(msg_lbl);
		editor_alert_wgt->setLayout(hbox1);
		editor_alert_wgt->setVisible(false);

		hbox->addWidget(editor_alert_wgt);
		hbox->addSpacerItem(new QSpacerItem(10,10, QSizePolicy::Expanding));

		load_file_btn = new QToolButton(top_widget);
		load_file_btn->setIcon(QPixmap(PgModelerUiNs::getIconPath("abrir")));
		load_file_btn->setIconSize(QSize(16,16));
		load_file_btn->setAutoRaise(true);
		load_file_btn->setText(tr("Load"));
		load_file_btn->setToolTip(tr("Load the object's source code from an external file"));
		load_file_btn->setFont(font);
		load_file_btn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
		hbox->addWidget(load_file_btn);
		connect(load_file_btn, SIGNAL(clicked(bool)), this, SLOT(loadFile()));

		edit_src_btn = new QToolButton(top_widget);
		edit_src_btn->setIcon(QPixmap(PgModelerUiNs::getIconPath("editar")));
		edit_src_btn->setIconSize(QSize(16,16));
		edit_src_btn->setAutoRaise(true);
		edit_src_btn->setText(tr("Edit"));
		edit_src_btn->setToolTip(tr("Edit the source code in the preferred external editor"));
		edit_src_btn->setFont(font);
		edit_src_btn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
		hbox->addWidget(edit_src_btn);
		connect(edit_src_btn, SIGNAL(clicked(bool)), this, SLOT(editSource()));

		clear_btn = new QToolButton(top_widget);
		clear_btn->setIcon(QPixmap(PgModelerUiNs::getIconPath("limpartexto")));
		clear_btn->setIconSize(QSize(16,16));
		clear_btn->setAutoRaise(true);
		clear_btn->setText(tr("Clear"));
		clear_btn->setFont(font);
		clear_btn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
		clear_btn->setDisabled(true);

		connect(clear_btn, &QToolButton::clicked, [&](){
			this->clear();
			clear_btn->setEnabled(false);
		});

		connect(this, &NumberedTextEditor::textChanged, [&](){
			clear_btn->setEnabled(!this->toPlainText().isEmpty() && !this->isReadOnly());
		});

		hbox->addWidget(clear_btn);
		top_widget->setLayout(hbox);

		connect(&src_editor_proc, SIGNAL(finished(int)), this, SLOT(updateSource(int)));
		connect(&src_editor_proc, SIGNAL(started()), this, SLOT(handleProcessStart()));
		connect(&src_editor_proc, SIGNAL(errorOccurred(QProcess::ProcessError)), this, SLOT(handleProcessError()));
	}

	setWordWrapMode(QTextOption::NoWrap);

	connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));
	connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumbers()));
	connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumbersSize()));

	setCustomContextMenuEnabled(true);
}

NumberedTextEditor::~NumberedTextEditor()
{
	if(src_editor_proc.state() != QProcess::NotRunning)
	{
		disconnect(&src_editor_proc, nullptr, this, nullptr);
		src_editor_proc.terminate();
		src_editor_proc.waitForFinished();
		QFile(tmp_src_file).remove();
	}
}

void NumberedTextEditor::setCustomContextMenuEnabled(bool enabled)
{
	if(!enabled)
	{
		setContextMenuPolicy(Qt::NoContextMenu);
		disconnect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu()));
	}
	else
	{
		setContextMenuPolicy(Qt::CustomContextMenu);
		connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu()), Qt::UniqueConnection);
	}
}

void NumberedTextEditor::setDefaultFont(const QFont &font)
{
	default_font=font;
}

void NumberedTextEditor::setLineNumbersVisible(bool value)
{
	line_nums_visible=value;
}

void NumberedTextEditor::setHighlightLines(bool value)
{
	highlight_lines=value;
}

void NumberedTextEditor::setLineHighlightColor(const QColor &color)
{
	line_hl_color=color;
}

void NumberedTextEditor::setTabDistance(double value)
{
	if(value < 0)
		tab_width=0;
	else
		tab_width=value;
}

double NumberedTextEditor::getTabDistance()
{
	if(static_cast<int>(tab_width) == 0)
		return 80;
	else
	{
		QFontMetrics fm(default_font);
		int chr_width = 0;

		#if (QT_VERSION < QT_VERSION_CHECK(5, 14, 0))
			chr_width = fm.width(' ');
		#else
			chr_width = fm.horizontalAdvance(' ');
		#endif

		return tab_width * chr_width;
	}
}

void NumberedTextEditor::setSourceEditorApp(const QString &app)
{
	NumberedTextEditor::src_editor_app = app;
}

void NumberedTextEditor::setSourceEditorAppArgs(const QString &args)
{
	NumberedTextEditor::src_editor_app_args = args;
}

void NumberedTextEditor::showContextMenu()
{
	QMenu *ctx_menu;
	QAction *act=nullptr;

	ctx_menu=createStandardContextMenu();

	if(!isReadOnly())
	{
		ctx_menu->addSeparator();

		act=ctx_menu->addAction(tr("Upper case"), this, SLOT(changeSelectionToUpper()), QKeySequence(QString("Ctrl+U")));
		act->setEnabled(textCursor().hasSelection());

		act=ctx_menu->addAction(tr("Lower case"), this, SLOT(changeSelectionToLower()), QKeySequence(QString("Ctrl+Shift+U")));
		act->setEnabled(textCursor().hasSelection());

		ctx_menu->addSeparator();

		act=ctx_menu->addAction(tr("Ident right"), this, SLOT(identSelectionRight()), QKeySequence(QString("Tab")));
		act->setEnabled(textCursor().hasSelection());

		act=ctx_menu->addAction(tr("Ident left"), this, SLOT(identSelectionLeft()), QKeySequence(QString("Shift+Tab")));
		act->setEnabled(textCursor().hasSelection());
	}

	ctx_menu->exec(QCursor::pos());
	delete ctx_menu;
}

void NumberedTextEditor::changeSelectionToLower()
{
	changeSelectionCase(true);
}

void NumberedTextEditor::changeSelectionToUpper()
{
	changeSelectionCase(false);
}

void NumberedTextEditor::changeSelectionCase(bool lower)
{
	QTextCursor cursor=textCursor();

	if(cursor.hasSelection())
	{
		int start=cursor.selectionStart(),
				end=cursor.selectionEnd();

		if(!lower)
			cursor.insertText(cursor.selectedText().toUpper());
		else
			cursor.insertText(cursor.selectedText().toLower());

		cursor.setPosition(start);
		cursor.setPosition(end, QTextCursor::KeepAnchor);
		setTextCursor(cursor);
	}
}

void NumberedTextEditor::identSelectionRight()
{
	identSelection(true);
}

void NumberedTextEditor::identSelectionLeft()
{
	identSelection(false);
}

void NumberedTextEditor::identSelection(bool ident_right)
{
	QTextCursor cursor=textCursor();

	if(cursor.hasSelection())
	{
		QStringList lines;
		int start=-1,	end=-1,
				factor=(ident_right ? 1 : -1),	count=0;

		/* Forcing the selection of the very beggining of the first line and
		as well the end of the last line to avoid moving chars and break words wrongly */
		start=toPlainText().lastIndexOf(QChar('\n'), cursor.selectionStart());
		end=toPlainText().indexOf(QChar('\n'), cursor.selectionEnd());

		cursor.setPosition(start, QTextCursor::MoveAnchor);
		cursor.setPosition(end, QTextCursor::KeepAnchor);
		lines=cursor.selectedText().split(QChar(QChar::ParagraphSeparator));

		for(int i=0; i < lines.size(); i++)
		{
			if(!lines[i].isEmpty())
			{
				if(ident_right)
				{
					lines[i].prepend(QChar('\t'));
					count++;
				}
				else if(lines[i].at(0)==QChar('\t'))
				{
					lines[i].remove(0,1);
					count++;
				}
			}
		}

		if(count > 0)
		{
			cursor.insertText(lines.join(QChar('\n')));

			//Preserving the selection in the text to permit user perform several identations
			cursor.setPosition(start);
			cursor.setPosition(end + (count * factor), QTextCursor::KeepAnchor);
			setTextCursor(cursor);
		}
	}
}

void NumberedTextEditor::loadFile()
{
	QFileDialog sql_file_dlg;

	sql_file_dlg.setDefaultSuffix(QString("sql"));
	sql_file_dlg.setFileMode(QFileDialog::AnyFile);
	sql_file_dlg.setNameFilter(tr("SQL file (*.sql);;All files (*.*)"));
	sql_file_dlg.setModal(true);
	sql_file_dlg.setWindowTitle(tr("Load file"));
	sql_file_dlg.setAcceptMode(QFileDialog::AcceptOpen);
	sql_file_dlg.exec();

	if(sql_file_dlg.result()==QDialog::Accepted)
	{
		QFile file;
		file.setFileName(sql_file_dlg.selectedFiles().at(0));

		if(!file.open(QFile::ReadOnly))
			throw Exception(Exception::getErrorMessage(ErrorCode::FileDirectoryNotAccessed)
											.arg(sql_file_dlg.selectedFiles().at(0)),
											ErrorCode::FileDirectoryNotAccessed ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		this->clear();
		this->setPlainText(file.readAll());
		file.close();


		clear_btn->setEnabled(!this->toPlainText().isEmpty());
	}
}

void NumberedTextEditor::editSource()
{
	QByteArray buffer;
	QFile input;
	QStringList args;

	if(tmp_src_file.isEmpty())
	{
		QTemporaryFile tmp_file;
		tmp_file.setFileTemplate(GlobalAttributes::getTemporaryFilePath("source_XXXXXX.sql"));
		tmp_file.open();
		tmp_src_file = tmp_file.fileName();
		tmp_file.close();
	}

	input.setFileName(tmp_src_file);

	if(!input.open(QFile::WriteOnly | QFile::Truncate))
		throw Exception(Exception::getErrorMessage(ErrorCode::FileDirectoryNotAccessed)
										.arg(tmp_src_file),
										ErrorCode::FileDirectoryNotAccessed ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	buffer.append(this->toPlainText());
	input.write(buffer);
	input.close();

	//Starting the source editor application using the temp source file as input
	if(!src_editor_app_args.isEmpty())
		args.push_back(src_editor_app_args);

	args.push_back(tmp_src_file);

	src_editor_proc.setProgram(src_editor_app);
	src_editor_proc.setArguments(args);
	src_editor_proc.setWorkingDirectory(QDir::currentPath());
	src_editor_proc.start();
	src_editor_proc.waitForStarted();
}

void NumberedTextEditor::enableEditor()
{
	editor_alert_wgt->setVisible(false);
	load_file_btn->setEnabled(true);
	edit_src_btn->setEnabled(true);
	clear_btn->setEnabled(!this->toPlainText().isEmpty());
	this->setReadOnly(false);
}

void NumberedTextEditor::updateSource(int exit_code)
{
	if(exit_code != 0)
		handleProcessError();
	else
	{
		QFile input(tmp_src_file);

		enableEditor();

		if(!input.open(QFile::ReadOnly))
			throw Exception(Exception::getErrorMessage(ErrorCode::FileDirectoryNotAccessed)
											.arg(tmp_src_file),
											ErrorCode::FileDirectoryNotAccessed ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		this->setPlainText(input.readAll());
		input.close();
		input.remove();
	}
}

void NumberedTextEditor::handleProcessStart()
{
	if(src_editor_proc.state() == QProcess::Running)
	{
		msg_lbl->setText(PgModelerUiNs::formatMessage(tr("The source editor `%1' is running on `pid: %2'.")
																									.arg(src_editor_proc.program()).arg(src_editor_proc.processId())));
		editor_alert_wgt->setVisible(true);
		load_file_btn->setEnabled(false);
		edit_src_btn->setEnabled(false);
		clear_btn->setEnabled(false);
		this->setReadOnly(true);
	}
}

void NumberedTextEditor::handleProcessError()
{
	Messagebox msg_box;
	QStringList errors = { src_editor_proc.errorString(),  src_editor_proc.readAllStandardError() };

	msg_box.show(PgModelerUiNs::formatMessage(tr("Failed to the source code editor <strong>%1</strong>! Make to sure that the source editor path points to a valid executable and the current user has permission to run the application. Error message returned: <strong>%2</strong>")
																						.arg(src_editor_proc.program())
																						.arg(errors.join(QString("\n\n")))), Messagebox::ErrorIcon);

	enableEditor();
}

void NumberedTextEditor::setReadOnly(bool ro)
{
	if(handle_ext_files)
	{
		load_file_btn->setEnabled(!ro);
		edit_src_btn->setEnabled(!ro);
		clear_btn->setEnabled(!ro && !this->toPlainText().isEmpty());
	}

	QPlainTextEdit::setReadOnly(ro);
}

void NumberedTextEditor::setFocus()
{
	QPlainTextEdit::setFocus();
	this->highlightCurrentLine();
}

void NumberedTextEditor::updateLineNumbers()
{
	line_number_wgt->setVisible(line_nums_visible);
	if(!line_nums_visible) return;

	setFont(default_font);
	line_number_wgt->setFont(default_font);

	QTextBlock block = firstVisibleBlock();
	int block_number = block.blockNumber(),
			//Calculates the first block postion (in widget coordinates)
			top = static_cast<int>(blockBoundingGeometry(block).translated(contentOffset()).top()),
			bottom = top +  static_cast<int>(blockBoundingRect(block).height()),
			dy = top;
	unsigned first_line=0, line_count=0;	
	double tab_stop_dist = 0;

	// Calculates the visible lines by iterating over the visible/valid text blocks.
	while(block.isValid())
	{
		if(block.isVisible())
		{
			line_count++;
			if(first_line==0)
				first_line=static_cast<unsigned>(block_number + 1);
		}

		block = block.next();
		top = bottom;
		bottom = top + static_cast<int>(blockBoundingRect(block).height());
		++block_number;

		/* Check if the line count converted to widget coordinates is higher than the widget height.
		 This is done to avoid draw line numbers that are beyond the widget's height */
		if((static_cast<int>(line_count) * fontMetrics().height()) > this->height())
			break;
	}

	line_number_wgt->drawLineNumbers(first_line, line_count, dy);

	#if (QT_VERSION < QT_VERSION_CHECK(5, 14, 0))
		tab_stop_dist = this->tabStopWidth();
	#else
		tab_stop_dist = this->tabStopDistance();
	#endif

	if(round(tab_stop_dist) != round(NumberedTextEditor::getTabDistance()))
	{
		#if (QT_VERSION < QT_VERSION_CHECK(5, 14, 0))
			this->setTabStopWidth(NumberedTextEditor::getTabDistance());
		#else
			this->setTabStopDistance(NumberedTextEditor::getTabDistance());
		#endif
	}
}

void NumberedTextEditor::updateLineNumbersSize()
{
	int py = (handle_ext_files && top_widget ? top_widget->height() : 0);

	if(line_nums_visible)
	{
		QRect rect=contentsRect();

		setViewportMargins(getLineNumbersWidth(), py, 0, 0);
		line_number_wgt->setGeometry(QRect(rect.left(), rect.top() + py, getLineNumbersWidth(), rect.height() - py));

		if(top_widget)
			top_widget->setGeometry(rect.left(), rect.top(),
															rect.width() - (this->verticalScrollBar()->isVisible() ? this->verticalScrollBar()->width() : 0),
															top_widget->height());
	}
	else
		setViewportMargins(0, py, 0, 0);
}

int NumberedTextEditor::getLineNumbersWidth()
{
	int digits=1, max=qMax(1, blockCount()),
			chr_width = 0;

	while(max >= 10)
	{
		max /= 10;
		++digits;
	}

	#if (QT_VERSION < QT_VERSION_CHECK(5, 14, 0))
		chr_width = fontMetrics().width(QChar('9'));
	#else
		chr_width = fontMetrics().horizontalAdvance(QChar('9'));
	#endif

	return (15 + chr_width * digits);
}

void NumberedTextEditor::resizeEvent(QResizeEvent *event)
{
	QPlainTextEdit::resizeEvent(event);
	updateLineNumbersSize();
}

void NumberedTextEditor::keyPressEvent(QKeyEvent *event)
{
	if(!isReadOnly() && textCursor().hasSelection())
	{
		if(event->key()==Qt::Key_U && event->modifiers()!=Qt::NoModifier)
		{
			if(event->modifiers()==Qt::ControlModifier)
				changeSelectionToUpper();
			else if(event->modifiers()==(Qt::ControlModifier | Qt::ShiftModifier))
				changeSelectionToLower();
		}
		else if(event->key()==Qt::Key_Tab || event->key()==Qt::Key_Backtab)
		{
			if(event->key()==Qt::Key_Tab)
				identSelectionRight();
			else if(event->key()==Qt::Key_Backtab)
				identSelectionLeft();
		}
		else
			QPlainTextEdit::keyPressEvent(event);
	}
	else
		QPlainTextEdit::keyPressEvent(event);
}

void NumberedTextEditor::highlightCurrentLine()
{
	QList<QTextEdit::ExtraSelection> extraSelections;

	if(highlight_lines && !isReadOnly())
	{
		QTextEdit::ExtraSelection selection;
		selection.format.setBackground(line_hl_color);
		selection.format.setProperty(QTextFormat::FullWidthSelection, true);
		selection.cursor = textCursor();
		selection.cursor.clearSelection();
		extraSelections.append(selection);
	}

	setExtraSelections(extraSelections);
}
