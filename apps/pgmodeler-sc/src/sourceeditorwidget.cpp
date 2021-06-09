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

	connect(find_wgt, SIGNAL(s_hideRequested()), find_tb, SLOT(toggle()));
	connect(validate_tb, SIGNAL(clicked(bool)), this, SLOT(validateSyntax()));
	connect(editor_txt, SIGNAL(modificationChanged(bool)), this, SLOT(restoreEditorPalette()));
	connect(editor_txt, SIGNAL(cursorPositionChanged()), this, SLOT(restoreEditorPalette()));
	connect(find_tb, SIGNAL(toggled(bool)), find_parent, SLOT(setVisible(bool)));
	connect(find_wgt, SIGNAL(toggled(bool)), find_parent, SLOT(setVisible(bool)));
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

void SourceEditorWidget::restoreEditorPalette()
{
	editor_txt->setPalette(def_editor_pal);
}

void SourceEditorWidget::setDefaultEditorPalette(const QPalette &pal)
{
	def_editor_pal = pal;
}

QString SourceEditorWidget::getFilename()
{
	return filename;
}
