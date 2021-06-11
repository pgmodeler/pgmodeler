#ifndef SOURCEEDITORWIDGET_H
#define SOURCEEDITORWIDGET_H

#include <QWidget>
#include "ui_sourceeditorwidget.h"
#include "numberedtexteditor.h"
#include "syntaxhighlighter.h"
#include "findreplacewidget.h"

class SourceEditorWidget: public QWidget, public Ui::SourceEditorWidget {
	private:
		Q_OBJECT

		static QPalette def_editor_pal;

		NumberedTextEditor *editor_txt;

		SyntaxHighlighter *editor_hl;

		FindReplaceWidget *find_wgt;

		QString filename;

		QAction *act_break_inline_ifs, *act_preserve_comments;

		QMenu indent_opts_menu;

	public:
		explicit SourceEditorWidget(QWidget *parent = nullptr);

		static void setDefaultEditorPalette(const QPalette &pal);

		QString getFilename();

	private slots:
		void validateSyntax();
		void restoreEditorPalette();
		void applyIndentation();

	public slots:
		void loadFile(const QString &filename);
		void saveFile(const QString &filename);
		void loadSyntaxConfig(const QString &filename);
};

#endif
