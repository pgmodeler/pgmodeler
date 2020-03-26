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

#include "findreplacewidget.h"

FindReplaceWidget::FindReplaceWidget(QPlainTextEdit *txt_edit, QWidget *parent): QWidget(parent)
{
	if(!txt_edit)
		throw Exception(ErrorCode::AsgNotAllocattedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	setupUi(this);
	text_edt=txt_edit;

	next_tb->setToolTip(next_tb->toolTip() + QString(" (%1)").arg(next_tb->shortcut().toString()));
	previous_tb->setToolTip(previous_tb->toolTip() + QString(" (%1)").arg(previous_tb->shortcut().toString()));

	connect(replace_tb, SIGNAL(clicked()), this, SLOT(replaceText()));
	connect(replace_find_tb, SIGNAL(clicked()), this, SLOT(replaceFindText()));
	connect(replace_all_tb, SIGNAL(clicked()), this, SLOT(replaceAll()));

	connect(next_tb, &QToolButton::clicked,
			[&]() { findText(false, true); });

	connect(previous_tb, &QToolButton::clicked,
			[&]() { findText(true, true); });

	connect(find_edt, &QLineEdit::textChanged,
			[&]() { bool enable=!find_edt->text().isEmpty();
		next_tb->setEnabled(enable);
		previous_tb->setEnabled(enable);
		replace_tb->setEnabled(enable);
		replace_all_tb->setEnabled(enable);
		replace_find_tb->setEnabled(enable); });

	//Disabling the regular expression checkbox if the Qt version in use is lower than 5.3
#if (QT_VERSION < QT_VERSION_CHECK(5, 3, 0))
	regexp_chk->setEnabled(false);
	regexp_chk->setChecked(false);
	regexp_chk->setVisible(false);
#endif
}

void FindReplaceWidget::showEvent(QShowEvent *)
{
	find_edt->setFocus();
	replace_btns_parent->setVisible(!text_edt->isReadOnly());
	replace_lbl->setVisible(!text_edt->isReadOnly());
	replace_edt->setVisible(!text_edt->isReadOnly());
}

void FindReplaceWidget::replaceText()
{
	QTextCursor cursor=text_edt->textCursor();

	if(cursor.hasSelection())
	{
		cursor.removeSelectedText();
		cursor.insertText(replace_edt->text());
	}
}

void FindReplaceWidget::replaceAll()
{
	QTextCursor orig_cursor, cursor=text_edt->textCursor();

	orig_cursor=cursor;
	cursor.setPosition(0);
	text_edt->setTextCursor(cursor);

	while(findText(false, false))
		text_edt->textCursor().insertText(replace_edt->text());

	text_edt->setTextCursor(orig_cursor);
}

void FindReplaceWidget::replaceFindText()
{
	if(text_edt->textCursor().hasSelection())
	{
		replaceText();
		findText(false, true);
	}
}

#if (QT_VERSION >= QT_VERSION_CHECK(5, 3, 0))
bool FindReplaceWidget::findText(const QString &text, bool regexp, QTextDocument::FindFlags flags)
#else
bool FindReplaceWidget::findText(const QString &text, bool, QTextDocument::FindFlags flags)
#endif
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 3, 0))
	if(regexp)
		return text_edt->find(QRegExp(text, ((flags & QTextDocument::FindCaseSensitively)==QTextDocument::FindCaseSensitively ?
																					 Qt::CaseSensitive : Qt::CaseInsensitive)), flags);
	else
		return text_edt->find(text, flags);
#else
#warning "Text find through regular expressions is available only in Qt 5.3 or above."
	return text_edt->find(text, flags);
#endif
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

	found=findText(find_edt->text(), regexp_chk->isChecked(), flags);

	if(!found && cyclic)
	{
		cursor=text_edt->textCursor();

		if(!backward)
			cursor.setPosition(0);
		else
			cursor.setPosition(text_edt->toPlainText().length());

		text_edt->setTextCursor(cursor);

		found=findText(find_edt->text(), regexp_chk->isChecked(), flags);
	}

	return found;
}
