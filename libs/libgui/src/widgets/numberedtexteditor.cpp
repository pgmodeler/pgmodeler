/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2023 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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
#include <QTextBlock>
#include <QScrollBar>
#include <QDebug>
#include <QFileDialog>
#include <QTemporaryFile>
#include "guiutilsns.h"
#include <QMenu>
#include <QHBoxLayout>
#include <QApplication>
#include <QClipboard>
#include "messagebox.h"
#include "utilsns.h"

bool NumberedTextEditor::line_nums_visible=true;
bool NumberedTextEditor::highlight_lines=true;
QColor NumberedTextEditor::line_hl_color=Qt::yellow;
QFont NumberedTextEditor::default_font=QFont("Source Code Pro", 10);
double NumberedTextEditor::tab_width=0;
QString NumberedTextEditor::src_editor_app="";
QString NumberedTextEditor::src_editor_app_args="";

NumberedTextEditor::NumberedTextEditor(QWidget * parent, bool handle_ext_files, qreal custom_fnt_size) : QPlainTextEdit(parent)
{
	this->handle_ext_files = handle_ext_files;
	line_number_wgt=new LineNumbersWidget(this);
	top_widget = nullptr;
	load_file_btn = clear_btn	 = nullptr;
	this->custom_fnt_size = custom_fnt_size;

	if(handle_ext_files)
	{
		QPalette pal;
		QHBoxLayout *hbox = new QHBoxLayout, *hbox1 = new QHBoxLayout;
		QFont font = this->font();

		font.setPointSizeF(font.pointSizeF() * 0.90);

		top_widget = new QWidget(this);
		top_widget->setAutoFillBackground(true);

		pal.setColor(QPalette::Window, LineNumbersWidget::getBackgroundColor());
		top_widget->setPalette(pal);
		top_widget->setVisible(handle_ext_files);
		top_widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

		hbox->setContentsMargins(GuiUtilsNs::LtMargin,GuiUtilsNs::LtMargin,GuiUtilsNs::LtMargin,GuiUtilsNs::LtMargin);
		hbox1->setContentsMargins(0,0,0,0);

		QLabel *ico = new QLabel(this);
		msg_lbl = new QLabel(this);
		msg_lbl->setTextInteractionFlags(Qt::TextSelectableByMouse);

		ico->setPixmap(QPixmap(GuiUtilsNs::getIconPath("alert")));
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
		load_file_btn->setIcon(QIcon(GuiUtilsNs::getIconPath("open")));
		load_file_btn->setAutoRaise(true);
		load_file_btn->setText(tr("Load"));
		load_file_btn->setToolTip(tr("Load text from an external file"));
		load_file_btn->setFont(font);
		load_file_btn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
		hbox->addWidget(load_file_btn);
		connect(load_file_btn, &QToolButton::clicked, this, &NumberedTextEditor::loadFile);

		save_file_btn = new QToolButton(top_widget);
		save_file_btn->setIcon(QIcon(GuiUtilsNs::getIconPath("save")));
		save_file_btn->setAutoRaise(true);
		save_file_btn->setText(tr("Save"));
		save_file_btn->setToolTip(tr("Save the text to a file"));
		save_file_btn->setFont(font);
		save_file_btn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
		hbox->addWidget(save_file_btn);
		connect(save_file_btn, &QToolButton::clicked, this, &NumberedTextEditor::saveFile);

		edit_src_btn = new QToolButton(top_widget);
		edit_src_btn->setIcon(QIcon(GuiUtilsNs::getIconPath("edit")));
		edit_src_btn->setAutoRaise(true);
		edit_src_btn->setText(tr("Edit"));
		edit_src_btn->setToolTip(tr("Edit the text in the defined external editor"));
		edit_src_btn->setFont(font);
		edit_src_btn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
		hbox->addWidget(edit_src_btn);

		connect(edit_src_btn,  &QToolButton::clicked, this, __slot(this, NumberedTextEditor::editSource));

		word_wrap_btn = new QToolButton(top_widget);
		word_wrap_btn->setIcon(QIcon(GuiUtilsNs::getIconPath("wordwrap")));
		word_wrap_btn->setAutoRaise(true);
		word_wrap_btn->setCheckable(true);
		word_wrap_btn->setText(tr("Wrap"));
		word_wrap_btn->setToolTip(tr("Toggles the word wrap"));
		word_wrap_btn->setFont(font);
		word_wrap_btn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
		word_wrap_btn->setDisabled(true);
		hbox->addWidget(word_wrap_btn);

		connect(word_wrap_btn,  &QToolButton::toggled, this, [this](bool checked) {
			setWordWrapMode(checked ? QTextOption::WrapAtWordBoundaryOrAnywhere : QTextOption::NoWrap);
		});

		clear_btn = new QToolButton(top_widget);
		clear_btn->setIcon(QIcon(GuiUtilsNs::getIconPath("cleartext")));
		clear_btn->setAutoRaise(true);
		clear_btn->setText(tr("Clear"));
		clear_btn->setFont(font);
		clear_btn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
		clear_btn->setDisabled(true);

		connect(clear_btn, &QToolButton::clicked, this, [this](){
			this->clear();
			clear_btn->setEnabled(false);
		});

		connect(this, &NumberedTextEditor::textChanged, this, [this](){
			clear_btn->setEnabled(!this->document()->isEmpty() && !this->isReadOnly());
			word_wrap_btn->setEnabled(!document()->isEmpty());
			save_file_btn->setEnabled(!document()->isEmpty());
		});

		ico->setMaximumSize(edit_src_btn->iconSize());
		hbox->addWidget(clear_btn);
		top_widget->setLayout(hbox);
		top_widget->adjustSize();

		connect(&src_editor_proc, &QProcess::finished, this, __slot_n(this, NumberedTextEditor::updateSource));
		connect(&src_editor_proc, &QProcess::started, this, &NumberedTextEditor::handleProcessStart);
		connect(&src_editor_proc, &QProcess::errorOccurred, this, &NumberedTextEditor::handleProcessError);
	}

	setWordWrapMode(QTextOption::NoWrap);

	connect(this, &NumberedTextEditor::cursorPositionChanged, this, &NumberedTextEditor::highlightCurrentLine);
	connect(this, &NumberedTextEditor::updateRequest, this, &NumberedTextEditor::updateLineNumbers);
	connect(this, &NumberedTextEditor::blockCountChanged, this, &NumberedTextEditor::updateLineNumbersSize);

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
		disconnect(this, &NumberedTextEditor::customContextMenuRequested, this, &NumberedTextEditor::showContextMenu);
	}
	else
	{
		setContextMenuPolicy(Qt::CustomContextMenu);
		connect(this, &NumberedTextEditor::customContextMenuRequested, this, &NumberedTextEditor::showContextMenu, Qt::UniqueConnection);
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

bool NumberedTextEditor::isHighlightLines()
{
	return highlight_lines;
}

void NumberedTextEditor::setLineHighlightColor(const QColor &color)
{
	line_hl_color=color;
}

QColor NumberedTextEditor::getLineHighlightColor()
{
	return line_hl_color;
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

	return tab_width * QFontMetrics(default_font).horizontalAdvance(' ');
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

		act = ctx_menu->addAction(tr("Paste code"), this, &NumberedTextEditor::pasteCode, QKeySequence("Ctrl+Shift+V"));
		act->setEnabled(!qApp->clipboard()->text().isEmpty());

		act = ctx_menu->addAction(tr("Upper case"), this, &NumberedTextEditor::changeSelectionToUpper, QKeySequence("Ctrl+U"));
		act->setEnabled(textCursor().hasSelection());

		act = ctx_menu->addAction(tr("Lower case"), this, &NumberedTextEditor::changeSelectionToLower, QKeySequence("Ctrl+Shift+U"));
		act->setEnabled(textCursor().hasSelection());

		ctx_menu->addSeparator();

		act = ctx_menu->addAction(tr("Ident right"), this, &NumberedTextEditor::identSelectionRight, QKeySequence("Tab"));
		act->setEnabled(textCursor().hasSelection());

		act = ctx_menu->addAction(tr("Ident left"), this, &NumberedTextEditor::identSelectionLeft, QKeySequence("Shift+Tab"));
		act->setEnabled(textCursor().hasSelection());
	}

	ctx_menu->exec(QCursor::pos());
	delete ctx_menu;
}

void NumberedTextEditor::pasteCode()
{
	QString code = qApp->clipboard()->text();
	QStringList buffer = code.split(QChar::LineFeed),
		patterns = {
			"^(\\s)*((\\%2)?(\\s)*)*(%1)",
			"(%1)((\\s)*(\\%2)?)*$",
			"(%1)(\\s)*(\\%2)(\\s)*(%1)",
			"(%1)(\\s)*(\\%2)",
			"(\\%2)(\\s)*(%1)"
		},
			opers = { "\"+", "'.", "'+", "\"." };
	QChar concat_opr = '+', str_delim = '"';
	QRegularExpression regexp;

	for(auto &line : buffer)
	{
		/* Detecting the combination of string delimiter and concatenation operator
		 * present in the current buffer line */
		for(auto &opr : opers)
		{
			str_delim = opr[0];
			concat_opr = opr[1];

			if(line.contains(QRegularExpression(QString("^(\\s)*(\\%1|\\%2)+(\\s)*").arg(str_delim, concat_opr))) &&
				 line.contains(QRegularExpression(QString("(\\s)*(\\%1|\\%2)+(\\s)*$").arg(str_delim, concat_opr))))
				break;
		}

		/* Cleaning up the line using the patterns configured with the
		 * determined string delimiter and concatation operator */
		for(auto &pattern : patterns)
		{
			regexp.setPattern(pattern.arg(str_delim, concat_opr));
			line.remove(regexp);
		}
	}

	insertPlainText(buffer.join(QChar::LineFeed));
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
		QString buff = toPlainText();

		/* Forcing the selection of the very beggining of the first line and
		as well the end of the last line to avoid moving chars and break words wrongly */
		start = buff.lastIndexOf(QChar('\n'), cursor.selectionStart());
		end = buff.indexOf(QChar('\n'), cursor.selectionEnd());

		if(start < 0)
			start = 0;

		if(end < 0)
			end = buff.length();

		cursor.setPosition(start, QTextCursor::MoveAnchor);
		cursor.setPosition(end, QTextCursor::KeepAnchor);
		lines = cursor.selectedText().split(QChar(QChar::ParagraphSeparator));

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
	QByteArray buff;
	bool loaded = false;

	try
	{
		loaded = GuiUtilsNs::selectAndLoadFile(buff,	tr("Load file"), QFileDialog::ExistingFile,
																					{ tr("SQL file (*.sql)"),	tr("All files (*.*)") });
	}
	catch(Exception &e)
	{
		Messagebox::error(e, __PRETTY_FUNCTION__, __FILE__, __LINE__);
	}

	if(loaded)
	{
		clear();
		setPlainText(buff);
		clear_btn->setEnabled(!document()->isEmpty());
	}
}

void NumberedTextEditor::saveFile()
{
	try
	{
		GuiUtilsNs::selectAndSaveFile(toPlainText().toUtf8(), tr("Save file"),
																	QFileDialog::AnyFile,
																	{ tr("SQL file (*.sql)"),	tr("All files (*.*)") },
																	{}, "sql");
	}
	catch(Exception &e)
	{
		Messagebox::error(e, __PRETTY_FUNCTION__, __FILE__, __LINE__);
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

	buffer.append(this->toPlainText().toUtf8());
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
	clear_btn->setEnabled(!this->document()->isEmpty());
	word_wrap_btn->setEnabled(!this->document()->isEmpty());
	save_file_btn->setEnabled(!this->document()->isEmpty());
	this->setReadOnly(false);
}

void NumberedTextEditor::updateSource(int exit_code, QProcess::ExitStatus)
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
		msg_lbl->setText(UtilsNs::formatMessage(tr("The source editor `%1' is running on `pid: %2'.")
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
	QStringList errors = { src_editor_proc.errorString(),  src_editor_proc.readAllStandardError() };

	Messagebox::error(UtilsNs::formatMessage(tr("Failed to run the source code editor <strong>%1</strong>! Make to sure that the application path points to a valid executable and the current user has permission to run the application. Error message returned: <strong>%2</strong>")
																						.arg(src_editor_proc.program())
																						.arg(errors.join("\n\n"))));

	enableEditor();
}

void NumberedTextEditor::setReadOnly(bool ro)
{
	if(handle_ext_files)
	{
		load_file_btn->setEnabled(!ro);
		edit_src_btn->setEnabled(!ro);
		clear_btn->setEnabled(!ro && !this->document()->isEmpty());
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

	QFont fnt = default_font;

	if(custom_fnt_size)
		fnt.setPointSizeF(custom_fnt_size);

	setFont(fnt);
	line_number_wgt->setFont(fnt);

	QTextBlock block = firstVisibleBlock();
	int block_number = block.blockNumber(),

			//Calculates the first block postion (in widget coordinates)
			top = static_cast<int>(blockBoundingGeometry(block).translated(contentOffset()).top()),

			/* We need to retrieve the minimal height for one line of the block. So, we divide the
			 * total block height by the block line numbers (in case of the block has more than one line, e.g., the
			 * line wrap mode is activated) */
			height = static_cast<int>(blockBoundingRect(block).height()) / block.lineCount(),
			bottom = top + height,
			dy = top;
	unsigned first_line=0, line_count=0;
	double tab_stop_dist = 0;

	// Calculates the visible lines by iterating over the visible/valid text blocks.
	while(block.isValid())
	{
		if(block.isVisible())
		{
			line_count += block.lineCount();

			if(first_line == 0)
				first_line = static_cast<unsigned>(block_number + 1);
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

	line_number_wgt->drawLineNumbers(first_line, line_count, dy, height);
	tab_stop_dist = this->tabStopDistance();

	if(round(tab_stop_dist) != round(NumberedTextEditor::getTabDistance()))
		setTabStopDistance(NumberedTextEditor::getTabDistance());
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

	chr_width = this->fontMetrics().horizontalAdvance(QChar('9'));
	return (15 + chr_width * digits);
}

void NumberedTextEditor::resizeEvent(QResizeEvent *event)
{
	QPlainTextEdit::resizeEvent(event);
	updateLineNumbersSize();
}

void NumberedTextEditor::keyPressEvent(QKeyEvent *event)
{
	if(!isReadOnly() && event->key()==Qt::Key_V && event->modifiers() == (Qt::ControlModifier | Qt::ShiftModifier))
	{
		pasteCode();
	}
	else if(!isReadOnly() && textCursor().hasSelection())
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
