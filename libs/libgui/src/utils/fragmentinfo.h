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
\class FragmentInfo
\brief Auxiliary class used by TextBlockInfo and SyntaxHighlight that holds text fragments format info and other
useful information that helps in syntax highlighting process in a document.
*/

#ifndef FRAGMENT_INFO_H
#define FRAGMENT_INFO_H

#include "guiglobal.h"
#include <QString>

class	__libgui	FragmentInfo {
	private:
		//! \brief The name of the group that formatted the fragment
		QString group;

		//! \brief Position in the text block where the fragment starts
		int start,

		//! \brief Position in the text block where the fragment ends
		end;

		//! \brief Indicates if the fragment holds a persistent highlighting
		bool persistent,

		//! \brief Indicates if the fragment is open (only for multiline expressions)
		open,

		//! \brief Indicates if the fragment is closed (only for multiline expressions)
		closed,

		/*! \brief Indicates if the fragment accepts code completion when the cursor is
		 *  between the start and end positions */
		allow_completion;

	public:
		FragmentInfo();

		FragmentInfo(const QString &grp, int start, int end, bool persist,
								 bool open, bool closed, bool allow_compl);

		QString getGroup() const;
		int getStart() const;
		int getEnd() const;
		bool isOpen() const;
		bool isClosed() const;
		bool isPersistent() const;
		bool isValid() const;

		/*! \brief Returns if the fragment accepts the code completion to be
		 *  displayed. If pos >= 0, besides the allow_completion state, the
		 *  method will also validate if the position is inside the fragment
		 *  boundaries, returning true in positive case. */
		bool isCompletionAllowed(int pos = -1) const;
};

#endif
