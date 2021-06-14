#include "sourceeditorwidget.h"
#include "messagebox.h"
#include "pgmodeleruins.h"
#include "qtcompat/splitbehaviorcompat.h"

QPalette SourceEditorWidget::def_editor_pal;

SourceEditorWidget::SourceEditorWidget(QWidget *parent) : QWidget(parent)
{
	setupUi(this);

	is_modified = false;

	editor_txt = PgModelerUiNs::createNumberedTextEditor(editor_parent);
	def_editor_pal = editor_txt->palette();

	editor_hl = new SyntaxHighlighter(editor_txt);
	find_wgt = new FindReplaceWidget(editor_txt, find_parent);
	find_parent->setVisible(false);

	QVBoxLayout *vbox = new QVBoxLayout(find_parent);
	vbox->setContentsMargins(0, 0, 0, 4);
	vbox->addWidget(find_wgt);

	indent_tb->setMenu(&indent_opts_menu);
	act_break_inline_ifs = indent_opts_menu.addAction(tr("Break inline ifs"));
	act_break_inline_ifs->setCheckable(true);
	act_break_inline_ifs->setChecked(true);

	connect(find_wgt, SIGNAL(s_hideRequested()), find_tb, SLOT(toggle()));
	connect(validate_tb, SIGNAL(clicked(bool)), this, SLOT(validateSyntax()));
	connect(indent_tb, SIGNAL(clicked(bool)), this, SLOT(	applyIndentation()));
	connect(editor_txt, SIGNAL(modificationChanged(bool)), this, SLOT(restoreEditorPalette()));
	connect(editor_txt, SIGNAL(undoAvailable(bool)), this, SLOT(setModified(bool)));
	connect(editor_txt, SIGNAL(cursorPositionChanged()), this, SLOT(restoreEditorPalette()));
	connect(find_tb, SIGNAL(toggled(bool)), find_parent, SLOT(setVisible(bool)));
}

void SourceEditorWidget::saveFile(const QString &filename)
{
	QFile input;

	input.setFileName(filename);
	input.open(QFile::WriteOnly);

	if(!input.isOpen())
	{
		throw Exception(Exception::getErrorMessage(ErrorCode::FileDirectoryNotAccessed).arg(filename),
										ErrorCode::FileDirectoryNotAccessed, __PRETTY_FUNCTION__, __FILE__, __LINE__);
	}

	input.write(editor_txt->toPlainText().toUtf8());
	input.close();

	QFileInfo fi(filename);
	validate_tb->setEnabled(filename.endsWith(GlobalAttributes::SchemaExt));
	indent_tb->setEnabled(filename.endsWith(GlobalAttributes::SchemaExt));
	this->filename = filename;
}

void SourceEditorWidget::loadSyntaxConfig(const QString &filename)
{
	try
	{
		editor_hl->loadConfiguration(filename);
		editor_hl->rehighlight();
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(), __PRETTY_FUNCTION__, __FILE__, __LINE__, &e);
	}
}

void SourceEditorWidget::loadFile(const QString &filename)
{
	if(filename.isEmpty())
		return;

	QFile input;
	input.setFileName(filename);
	input.open(QFile::ReadOnly);

	if(!input.isOpen())
	{
		throw Exception(Exception::getErrorMessage(ErrorCode::FileDirectoryNotAccessed).arg(filename),
										ErrorCode::FileDirectoryNotAccessed, __PRETTY_FUNCTION__, __FILE__, __LINE__);
	}

	editor_txt->setPlainText(input.readAll());
	validate_tb->setEnabled(filename.endsWith(GlobalAttributes::SchemaExt));
	indent_tb->setEnabled(filename.endsWith(GlobalAttributes::SchemaExt));
	this->filename = filename;
	input.close();
}

void SourceEditorWidget::validateSyntax()
{
	SchemaParser schparser;
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
		cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, schparser.getCurrentColumn());

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

void SourceEditorWidget::restoreEditorPalette()
{
	editor_txt->setPalette(def_editor_pal);
}

void SourceEditorWidget::applyIndentation()
{
	QStringList buffer = editor_txt->toPlainText().split(QChar::LineFeed);
	int if_level = 0, comment_pos = -1, line_count = buffer.size();
	bool found_cond = false, found_if = false;
	QString cond_pattern = QString("^(( )|(\\t))*(%1)"), line,
			tk_if = SchemaParser::CharStartConditional + SchemaParser::TokenIf,
			tk_then = SchemaParser::CharStartConditional + SchemaParser::TokenThen,
			tk_else = SchemaParser::CharStartConditional + SchemaParser::TokenElse,
			tk_end = SchemaParser::CharStartConditional + SchemaParser::TokenEnd;

	for(int ln_idx = 0; ln_idx < line_count; ln_idx++)
	{
		line = buffer[ln_idx];
		comment_pos = line.indexOf(SchemaParser::CharComment);

		if(line.contains(QRegExp(cond_pattern.arg(tk_if))))
		{
			if_level++;
			found_if = found_cond = true;
		}
		else if(line.contains(QRegExp(cond_pattern.arg(tk_else))) ||
						line.contains(QRegExp(cond_pattern.arg(tk_end))))
			found_cond = true;

		if(act_break_inline_ifs->isChecked() && line.contains(QRegExp(QString("(%1)(.)+(%2)").arg(tk_if).arg(tk_end))))
		{
			line.replace(tk_if, QChar::LineFeed + tk_if);
			line.replace(tk_then, tk_then + QChar::LineFeed);
			line.replace(tk_else, QChar::LineFeed + tk_else + QChar::LineFeed);
			line.replace(tk_end, QChar::LineFeed + tk_end + QChar::LineFeed );

			if(line.contains(QChar::LineFeed))
			{
				QStringList buf_aux = line.split(QChar::LineFeed, QtCompat::SkipEmptyParts);
				buffer.removeAt(ln_idx);

				for(auto itr = buf_aux.rbegin(); itr != buf_aux.rend(); itr++)
					buffer.insert(ln_idx, *itr);

				ln_idx = 0;
				found_cond = found_if = false;
				if_level = 0;
				line_count = buffer.size();
				continue;
			}
		}

		line = line.simplified();

		if(!line.isEmpty())
		{
			line = line.rightJustified(line.size() + if_level + (found_cond ? -1 : 0), QChar::Tabulation);
			if_level -= line.mid(0, comment_pos).count(QString("%1%2").arg(SchemaParser::CharStartConditional).arg(SchemaParser::TokenEnd));
		}

		buffer[ln_idx] = line;
		found_cond = found_if = false;
	}

	QRegExp cond_tk_regexp(QString("(%1)[a-z]+").arg(SchemaParser::CharStartConditional));
	QString prev_line, next_line,
			tk_set = SchemaParser::CharStartConditional + SchemaParser::TokenSet,
			tk_unset = SchemaParser::CharStartConditional + SchemaParser::TokenUnset;

	buffer.removeAll("");

	for(int ln_idx = 0; ln_idx < buffer.count() - 1; ln_idx++)
	{
		// Capturing the previous, current and next lines without comment portion
		prev_line = ln_idx > 0 ? buffer[ln_idx - 1].mid(0, buffer[ln_idx - 1].indexOf(SchemaParser::CharComment)) : "";
		line = buffer[ln_idx].mid(0, buffer[ln_idx].indexOf(SchemaParser::CharComment));
		next_line = ln_idx < buffer.count() - 1 ? buffer[ln_idx + 1].mid(0, buffer[ln_idx + 1].indexOf(SchemaParser::CharComment)) : "";

		// Separating an end token from any conditional token in the next line
		if(line.contains(tk_end) &&
			 !next_line.isEmpty() &&
			 !next_line.contains(cond_tk_regexp))
			buffer[ln_idx].append(QChar::LineFeed);

		// Separating an closed plain text from the next line
		else if(line.endsWith(SchemaParser::CharEndPlainText) &&
						!next_line.isEmpty() &&
						!next_line.contains(cond_tk_regexp))
			buffer[ln_idx].append(QChar::LineFeed);

		// If the current line has an %if and the previous is not a conditional instruction %
		else if(line.contains(tk_if) &&
						!prev_line.isEmpty() &&
						!prev_line.contains(cond_tk_regexp))
			buffer[ln_idx].prepend(QChar::LineFeed);

		// Separating an if token from previous end, set and unset
		else if(line.contains(tk_if) &&
						(prev_line.contains(tk_end) ||
						 prev_line.contains(tk_set) ||
						 prev_line.contains(tk_unset)))
			buffer[ln_idx].prepend(QChar::LineFeed);
	}

	/* Replacing the current code with the formatted one in such a way to preserve
	 * the undo/redo stack. This is achieved by selecting the whole text document
	 * and inserting the new text using the cursor. Using QPlainTextEdit::setPlainText
	 * will clear the stack. */
	QTextCursor cursor = editor_txt->textCursor();
	cursor.movePosition(QTextCursor::Start);
	cursor.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
	cursor.insertText(buffer.join(QChar::LineFeed));
	cursor = editor_txt->textCursor();
	editor_txt->moveCursor(QTextCursor::Start);
}

void SourceEditorWidget::setModified(bool value)
{
	is_modified = value;
	emit s_editorModified(value);
}

void SourceEditorWidget::setDefaultEditorPalette(const QPalette &pal)
{
	def_editor_pal = pal;
}

QString SourceEditorWidget::getFilename()
{
	return filename;
}

bool SourceEditorWidget::isModified()
{
	return is_modified;
}
