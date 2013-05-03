#ifndef XML2OBJECT_WIDGET_H
#define XML2OBJECT_WIDGET_H

#include "ui_xml2objectwidget.h"
#include "syntaxhighlighter.h"

class Xml2ObjectWidget: public QDialog, public Ui::Xml2ObjectWidget {
	private:
		Q_OBJECT

		SyntaxHighlighter *code_hl;

	public:
		Xml2ObjectWidget(QWidget *parent = 0, Qt::WindowFlags f = 0);
};

#endif
