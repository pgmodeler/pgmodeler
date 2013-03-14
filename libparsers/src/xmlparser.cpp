/*
# PostgreSQL Database Modeler (pgModeler)
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

#include "xmlparser.h"

QString XMLParser::xml_doc_filename="";
QString XMLParser::xml_buffer="";
QString XMLParser::dtd_decl="";
QString XMLParser::xml_decl="";
xmlNode *XMLParser::root_elem=NULL;
xmlNode *XMLParser::curr_elem=NULL;
xmlDoc *XMLParser::xml_doc=NULL;
stack<xmlNode*> XMLParser::elems_stack;

const QString XMLParser::CHAR_AMP="&amp;";
const QString XMLParser::CHAR_LT="&lt;";
const QString XMLParser::CHAR_GT="&gt;";
const QString XMLParser::CHAR_QUOT="&quot;";
const QString XMLParser::CHAR_APOS="&apos;";

XMLParser::~XMLParser(void)
{
	restartParser();
}

void XMLParser::removeDTD(void)
{
	int pos1=-1, pos2=-1, pos3=-1, len;

	if(!xml_buffer.isEmpty())
	{
		/* Removes the current DTD from document.
		 If the user attempts to manipulate the structure of
		 document damaging its integrity. */
		pos1=xml_buffer.indexOf("<!DOCTYPE");
		pos2=xml_buffer.indexOf("]>\n");
		pos3=xml_buffer.indexOf("\">\n");
		if(pos1 >=0 && (pos2 >=0 || pos3 >= 0))
		{
			len=((pos2 > pos3) ? (pos2-pos1)+3 :  (pos3-pos2)+3);
			xml_buffer.replace(pos1,len,"");
		}
	}
}

void XMLParser::loadXMLFile(const QString &filename)
{
	try
	{
		ifstream input;
		QString buffer, str_aux;
		string line;

		if(filename!="")
		{
			//Opens a file stream using the file name
			input.open(filename.toAscii(),ios_base::in);

			//Case the file opening was sucessful
			if(!input.is_open())
			{
				str_aux=QString(Exception::getErrorMessage(ERR_FILE_DIR_NOT_ACCESSED)).arg(filename);
				throw Exception(str_aux,ERR_FILE_DIR_NOT_ACCESSED,__PRETTY_FUNCTION__,__FILE__,__LINE__);
			}

			buffer="";

			//Read the file line by line and store them on the parser buffer
			while(!input.eof())
			{
				getline(input, line);
				buffer+=QString::fromStdString(line) + "\n";
			}
			input.close();

			xml_doc_filename=filename;
			loadXMLBuffer(buffer);
		}
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(), __PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void XMLParser::loadXMLBuffer(const QString &xml_buf)
{
	try
	{
		int pos1=-1, pos2=-1, tam=0;

		if(xml_buf.isEmpty())
			throw Exception(ERR_ASG_EMPTY_XML_BUFFER,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		pos1=xml_buf.indexOf("<?xml");
		pos2=xml_buf.indexOf("?>");
		xml_buffer=xml_buf;

		if(pos1 >= 0 && pos2 >= 0)
		{
			tam=(pos2-pos1)+3;
			xml_decl=xml_buffer.mid(pos1, tam);
			xml_buffer.replace(pos1,tam,"");
		}
		else
			xml_decl="<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";

		removeDTD();
		readBuffer();
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(), __PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void XMLParser::setDTDFile(const QString &dtd_file, const QString &dtd_name)
{
	QString fmt_dtd_file;

	if(dtd_file.isEmpty())
		throw Exception(ERR_ASG_EMPTY_DTD_FILE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	if(dtd_name.isEmpty())
		throw Exception(ERR_ASG_EMPTY_DTD_NAME,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	#ifndef Q_OS_WIN
		fmt_dtd_file="file://";
	#else
		fmt_dtd_file="file:///";
	#endif

	//Formats the dtd file path in order to replace spaces by %20 (url format)
	fmt_dtd_file+=QFileInfo(dtd_file).absoluteFilePath();
	dtd_decl="<!DOCTYPE " + dtd_name + " SYSTEM " + "\"" +  fmt_dtd_file.replace(QString(" "),QString("%20")) + "\">\n";
}

void XMLParser::readBuffer(void)
{
	QString buffer, msg, file;
	xmlError *xml_error=NULL;
	int parser_opt;

	if(!xml_buffer.isEmpty())
	{
		//Inserts the XML declaration
		buffer+=xml_decl;

		//Configures the parser, initially, to not validate the document against the dtd
		parser_opt=( XML_PARSE_NOBLANKS | XML_PARSE_NONET | XML_PARSE_NOENT );

		//If the dtd declarions is setup
		if(!dtd_decl.isEmpty())
		{
			//Inserts the default software DTD declarion into XML buffer
			buffer+=dtd_decl;

			//Now configures the parser to validate the buffer against the DTD
			parser_opt=(parser_opt | XML_PARSE_DTDLOAD | XML_PARSE_DTDVALID);
		}

		buffer+=xml_buffer;

		//Create an xml document from the buffer
		xml_doc=xmlReadMemory(buffer.toStdString().c_str(), buffer.size(),
													NULL, NULL, parser_opt);

		//In case the document criation fails, gets the last xml parser error
		xml_error=xmlGetLastError();

		//If some error is set
		if(xml_error)
		{
			//Formats the error
			msg=xml_error->message;
			file=xml_error->file;
			if(!file.isEmpty()) file="("+file+")";
			msg.replace("\n"," ");

			//Restarts the parser
			if(xml_doc) restartParser();

			//Raise an exception with the error massege from the parser xml
			throw Exception(QString(Exception::getErrorMessage(ERR_LIBXMLERR))
											.arg(xml_error->line).arg(xml_error->int2).arg(msg).arg(file),
											ERR_LIBXMLERR,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		}

		//Gets the referênce to the root element on the document
		root_elem=curr_elem=xmlDocGetRootElement(xml_doc);

		//Cleanup the allocated parser variables
		if(xml_doc) xmlCleanupParser();
	}
}

void XMLParser::savePosition(void)
{
	if(!root_elem)
		throw Exception(ERR_OPR_NOT_ALOC_ELEM_TREE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	elems_stack.push(curr_elem);
}

void XMLParser::restorePosition(void)
{
	if(!root_elem)
		throw Exception(ERR_OPR_NOT_ALOC_ELEM_TREE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	if(elems_stack.empty())
		curr_elem=root_elem;
	else
	{
		curr_elem=elems_stack.top();
		elems_stack.pop();
	}
}

void XMLParser::restorePosition(const xmlNode *elem)
{
	if(!elem)
		throw Exception(ERR_OPR_NOT_ALOC_ELEMENT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else if(elem->doc!=xml_doc)
		throw Exception(ERR_OPR_INEXIST_ELEMENT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	restartNavigation();
	curr_elem=const_cast<xmlNode *>(elem);
}

void XMLParser::restartNavigation(void)
{
	if(!root_elem)
		throw Exception(ERR_OPR_NOT_ALOC_ELEM_TREE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	curr_elem=root_elem;

	while(!elems_stack.empty())
		elems_stack.pop();
}

void XMLParser::restartParser(void)
{
	root_elem=curr_elem=NULL;
	if(xml_doc)
	{
		xmlFreeDoc(xml_doc);
		xml_doc=NULL;
	}
	dtd_decl=xml_buffer=xml_decl="";

	while(!elems_stack.empty())
		elems_stack.pop();

	xmlResetLastError();
	xml_doc_filename="";
}

bool XMLParser::accessElement(unsigned elem_type)
{
	bool has_elem;
	xmlNode *elems[4];

	if(!root_elem)
		throw Exception(ERR_OPR_NOT_ALOC_ELEM_TREE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	elems[ROOT_ELEMENT]=curr_elem->parent;
	elems[CHILD_ELEMENT]=curr_elem->children;
	elems[NEXT_ELEMENT]=curr_elem->next;
	elems[PREVIOUS_ELEMENT]=curr_elem->prev;

	/* Checks whether the current element has the element that
		is to  be accessed. The flag 'has_elem' is also used
		on the method return to indicate if the element has been
		accessed or not. */
	has_elem=hasElement(elem_type);

	if(has_elem)
		curr_elem=elems[elem_type];

	return(has_elem);
}

bool XMLParser::hasElement(unsigned tipo_elem)
{
	if(!root_elem)
		throw Exception(ERR_OPR_NOT_ALOC_ELEM_TREE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	if(tipo_elem==ROOT_ELEMENT)
		/* Returns the verification if the current element has a parent.
		 The element must be different from the root, because the root element
		 is not connected to a parent */
		return(curr_elem!=root_elem && curr_elem->parent!=NULL);
	else if(tipo_elem==CHILD_ELEMENT)
		//Returns the verification if the current element has children
		return(curr_elem->children!=NULL);
	else if(tipo_elem==NEXT_ELEMENT)
		return(curr_elem->next!=NULL);
	else
		/* The second comparison in the expression is made for the root element
		 because libxml2 places the previous element as the root itself */
		return(curr_elem->prev!=NULL && curr_elem->prev!=root_elem);
}

bool XMLParser::hasAttributes(void)
{
	if(!root_elem)
		throw Exception(ERR_OPR_NOT_ALOC_ELEM_TREE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return(curr_elem->properties!=NULL);
}

QString XMLParser::getElementContent(void)
{
	if(!root_elem)
		throw Exception(ERR_OPR_NOT_ALOC_ELEM_TREE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	/* If the current element has  <![CDATA[]]> node returns the content of the CDATA instead
	of return the content of the element itself */
	if(curr_elem->next && curr_elem->next->type == XML_CDATA_SECTION_NODE)
		return(QString(reinterpret_cast<char *>(curr_elem->next->content)));
	else
		//Return the content of the element when is not a CDATA node
		return(QString(reinterpret_cast<char *>(curr_elem->content)));
}

QString XMLParser::getElementName(void)
{
	if(!root_elem)
		throw Exception(ERR_OPR_NOT_ALOC_ELEM_TREE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return(QString(reinterpret_cast<const char *>(curr_elem->name)));
}

xmlElementType XMLParser::getElementType(void)
{
	if(!root_elem)
		throw Exception(ERR_OPR_NOT_ALOC_ELEM_TREE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return(curr_elem->type);
}

const xmlNode *XMLParser::getCurrentElement(void)
{
	return(curr_elem);
}

void XMLParser::getElementAttributes(map<QString, QString> &attributes)
{
	xmlAttr *elem_attribs=NULL;
	QString attrib, value;

	if(!root_elem)
		throw Exception(ERR_OPR_NOT_ALOC_ELEM_TREE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	//Always clears the passed attributes maps
	attributes.clear();

	//Gets the references to the element properties
	elem_attribs=curr_elem->properties;

	while(elem_attribs)
	{
		//Gets the attribute name
		attrib=QString(reinterpret_cast<const char *>(elem_attribs->name));
		//Gets the attribute value
		value=QString(reinterpret_cast<char *>(elem_attribs->children->content));

		/* Assigns to the attribute map in the index specified by the
		 attribute name the obtained value */
		attributes[attrib]=value;

		//Step to the next element attribute
		elem_attribs=elem_attribs->next;
	}
}

QString XMLParser::getLoadedFilename(void)
{
	return(xml_doc_filename);
}

QString XMLParser::getXMLBuffer(void)
{
	return(xml_buffer);
}

int XMLParser::getCurrentBufferLine(void)
{
	if(curr_elem)
		return(curr_elem->line);
	else
		return(0);
}

int XMLParser::getBufferLineCount(void)
{
	if(xml_doc)
		return(xml_doc->last->line);
	else
		return(0);
}

