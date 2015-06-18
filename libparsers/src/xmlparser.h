/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2015 - Raphael Araújo e Silva <raphael@pgmodeler.com.br>
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
\ingroup libparsers
\class XMLParser
\brief This class implements basic operations of a xml analyzer encapsulating some functions implemented by libxml2 library
\note <strong>Creation date:</strong> 02/04/2008
*/

#ifndef XML_PARSER_H
#define XML_PARSER_H

#include <libxml/parser.h>
#include <libxml/tree.h>
#include "schemaparser.h"
#include "exception.h"
#include <stack>
#include <iostream>
#include "attribsmap.h"

class XMLParser {
	private:
		/*! \brief Stores the name of the file that generated the xml buffer when
		 loadXMLFile() method is called */
		QString xml_doc_filename;

		//! \brief Stores the xml document (element tree) generated after the buffer reading
		xmlDoc *xml_doc;

		//! \brief Stores the reference to the root element of the element tree
		xmlNode	*root_elem,
										//! \brief Stores the current element that parser is analyzing
										*curr_elem;

		/*! \brief Stores the elements that marks the position in the tree before do
		 a subsequent operation. To configure this element it is necessary
		 call the method savePosition() and to return the navigation to the saved
		 position is necessary call restorePosition() */
		stack<xmlNode *> elems_stack;

		//! \brief Stores the document DTD declaration
		QString	dtd_decl,
										//! \brief Stores XML document to be analyzed
										xml_buffer,
										/*! \brief Stores the declaration <?xml?>. If this isn't exists it will be
										 a default declaration. */
										xml_decl;

		/*! \brief Remove the original DTD from the document. This is done to evit that
		 the user insert some external dtd in the model file that is not valid for pgModeler */
		void removeDTD(void);

		/*! \brief Makes the interpretation of XML inside the buffer validating it according to
		 DTD defined configured (by the parser) to the buffer. Initializes
		 the necessary attributes to make possible the navigation through the element tree
		 generated from the XML document read. */
		void readBuffer(void);

	public:
		//! \brief Constants used to referência the elements on the element tree
    static const unsigned ROOT_ELEMENT=0,
															CHILD_ELEMENT=1,
															NEXT_ELEMENT=2,
															PREVIOUS_ELEMENT=3;

		static const QString CHAR_AMP; //! \brief & = &amp;
		static const QString CHAR_LT; //! \brief  < = &lt;
		static const QString CHAR_GT; //! \brief  < = &gt;
		static const QString CHAR_QUOT; //! \brief  < = &quot;
		static const QString CHAR_APOS; //! \brief  < = &apos;

		XMLParser(void);
		~XMLParser(void);

		//! \brief Loads the XML buffer from a file
		void loadXMLFile(const QString &filename);

		//! \brief Loads the XML buffer from a string
		void loadXMLBuffer(const QString &xml_buf);

		//! \brief Informs the DTD file used to make element validations
		void setDTDFile(const QString &dtd_file, const QString &dtd_name);

		//! \brief Saves to stack the current navigation position on the element tree
		void savePosition(void);

		//! \brief Restores the previous navigation position oh the element tree
		void restorePosition(void);

		/*! \brief Restores the position of the navigation on a specific
		 element on the document. The navigation stack is always
		 emptied when this method is called */
		void restorePosition(const xmlNode *elem);

		/*! \brief Moves one level in the element tree according to the type of element
		 to be accessed. Returns true if the position was moved to the
		 desired element. */
		bool accessElement(unsigned elem_type);

		//! \brief Returns if an element has a root, child, previous or next element
    bool hasElement(unsigned elem_type, xmlElementType xml_node_type=static_cast<xmlElementType>(0));

		//! \brief Retorns if an element has attributes
		bool hasAttributes(void);

		//! \brief Stores on a map the atrributes (names and values) of the current element
		void getElementAttributes(attribs_map &attributes);

		/*! \brief Returns the content text of the element, used only for elements which do not have children
		 and that are filled by simple texts */
		QString getElementContent(void);

		//! \brief Returns the current element type
		xmlElementType getElementType(void);

		//! \brief Returns the constant reference to the current element on the tree
		const xmlNode *getCurrentElement(void);

		//! \brief Returns the current line number on the buffer that is being processed
		int getCurrentBufferLine(void);

		//! \brief Returns the total line amount of the buffer
		int getBufferLineCount(void);

		//! \brief Returns the tag name that defines the current element
		QString getElementName(void);

		//! \brief Returns the filename that generated XML buffer
		QString getLoadedFilename(void);

		//! \brief Returns the full parser buffer
		QString getXMLBuffer(void);

		//! \brief Reset all the elements resposible to the navigation through the element tree
		void restartNavigation(void);

		/*! \brief Reset all the parser attributes, deallocating the element tree. The user have to
		 reload the file to analyze it again */
		void restartParser(void);
};

#endif
