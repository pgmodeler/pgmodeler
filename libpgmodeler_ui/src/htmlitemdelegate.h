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

/**
\ingroup libpgmodeler_ui
\class HtmlItemDelegate
\brief Implements a custom item delegate used in treewidget to paint items that contains html as text
*/

#ifndef HTML_ITEM_DELEGATE_H
#define HTML_ITEM_DELEGATE_H

#include <QObject>
#include <QPainter>
#include "plaintextitemdelegate.h"

class HtmlItemDelegate : public PlainTextItemDelegate {
	private:
		Q_OBJECT

	public:
		HtmlItemDelegate(QObject * parent = nullptr);
		virtual ~HtmlItemDelegate();

	protected:
		virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif
