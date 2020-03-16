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
\ingroup libpgmodeler
\namespace PgModelerNS
\brief This namespace is used to organize all functions or constants used in the libpgmodeler package.
*/

#ifndef PGMODELER_NS_H
#define PGMODELER_NS_H

#include "baseobject.h"
#include "tableobject.h"
#include <QCryptographicHash>

namespace PgModelerNs {
	//! \brief Default char used as unescaped value start delimiter
	static const QChar UnescValueStart='/';

	//! \brief Default char used as unescaped value end delimiter
	static const QChar	UnescValueEnd='/';

	/*! \brief Template function that makes a copy from 'copy_obj' to 'psrc_obj' doing the cast to the
		 correct object type. If the source object (psrc_obj) is not allocated the function allocates the attributes
		 before copying. Both objects must be the same type if both are allocated.
		 -- Brainfuck syntax style! :p -- */
	template <class Class>
	extern void copyObject(BaseObject **psrc_obj, Class *copy_obj);

	/*! \brief This functions is a second way to make a copy between two objects. It simply calls
		 the template function above. */
	extern void copyObject(BaseObject **psrc_obj, BaseObject *copy_obj, ObjectType obj_type);

	//! \brief Returns true if the specified word is a PostgreSQL reserved word.
	extern bool isReservedKeyword(const QString &word);

	/*! \brief Generates a unique name based upon the specified object and the list of objects of the same type.
	 * User can specify a suffix for the generated name as well if the comparison inside the method must be done with
	 * formated names. The optinal parameter use_suf_on_conflict indicates that the suffix should be used only in case of conflicts.
	 * Now, the discard_input_obj is used to indicate if the input object should be considered or not in the unique name generation, in this case,
	 * when false the name of the input object (obj) will always be compared to itself if it is present in the provided list. When that
	 * parameter is true the comparison is not made. */
	template <class Class>
	QString generateUniqueName(BaseObject *obj, vector<Class *> &obj_vector,
														 bool fmt_name = false, const QString &suffix = QString(),
														 bool use_suf_on_conflict = false, bool discard_input_obj = false)
	{
		unsigned counter=0;
		int len=0;
		QString aux_name, obj_name, id;
		Class *aux_obj=nullptr;
		typename std::vector<Class *>::iterator itr=obj_vector.begin(), itr_end=obj_vector.end();
		QChar oper_uniq_chr='?'; //Char appended at end of operator names in order to resolve conflicts
		ObjectType obj_type;
		QCryptographicHash hash(QCryptographicHash::Md5);
		QByteArray buffer;

		if(!obj)
			return("");
		//Cast objects will not have the name changed since their name are automatically generated
		else if(obj->getObjectType()==ObjectType::Cast || obj->getObjectType()==ObjectType::Database)
			return(obj->getName());

		obj_name=obj->getName(fmt_name);
		obj_type=obj->getObjectType();

		if(!use_suf_on_conflict && obj_type!=ObjectType::Operator)
			obj_name += suffix;

		counter = (use_suf_on_conflict && obj_type!= ObjectType::Operator? 0 : 1);
		buffer.append(obj_name);
		id = hash.result().toHex().mid(0,6);
		len = obj_name.size() + id.size();

		//If the name length exceeds the maximum size
		if(len > BaseObject::ObjectNameMaxLength)
		{
			//Remove the last chars in the count of 3 + length of id
			obj_name.chop(id.size() + 3);

			//Append the id of the object on its name (this is not applied to operators)
			if(obj_type!=ObjectType::Operator)
				obj_name += QString("_") + id;
		}

		obj_name.remove('"');
		aux_name = obj_name;

		//Check if the object's new name conflicts with some of the objects within the list
		while(itr != itr_end)
		{
			aux_obj = (*itr);
			itr++;

			if(discard_input_obj && aux_obj == obj)
				continue;

			//If a conflicting object is found
			if(aux_obj->getName(fmt_name).remove('"') == aux_name)
			{
				//For operators is appended a '?' on the name
				if(obj_type==ObjectType::Operator)
					aux_name=QString("%1%2").arg(obj_name).arg(QString("").leftJustified(counter++, oper_uniq_chr));
				else
				{
					aux_name=QString("%1%2%3")
									 .arg(obj_name)
									 .arg(use_suf_on_conflict ? suffix : QString())
									 .arg(use_suf_on_conflict && counter == 0 ? QString() : QString::number(counter));
					counter++;
				}

				itr=obj_vector.begin();
			}
		}

		if(aux_name != obj_name)
			obj_name = aux_name;

		// If using name formatting option we need to strip the schema name from the generated name
		if(fmt_name && obj->getSchema())
			obj_name.remove(QString("%1.").arg(obj->getSchema()->getName()));

		return(obj_name);
	}
}

#endif
