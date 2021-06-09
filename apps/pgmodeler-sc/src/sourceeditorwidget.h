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

	public:
		explicit SourceEditorWidget(QWidget *parent = nullptr);

		static void setDefaultEditorPalette(const QPalette &pal);

		QString getFilename();

	public slots:
		void loadFile(const QString &filename);
		void validateSyntax();
		void restoreEditorPalette();
		void saveFile(const QString &filename);
		void loadSyntaxConfig(const QString &filename);
};

#endif
