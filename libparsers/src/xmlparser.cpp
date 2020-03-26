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

#include "xmlparser.h"
#include <QUrl>

const QString XmlParser::CharAmp("&amp;");
const QString XmlParser::CharLt("&lt;");
const QString XmlParser::CharGt("&gt;");
const QString XmlParser::CharQuot("&quot;");
const QString XmlParser::CharApos("&apos;");
const QString XmlParser::CdataStart("<![CDATA[");
const QString XmlParser::CdataEnd("]]>");
const QString XmlParser::CommentStart("<!--");
const QString XmlParser::CommentEnd("-->");

XmlParser::XmlParser()
{
	root_elem=nullptr;
	curr_elem=nullptr;
	xml_doc=nullptr;
	curr_line = 0;
	xmlInitParser();
}

XmlParser::~XmlParser()
{
	restartParser();
	xmlCleanupParser();
}

void XmlParser::removeDTD()
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
		throw Exception(e.getErrorMessage(), e.getErrorCode(), __PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
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
		throw Exception(e.getErrorMessage(), e.getErrorCode(), __PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
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

void XmlParser::readBuffer()
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
		parser_opt=( XML_PARSE_NOBLANKS | XML_PARSE_NONET | XML_PARSE_NOENT | XML_PARSE_BIG_LINES);

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

void XmlParser::savePosition()
{
	if(!root_elem)
		throw Exception(ErrorCode::OprNotAllocatedElementTree,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	elems_stack.push(curr_elem);
}

void XmlParser::restorePosition()
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

void XmlParser::restartNavigation()
{
	if(!root_elem)
		throw Exception(ErrorCode::OprNotAllocatedElementTree,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	curr_elem=root_elem;

	while(!elems_stack.empty())
		elems_stack.pop();
}

void XmlParser::restartParser()
{
	root_elem=curr_elem=nullptr;
	curr_line = 0;

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
	{
		curr_elem=elems[elem_type];

		/* NOTE: Due to XML2 implementation big line numbers are stored in the psvi
		 * attribute so we need to convert the void* to char and convert it back to integer value */
		if(curr_elem->line == 65535 && curr_elem->next && curr_elem->next->psvi != nullptr)
		{
			char hex_value[10] = "";
			int aux_line = 0;

			sprintf(hex_value, "%p", curr_elem->next->psvi);
			aux_line = static_cast<int>(strtol(hex_value, nullptr, 16));

			if(curr_line < aux_line)
				curr_line = aux_line;
		}
		else if(curr_elem->line > curr_line)
			curr_line = curr_elem->line;
	}

	return has_elem;
}

bool XmlParser::hasElement(unsigned elem_type, xmlElementType xml_node_type)
{
	if(!root_elem)
		throw Exception(ErrorCode::OprNotAllocatedElementTree,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	if(elem_type==RootElement)
		/* Returns the verification if the current element has a parent.
		 The element must be different from the root, because the root element
		 is not connected to a parent */
		return (curr_elem!=root_elem && curr_elem->parent!=nullptr &&
						(xml_node_type==0 || (xml_node_type!=0 && curr_elem->parent->type==xml_node_type)));
	else if(elem_type==ChildElement)
		//Returns the verification if the current element has children
		return (curr_elem->children!=nullptr &&
						(xml_node_type==0 || (xml_node_type!=0 && curr_elem->children->type==xml_node_type)));
	else if(elem_type==NextElement)
		return (curr_elem->next!=nullptr &&
						(xml_node_type==0 || (xml_node_type!=0 && curr_elem->next->type==xml_node_type)));
	else
		/* The second comparison in the expression is made for the root element
		 because libxml2 places the previous element as the root itself */
		return (curr_elem->prev!=nullptr && curr_elem->prev!=root_elem &&
															(xml_node_type==0 || (xml_node_type!=0 && curr_elem->prev->type==xml_node_type)));
}

bool XmlParser::hasAttributes()
{
	if(!root_elem)
		throw Exception(ErrorCode::OprNotAllocatedElementTree,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return (curr_elem->properties != nullptr);
}

QString XmlParser::getElementContent()
{
	if(!root_elem)
		throw Exception(ErrorCode::OprNotAllocatedElementTree,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	/* If the current element has  <![CDATA[]]> node returns the content of the CDATA instead
	of return the content of the element itself */
	if(curr_elem->next && curr_elem->next->type == XML_CDATA_SECTION_NODE)
		return QString(reinterpret_cast<char *>(curr_elem->next->content));
	else
		//Return the content of the element when is not a CDATA node
		return QString(reinterpret_cast<char *>(curr_elem->content));
}

QString XmlParser::getElementName()
{
	if(!root_elem)
		throw Exception(ErrorCode::OprNotAllocatedElementTree,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return QString(reinterpret_cast<const char *>(curr_elem->name));
}

xmlElementType XmlParser::getElementType()
{
	if(!root_elem)
		throw Exception(ErrorCode::OprNotAllocatedElementTree,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return curr_elem->type;
}

const xmlNode *XmlParser::getCurrentElement()
{
	return curr_elem;
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

QString XmlParser::getLoadedFilename()
{
	return xml_doc_filename;
}

QString XmlParser::getXMLBuffer()
{
	return xml_buffer;
}

int XmlParser::getCurrentBufferLine()
{
	if(curr_elem)
		return curr_line;
	else
		return 0;
}

int XmlParser::getBufferLineCount()
{
	if(xml_doc)
	{
		/* To get the very last line of the document is necessary to call
		the last element of the last because xml_doc->last->line stores the
		last line of the root element.

		NOTE: Due to XML2 implementation big line numbers are stored in the psvi
		attribute so we need to convert the void* to char and convert it back to integer value */
		if(xml_doc->last->last->line == 65535 && xml_doc->last->last->psvi != nullptr)
		{
			char hex_value[10] = "";
			sprintf(hex_value, "%p", xml_doc->last->last->psvi);
			return static_cast<int>(strtol(hex_value, nullptr, 16));
		}

		return xml_doc->last->last->line;
	}
	else
		return 0;
}

QString XmlParser::convertCharsToXMLEntities(QString buf)
{
	QTextStream ts(&buf);
	QRegExp attr_regexp=QRegExp("([a-z]|\\-)+( )*(=\\\")"),
			attr_end_regexp=QRegExp("(\\\")(( )|(\\t)|(\\n)|((\\/\\>)|(\\>)))");
	int attr_start=0, attr_end=0, count=0, cdata_start = -1,
			cdata_end = -1, start = -1, end = -1, pos = 0;
	QString value, fmt_buf, lin;

	while(!ts.atEnd())
	{
		lin = ts.readLine();
		lin += "\n";

		// Ignoring the xml header
		if(lin.indexOf("<?xml") >= 0)
		{
			fmt_buf += lin;
			continue;
		}

		// Checking if the current line has at least one attribute in form (attr="value")
		attr_start = -1;
		attr_start = attr_regexp.indexIn(lin);

		if(attr_start >= 0)
		{
			/* Checking the presence of <![[CDATA ]]> tag in the current line.
			 * In case of finding it we need to perform specific operation to avoid
			 * replacing contents within that tag */
			cdata_start = lin.indexOf(CdataStart);
			cdata_end = lin.indexOf(CdataEnd);
			start = min<int>(cdata_start, cdata_end);
			end = max<int>(cdata_start, cdata_end);

			do
			{
				attr_end = attr_end_regexp.indexIn(lin, attr_start + attr_regexp.matchedLength());

				if(attr_start >= 0 && attr_end >= 0 &&
					 //CDATA absent in the current line
					 ((start < 0 && end < 0) ||
						//The attribute is at left of the CDATA tag
						(start >= 0 && attr_start < start && attr_end < start) ||
						//The attribute is at right of the CDATA tag
						(end >= 0 && attr_start > end && attr_end > end)))
				{
					// Calculates the initial position where the value to be retrived is (in that case rigth after attrib=")
					pos = attr_start + attr_regexp.matchedLength();
					count = attr_end - pos;
					value = lin.mid(pos, count);
				}
				else
					break;

				/* If the extracted value has one of the expected special chars
				 * in order to perform the replacemnt to xml entities */
				if(value.contains(QRegExp("(&|\\<|\\>|\")")))
				{
					if(!value.contains(CharQuot) && !value.contains(CharLt) &&
						 !value.contains(CharGt) && !value.contains(CharAmp) &&
						 !value.contains(CharApos) && value.contains('&'))
							value.replace('&', CharAmp);

						value.replace('"', CharQuot);
						value.replace('<', CharLt);
						value.replace('>', CharGt);

					//Puts in the original XML definition the modified string
					lin.replace(pos, count, value);
				}

				// Moving the position to the next attribute in the line (if existent)
				pos += value.length() + 1;
				attr_start = attr_regexp.indexIn(lin, pos);
				value.clear();
			}
			while(attr_start >=0 && attr_start < lin.size());
		}

		fmt_buf += lin;
		lin.clear();
	}

	return fmt_buf;
}
