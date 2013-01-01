/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Parsers library (libparsers)
# Classe: XMLParser
# Description: This class implements basic operations of an xml analyzer
#              encapsulating some functions implemented by libxml2 library
# Creation date: 02/04/2008
#
# Copyright 2006-2013 - Raphael Araújo e Silva <rkhaotix@gmail.com>
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
#ifndef XML_PARSER_H
#define XML_PARSER_H

#include <libxml/parser.h>
#include <libxml/tree.h>
#include "schemaparser.h"
#include "exception.h"
#include <fstream>
#include <iostream>
#include <map>
#include <stack>
using namespace std;

class XMLParser {
 private:
  /* Stores the name of the file that generated the xml buffer when
     loadXMLFile() method is called */
  static QString xml_doc_filename;

  //Stores the xml document (element tree) generated after the buffer reading
  static xmlDoc *xml_doc;

  //Stores the reference to the root element of the element tree
  static xmlNode *root_elem,
                 //Stores the current element that parser is analyzing
                 *curr_elem;

  /* Stores the elements that marks the position in the tree before do
     a subsequent operation. To configure this element it is necessary
     call the method savePosition() and to return the navigation to the saved
     position is necessary call restorePosition() */
  static stack<xmlNode *> elems_stack;

  //Stores the document DTD declaration
  static QString dtd_decl,
                //Stores XML document to be analyzed
                xml_buffer,
                /* Stores the declaration <?xml?>. If this isn't exists it will be
                   a default declaration. */
                xml_decl;

  /* Remove the original DTD from the document. This is done to evit that
     the user insert some external dtd in the model file that is not valid for pgModeler */
  static void removeDTD(void);

  /* Makes the interpretation of XML inside the buffer validating it according to
     DTD defined configured (by the parser) to the buffer. Initializes
     the necessary attributes to make possible the navigation through the element tree
     generated from the XML document read. */
  static void readBuffer(void);

 public:
  //Constants used to referência the elements on the element tree
  static const unsigned ROOT_ELEMENT=0,
                        CHILD_ELEMENT=1,
                        NEXT_ELEMENT=2,
                        PREVIOUS_ELEMENT=3;

  static const QString CHAR_AMP; //& = &amp;
  static const QString CHAR_LT; // < = &lt;
  static const QString CHAR_GT; // < = &gt;
  static const QString CHAR_QUOT; // < = &quot;
  static const QString CHAR_APOS; // < = &apos;

  XMLParser(void);
  ~XMLParser(void);

  //Loads the XML buffer from a file
  static void loadXMLFile(const QString &filename);

  //Loads the XML buffer from a string
  static void loadXMLBuffer(const QString &xml_buf);

  //Informs the DTD file used to make element validations
  static void setDTDFile(const QString &dtd_file, const QString &dtd_name);

  //Saves to stack the current navigation position on the element tree
  static void savePosition(void);

  //Restores the previous navigation position oh the element tree
  static void restorePosition(void);

  /* Restores the position of the navigation on a specific
     element on the document. The navigation stack is always
     emptied when this method is called */
  static void restorePosition(const xmlNode *elem);

  /* Moves one level in the element tree according to the type of element
     to be accessed. Returns true if the position was moved to the
     desired element. */
  static bool accessElement(unsigned elem_type);

  //Returns if an element has a root, child, previous or next element
  static bool hasElement(unsigned elem_type);

  //Retorns if an element has attributes
  static bool hasAttributes(void);

  //Stores on a map the atrributes (names and values) of the current element
  static void getElementAttributes(map<QString, QString> &attributes);

  /* Returns the content text of the element, used only for elements which do not have children
     and that are filled by simple texts */
  static QString getElementContent(void);

  //Returns the current element type
  static xmlElementType getElementType(void);

  //Returns the constant reference to the current element on the tree
  static const xmlNode *getCurrentElement(void);

  //Returns the current line number on the buffer that is being processed
  static int getCurrentBufferLine(void);

  //Returns the total line amount of the buffer
  static int getBufferLineCount(void);

  //Returns the tag name that defines the current element
  static QString getElementName(void);

  //Returns the filename that generated XML buffer
  static QString getLoadedFilename(void);

  //Returns the full parser buffer
  static QString getXMLBuffer(void);

  //Reset all the elements resposible to the navigation through the element tree
  static void restartNavigation(void);

  /* Reset all the parser attributes, deallocating the element tree. The user have to
     reload the file to analyze it again */
  static void restartParser(void);
};

#endif
