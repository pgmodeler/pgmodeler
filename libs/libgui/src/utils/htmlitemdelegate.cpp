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

#include "htmlitemdelegate.h"
#include <QTextDocument>
#include <QLineEdit>

const QRegularExpression HtmlItemDelegate::TagRegExp("<(\\/)?[a-z]+(>|\\/>)");

HtmlItemDelegate::HtmlItemDelegate(QObject *parent, bool ignore_tags_sz_hint) : PlainTextItemDelegate(parent, true)
{
	this->ignore_tags_sz_hint = ignore_tags_sz_hint;
}

HtmlItemDelegate::~HtmlItemDelegate()
{

}

QSize HtmlItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	if(ignore_tags_sz_hint)
	{
		QString text = index.data().toString();
		QSize sz = PlainTextItemDelegate::sizeHint(option, index);

		if(text.contains(TagRegExp))
		{
			/* When ignoring html tags in size hint calculation we erase all tags from
			 * the text and calculates the new size */
			text.remove(TagRegExp);
			sz.setWidth(option.fontMetrics.boundingRect(text).width() + option.decorationSize.width() * 2);
		}

		return sz;
	}

	return PlainTextItemDelegate::sizeHint(option, index);
}

void HtmlItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QString text=index.data().toString();

	painter->save();
	QStyledItemDelegate::paint(painter, option, index);

	if(text.contains(TagRegExp))
	{
		static QTextDocument doc;
		static QRect rect;
		static QColor bg_color;
		static int dy = 0;

		text.replace("\n", "<br/>");
		rect.setTop(option.rect.top());
		rect.setLeft(option.rect.left() + option.decorationSize.width() + 5);
		rect.setSize(option.rect.size());

		//Painting the correct background color according to the item state
		if((option.state & QStyle::State_Selected) == QStyle::State_Selected)
			//Selected
			bg_color=option.palette.color(QPalette::Highlight);
		else if(option.features==QStyleOptionViewItem::Alternate)
			//Alternate color
			bg_color=option.palette.color(QPalette::AlternateBase);
		else
			//Base color
			bg_color=option.palette.color(QPalette::Base);

		//Repaint the text area
		painter->fillRect(rect, bg_color);

		//Set the text to a html document instance and draw it to the painter
		doc.setHtml(text);
		dy = abs(option.rect.height() - option.decorationSize.height());
		painter->translate(rect.topLeft() - QPoint(0, dy));
		doc.drawContents(painter);
	}

	painter->restore();
}
