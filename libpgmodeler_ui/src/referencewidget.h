/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2018 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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

#ifndef REFERENCE_WIDGET_H
#define REFERENCE_WIDGET_H

#include <QWidget>
#include "ui_referencewidget.h"
#include "numberedtexteditor.h"
#include "syntaxhighlighter.h"
#include "codecompletionwidget.h"
#include "objectselectorwidget.h"
#include "hinttextwidget.h"

class ReferenceWidget : public QWidget, Ui::ReferenceWidget {
	private:
		Q_OBJECT

		HintTextWidget *ref_alias_ht, *used_in_ht, *ref_object_ht;

		NumberedTextEditor *expression_txt;

		SyntaxHighlighter *expression_hl;

		CodeCompletionWidget *expression_cp;

		ObjectSelectorWidget *ref_object_sel;

		unsigned ref_flags;

		Reference reference;

	public:
		explicit ReferenceWidget(QWidget *parent = 0);

		void setAttributes(Reference ref, unsigned ref_flags, DatabaseModel *model);

		Reference getReference(void);

		unsigned getReferenceFlags(void);

	public slots:
		void applyConfiguration(void);

	private slots:
		void selectReferenceType(void);

	signals:
		void s_closeRequested(void);
};

#endif
