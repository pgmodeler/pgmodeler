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
#include <QList>

class FragmentInfo {
	private:
		int fmt_start, fmt_length;

		bool persistent,
				 multi_expr,
				 closed,
				allow_completion;

		QString group;

	public:
	FragmentInfo()
	{
		persistent = multi_expr =
				closed = allow_completion = false;
		fmt_start = fmt_length = -1;
	}

	FragmentInfo(const QString &grp, int start, int length, bool persist, bool closed, bool allow_compl)
	{
		group = grp;
		fmt_start = start;
		fmt_length = length;
		persistent = persist;
		this->closed = closed;
		allow_completion = allow_compl;
	}

	int getStart() const
	{
		return fmt_start;
	}

	int getLength() const
	{
		return fmt_length;
	}

	QString getGroup() const
	{
		return group;
	}

	bool isClosed() const
	{
		return closed;
	}

	bool isAllowCompletion() const
	{
		return allow_completion;
	}

	bool isPersistent() const
	{
		return persistent;
	}

	bool isValid() const
	{
		return !group.isEmpty() &&
					 fmt_start >= 0 && fmt_length > 0;
	}

	friend class TextBlockInfo;
};

class  __libgui TextBlockInfo: public QTextBlockUserData {
	private:
		QList<FragmentInfo> frag_infos;

		//! \brief The name of the group that holds the formatting applied to the block
		QString group [[deprecated]] ;

		//! \brief Indicates if the block is related to a group that contains initial and final expression (multi lined expressions)
		bool multi_expr [[deprecated]] ,

		//! \brief Indicates if the block is closed (only for multi expression groups)
		closed [[deprecated]] ,

		//! \brief Indicates if the block was closed at least one time.
		closed_once [[deprecated]] ,

		//! \brief Indicates that, when available, the code completion can be triggered when the cursor is in this block info.
		allow_completion [[deprecated]],

		//! \brief Indicates that all words in the line of this block must receive the format of the this->group
		entire_line [[deprecated]];

	public:
		TextBlockInfo();

		//! \brief Clears the group name and set all flags to false
		void reset();

		void addFragmentInfo(const FragmentInfo &f_info);

		FragmentInfo getFragmentInfo(int start, int length, bool only_persist);

		[[deprecated]]
		void setGroup(const QString &grp);

		[[deprecated]]
		void setClosed(bool value);

		[[deprecated]]
		void setMultiExpr(bool value);

		[[deprecated]]
		void setAllowCompletion(bool value);

		[[deprecated]]
		void setEntireLine(bool value);

		[[deprecated]]
		QString getGroup();

		[[deprecated]]
		bool isMultiExpr();

		[[deprecated]]
		bool isClosedOnce();

		[[deprecated]]
		bool isClosed();

		[[deprecated]]
		bool isCompletionAllowed();

		[[deprecated]]
		bool isEntireLine();
};

#endif
