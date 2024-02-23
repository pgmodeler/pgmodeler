/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2024 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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
\ingroup libgui
\class TextBlockInfo
\brief Auxiliary class used by SyntaxHighlight and store highlighting states of words in a document.
*/

#ifndef TEXT_BLOCK_INFO_H
#define TEXT_BLOCK_INFO_H

#include "guiglobal.h"
#include <QString>
#include <QTextBlockUserData>

class __libgui TextBlockInfo: public QTextBlockUserData {
	private:
		//! \brief The name of the group that holds the formatting applied to the block
		QString group;

		//! \brief Indicates if the block is related to a group that contains initial and final expression (multi lined expressions)
		bool is_multi_expr,

		//! \brief Indicates if the block is closed (only for multi expression groups)
		is_closed,

		//! \brief Indicates if the block was closed at least one time.
		closed_once,

		//! \brief Indicates that, when available, the code completion can be triggered when the cursor is in this block info.
		allow_completion;

	public:
		TextBlockInfo(void);

		//! \brief Clears the group name and set all flags to false
		void reset(void);

		void setGroup(const QString &grp);
		void setClosed(bool value);
		void setMultiExpr(bool value);
		void setAllowCompletion(bool value);

		QString getGroup();
		bool isMultiExpr();
		bool isClosedOnce();
		bool isClosed();
		bool isCompletionAllowed();
};

#endif
