/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2019 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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
#include <QUrl>

const QString XmlParser::CharAmp=QString("&amp;");
const QString XmlParser::CharLt=QString("&lt;");
const QString XmlParser::CharGt=QString("&gt;");
const QString XmlParser::CharQuot=QString("&quot;");
const QString XmlParser::CharApos=QString("&apos;");

XmlParser::XmlParser(void)
{
	root_elem=nullptr;
	curr_elem=nullptr;
	xml_doc=nullptr;
	xmlInitParser();
}

XmlParser::~XmlParser(void)
{
	restartParser();
	xmlCleanupParser();
}

void XmlParser::removeDTD(void)
{
	int pos1=-1, pos2=-1, pos3=-1, len;

	if(!xml_buffer.isEmpty())
	{
		/* Removes the current DTD from document.
		 If the user attempts to manipulate the structure of
		 document damaging its integrity. */
		pos1=xml_buffer.indexOf(QLatin1String("<!DOCTYPE"));
		pos2=xml_buffer.indexOf(QLatin1String("]>\n"));
		pos3=xml_buffer.indexOf(QLatin1String("\">\n"));
		if(pos1 >=0 && (pos2 >=0 || pos3 >= 0))
		{
			len=((pos2 > pos3) ? (pos2-pos1)+3 :  (pos3-pos2)+3);
			xml_buffer.replace(pos1,len,QString());
		}
	}
}

void XmlParser::loadXMLFile(const QString &filename)
{
	try
	{
		QFile input;
		QString buffer;

		if(!filename.isEmpty())
		{
			//Opens a file stream using the file name
			input.setFileName(filename);
			input.open(QFile::ReadOnly);

			//Case the file opening was not sucessful
			if(!input.isOpen())
			{
				throw Exception(Exception::getErrorMessage(ErrorCode::FileDirectoryNotAccessed).arg(filename),
								ErrorCode::FileDirectoryNotAccessed,__PRETTY_FUNCTION__,__FILE__,__LINE__);
			}

			buffer=input.readAll();
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

void XmlParser::loadXMLBuffer(const QString &xml_buf)
{
	try
	{
		int pos1=-1, pos2=-1, tam=0;

		if(xml_buf.isEmpty())
			throw Exception(ErrorCode::AsgEmptyXMLBuffer,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		pos1=xml_buf.indexOf(QLatin1String("<?xml"));
		pos2=xml_buf.indexOf(QLatin1String("?>"));
		xml_buffer=xml_buf;

		if(pos1 >= 0 && pos2 >= 0)
		{
			tam=(pos2-pos1)+3;
			xml_decl=xml_buffer.mid(pos1, tam);
			xml_buffer.replace(pos1,tam,QString());
		}
		else
			xml_decl=QString("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");

		removeDTD();
		readBuffer();
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(), __PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void XmlParser::setDTDFile(const QString &dtd_file, const QString &dtd_name)
{
	QString fmt_dtd_file;

	if(dtd_file.isEmpty())
		throw Exception(ErrorCode::AsgEmptyDTDFile,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	if(dtd_name.isEmpty())
		throw Exception(ErrorCode::AsgEmptyDTDName,__PRETTY_FUNCTION__,__FILE__,__LINE__);

#ifndef Q_OS_WIN
	fmt_dtd_file=QString("file://");
#else
	fmt_dtd_file=QString("file:///");
#endif

	//Formats the dtd file path to URL style (converting to percentage format the non reserved chars)
	fmt_dtd_file=QUrl::toPercentEncoding(QFileInfo(dtd_file).absoluteFilePath(), "/:");
	dtd_decl=QString("<!DOCTYPE ") + dtd_name +
			 QString(" SYSTEM ") +
			 QString("\"") +
			 fmt_dtd_file + QString("\">\n");
}

void XmlParser::readBuffer(void)
{
	QByteArray buffer;
	QString msg, file;
	xmlError *xml_error=nullptr;
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
		xml_doc=xmlReadMemory(buffer.data(), buffer.size(),	nullptr, nullptr, parser_opt);

		//In case the document criation fails, gets the last xml parser error
		xml_error=xmlGetLastError();

		//If some error is set
		if(xml_error)
		{
			//Formats the error
			msg=xml_error->message;
			file=xml_error->file;
			if(!file.isEmpty()) file=QString("(%1)").arg(file);
			msg.replace("\n"," ");

			//Restarts the parser
			if(xml_doc) restartParser();

			//Raise an exception with the error massege from the parser xml
			throw Exception(Exception::getErrorMessage(ErrorCode::LibXMLError)
							.arg(xml_error->line).arg(xml_error->int2).arg(msg).arg(file),
							ErrorCode::LibXMLError,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		}

		//Gets the referênce to the root element on the document
		root_elem=curr_elem=xmlDocGetRootElement(xml_doc);
	}
}

void XmlParser::savePosition(void)
{
	if(!root_elem)
		throw Exception(ErrorCode::OprNotAllocatedElementTree,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	elems_stack.push(curr_elem);
}

void XmlParser::restorePosition(void)
{
	if(!root_elem)
		throw Exception(ErrorCode::OprNotAllocatedElementTree,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	if(elems_stack.empty())
		curr_elem=root_elem;
	else
	{
		curr_elem=elems_stack.top();
		elems_stack.pop();
	}
}

void XmlParser::restorePosition(const xmlNode *elem)
{
	if(!elem)
		throw Exception(ErrorCode::OprNotAllocatedElement,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else if(elem->doc!=xml_doc)
		throw Exception(ErrorCode::OprInexistentElement,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	restartNavigation();
	curr_elem=const_cast<xmlNode *>(elem);
}

void XmlParser::restartNavigation(void)
{
	if(!root_elem)
		throw Exception(ErrorCode::OprNotAllocatedElementTree,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	curr_elem=root_elem;

	while(!elems_stack.empty())
		elems_stack.pop();
}

void XmlParser::restartParser(void)
{
	root_elem=curr_elem=nullptr;

	if(xml_doc)
	{
		xmlFreeDoc(xml_doc);
		xml_doc=nullptr;
	}
	dtd_decl=xml_buffer=xml_decl=QString();

	while(!elems_stack.empty())
		elems_stack.pop();

	xml_doc_filename=QString();
	xmlResetLastError();
}

bool XmlParser::accessElement(unsigned elem_type)
{
	bool has_elem;
	xmlNode *elems[4];

	if(!root_elem)
		throw Exception(ErrorCode::OprNotAllocatedElementTree,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	elems[RootElement]=curr_elem->parent;
	elems[ChildElement]=curr_elem->children;
	elems[NextElement]=curr_elem->next;
	elems[PreviousElement]=curr_elem->prev;

	/* Checks whether the current element has the element that
		is to  be accessed. The flag 'has_elem' is also used
		on the method return to indicate if the element has been
		accessed or not. */
	has_elem=hasElement(elem_type);

	if(has_elem)
		curr_elem=elems[elem_type];

	return(has_elem);
}

bool XmlParser::hasElement(unsigned elem_type, xmlElementType xml_node_type)
{
	if(!root_elem)
		throw Exception(ErrorCode::OprNotAllocatedElementTree,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	if(elem_type==RootElement)
		/* Returns the verification if the current element has a parent.
		 The element must be different from the root, because the root element
		 is not connected to a parent */
		return(curr_elem!=root_elem && curr_elem->parent!=nullptr &&
														  (xml_node_type==0 || (xml_node_type!=0 && curr_elem->parent->type==xml_node_type)));
	else if(elem_type==ChildElement)
		//Returns the verification if the current element has children
		return(curr_elem->children!=nullptr &&
									(xml_node_type==0 || (xml_node_type!=0 && curr_elem->children->type==xml_node_type)));
	else if(elem_type==NextElement)
		return(curr_elem->next!=nullptr &&
								(xml_node_type==0 || (xml_node_type!=0 && curr_elem->next->type==xml_node_type)));
	else
		/* The second comparison in the expression is made for the root element
		 because libxml2 places the previous element as the root itself */
		return(curr_elem->prev!=nullptr && curr_elem->prev!=root_elem &&
															(xml_node_type==0 || (xml_node_type!=0 && curr_elem->prev->type==xml_node_type)));
}

bool XmlParser::hasAttributes(void)
{
	if(!root_elem)
		throw Exception(ErrorCode::OprNotAllocatedElementTree,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return(curr_elem->properties!=nullptr);
}

QString XmlParser::getElementContent(void)
{
	if(!root_elem)
		throw Exception(ErrorCode::OprNotAllocatedElementTree,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	/* If the current element has  <![CDATA[]]> node returns the content of the CDATA instead
	of return the content of the element itself */
	if(curr_elem->next && curr_elem->next->type == XML_CDATA_SECTION_NODE)
		return(QString(reinterpret_cast<char *>(curr_elem->next->content)));
	else
		//Return the content of the element when is not a CDATA node
		return(QString(reinterpret_cast<char *>(curr_elem->content)));
}

QString XmlParser::getElementName(void)
{
	if(!root_elem)
		throw Exception(ErrorCode::OprNotAllocatedElementTree,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return(QString(reinterpret_cast<const char *>(curr_elem->name)));
}

xmlElementType XmlParser::getElementType(void)
{
	if(!root_elem)
		throw Exception(ErrorCode::OprNotAllocatedElementTree,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return(curr_elem->type);
}

const xmlNode *XmlParser::getCurrentElement(void)
{
	return(curr_elem);
}

void XmlParser::getElementAttributes(attribs_map &attributes)
{
	xmlAttr *elem_attribs=nullptr;
	QString attrib, value;

	if(!root_elem)
		throw Exception(ErrorCode::OprNotAllocatedElementTree,__PRETTY_FUNCTION__,__FILE__,__LINE__);

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

QString XmlParser::getLoadedFilename(void)
{
	return(xml_doc_filename);
}

QString XmlParser::getXMLBuffer(void)
{
	return(xml_buffer);
}

int XmlParser::getCurrentBufferLine(void)
{
	if(curr_elem)
		return(curr_elem->line);
	else
		return(0);
}

int XmlParser::getBufferLineCount(void)
{
	if(xml_doc)
		/* To get the very last line of the document is necessary to call
		the last element of the last because xml_doc->last->line stores the
		last line of the root element */
		return(xml_doc->last->last->line);
	else
		return(0);
}

