/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2014 - Raphael Araújo e Silva <raphael@pgmodeler.com.br>
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

#include "findreplacewidget.h"

FindReplaceWidget::FindReplaceWidget(QTextEdit *txt_edit, QWidget *parent): QWidget(parent)
{
	if(!txt_edit)
		throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	setupUi(this);
	text_edt=txt_edit;

	next_tb->setToolTip(next_tb->toolTip() + QString(" (%1)").arg(next_tb->shortcut().toString()));
	previous_tb->setToolTip(previous_tb->toolTip() + QString(" (%1)").arg(previous_tb->shortcut().toString()));

	connect(replace_tb, SIGNAL(clicked()), this, SLOT(replaceText()));
	connect(replace_find_tb, SIGNAL(clicked()), this, SLOT(replaceFindText()));
	connect(replace_all_tb, SIGNAL(clicked()), this, SLOT(replaceAll()));

	connect(next_tb, &QToolButton::clicked,
					[=]() { findText(false, true); });

	connect(previous_tb, &QToolButton::clicked,
					[=]() { findText(true, true); });

	connect(find_edt, &QLineEdit::textChanged,
					[=]() { bool enable=!find_edt->text().isEmpty();
									next_tb->setEnabled(enable);
									previous_tb->setEnabled(enable);
									replace_tb->setEnabled(enable);
									replace_all_tb->setEnabled(enable);
									replace_find_tb->setEnabled(enable); });
}

void FindReplaceWidget::showEvent(QShowEvent *)
{
	find_edt->setFocus();
}

void FindReplaceWidget::replaceText(void)
{
	QTextCursor cursor=text_edt->textCursor();

	if(cursor.hasSelection())
	{
		cursor.removeSelectedText();
		cursor.insertText(replace_edt->text());
	}
}

void FindReplaceWidget::replaceAll(void)
{
	QTextCursor orig_cursor, cursor=text_edt->textCursor();

	orig_cursor=cursor;
	cursor.setPosition(0);
	text_edt->setTextCursor(cursor);

	while(findText(false, false))
		text_edt->textCursor().insertText(replace_edt->text());

	text_edt->setTextCursor(orig_cursor);
}

void FindReplaceWidget::replaceFindText(void)
{
	if(text_edt->textCursor().hasSelection())
	{
		replaceText();
		findText(false, true);
	}
}

bool FindReplaceWidget::findText(bool backward, bool cyclic)
{
	QTextDocument::FindFlags flags;
	QTextCursor cursor;
	bool found=false;

	if(backward)
		flags=QTextDocument::FindBackward;

	if(case_sensitive_chk->isChecked())
		flags=flags | QTextDocument::FindCaseSensitively;

	if(all_words_chk->isChecked())
		flags=flags | QTextDocument::FindWholeWords;

	if(regexp_chk->isChecked())
		found=text_edt->find(QRegExp(find_edt->text()), flags);
	else
		found=text_edt->find(find_edt->text(), flags);

	if(!found && cyclic)
	{
		cursor=text_edt->textCursor();

		if(!backward)
			cursor.setPosition(0);
		else
			cursor.setPosition(text_edt->toPlainText().length());

		text_edt->setTextCursor(cursor);

		if(regexp_chk->isChecked())
			found=text_edt->find(QRegExp(find_edt->text()), flags);
		else
			found=text_edt->find(find_edt->text(), flags);
	}

	return(found);
}
