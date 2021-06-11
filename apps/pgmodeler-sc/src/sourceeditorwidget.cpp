#include "sourceeditorwidget.h"
#include "messagebox.h"
#include "pgmodeleruins.h"

QPalette SourceEditorWidget::def_editor_pal;

SourceEditorWidget::SourceEditorWidget(QWidget *parent) : QWidget(parent)
{
	setupUi(this);

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

	act_preserve_comments = indent_opts_menu.addAction(tr("Preserve comments formatting"));
	act_preserve_comments->setCheckable(true);
	act_preserve_comments->setChecked(true);

	connect(find_wgt, SIGNAL(s_hideRequested()), find_tb, SLOT(toggle()));
	connect(validate_tb, SIGNAL(clicked(bool)), this, SLOT(validateSyntax()));
	connect(indent_tb, SIGNAL(clicked(bool)), this, SLOT(	applyIndentation()));
	connect(editor_txt, SIGNAL(modificationChanged(bool)), this, SLOT(restoreEditorPalette()));
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
	int if_level = 0, comment_pos = -1, line_count = buffer.size(), tk_pos = -1;
	bool found_cond = false;
	QString cond_pattern = QString("^(( )|(\\t))*(%1)"), line,
			tk_if = SchemaParser::CharStartConditional + SchemaParser::TokenIf,
			tk_then = SchemaParser::CharStartConditional + SchemaParser::TokenThen,
			tk_else = SchemaParser::CharStartConditional + SchemaParser::TokenElse,
			tk_end = SchemaParser::CharStartConditional + SchemaParser::TokenEnd;

	for(int ln_idx = 0; ln_idx < line_count; ln_idx++)
	{
		line = buffer[ln_idx];

		if(line.contains(QRegExp(cond_pattern.arg(tk_if))))
		{
			if_level++;
			found_cond = true;
		}
		else if(line.contains(QRegExp(cond_pattern.arg(tk_else))) ||
						line.contains(QRegExp(cond_pattern.arg(tk_end))))
			found_cond = true;

		if(act_break_inline_ifs->isChecked() && line.contains(QRegExp(QString("(%1)(.)+(%2)").arg(tk_if).arg(tk_end))))
		{
			tk_pos = line.indexOf(tk_if);
			if(!line.contains(QRegExp(cond_pattern.arg(tk_if))) && tk_pos > 0)
				line.insert(tk_pos, QString().fill(QChar::LineFeed, 2));

			tk_pos = line.indexOf(tk_then);
			if(tk_pos > 0 && (tk_pos + tk_then.size() < line.size()))
				line.insert(tk_pos + tk_then.size(), QChar::LineFeed);

			tk_pos = line.indexOf(tk_else);
			if(!line.contains(QRegExp(cond_pattern.arg(tk_else))) && tk_pos > 0)
			{
				line.insert(tk_pos, QChar::LineFeed);
				line.insert(tk_pos + tk_else.size() + 1, QChar::LineFeed);
			}

			tk_pos = line.indexOf(tk_end);
			if(!line.contains(QRegExp(cond_pattern.arg(tk_end))) && tk_pos > 0)
			{
				line.insert(tk_pos, QChar::LineFeed);
				line.insert(tk_pos + tk_end.size() + 1, QChar::LineFeed);
			}

			if(line.contains(QChar::LineFeed))
			{
				QStringList buf_aux = line.split(QChar::LineFeed);
				buffer.removeAt(ln_idx);

				for(auto itr = buf_aux.rbegin(); itr != buf_aux.rend(); itr++)
					buffer.insert(ln_idx, *itr);

				ln_idx--;
				line_count = buffer.size();
				continue;
			}
		}

		comment_pos = line.indexOf(SchemaParser::CharComment);

		if(comment_pos >= 0 && act_preserve_comments->isChecked())
		{
			QString comment = line.mid(comment_pos);

			if(line.size() != comment.size())
			{
				line = line.mid(0, comment_pos).simplified();
				line += " " + comment;
			}
		}
		else
			line = line.simplified();

		if(!line.isEmpty())
		{
			line = line.rightJustified(line.size() + if_level + (found_cond ? -1 : 0), '\t');
			if_level -= line.mid(0, comment_pos).count(QString("%1%2").arg(SchemaParser::CharStartConditional).arg(SchemaParser::TokenEnd));
		}

		buffer[ln_idx] = line;
		found_cond = false;
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

void SourceEditorWidget::setDefaultEditorPalette(const QPalette &pal)
{
	def_editor_pal = pal;
}

QString SourceEditorWidget::getFilename()
{
	return filename;
}
