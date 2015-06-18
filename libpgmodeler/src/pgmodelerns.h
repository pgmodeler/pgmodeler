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
\ingroup libpgmodeler
\namespace PgModelerNS
\brief This namespace is used to organize all functions or constants used in the libpgmodeler package.
*/

#ifndef PGMODELER_NS_H
#define PGMODELER_NS_H

#include "baseobject.h"

namespace PgModelerNS {
	/*! \brief Template function that makes a copy from 'copy_obj' to 'psrc_obj' doing the cast to the
		 correct object type. If the source object (psrc_obj) is not allocated the function allocates the attributes
		 before copying. Both objects must be the same type if both are allocated.
		 -- Brainfuck syntax style! :p -- */
  template <class Class>
  extern void copyObject(BaseObject **psrc_obj, Class *copy_obj);

	/*! \brief This functions is a second way to make a copy between two objects. It simply calls
		 the template function above. */
  extern void copyObject(BaseObject **psrc_obj, BaseObject *copy_obj, ObjectType obj_type);

  //! brief Returns true if the specified word is a PostgreSQL reserved word.
  extern bool isReservedKeyword(const QString &word);

  /*! brief Generates a unique name based upon the specified object and the list of objects of the same type.
  User can specify a suffix for the generated name as well if the comparison inside the method must be done with
  formated names */
  template <class Class>
  QString generateUniqueName(BaseObject *obj, vector<Class *> &obj_vector, bool fmt_name=false, const QString &suffix=QString())
  {
    unsigned counter=1;
    int len=0;
    QString aux_name, obj_name, id;
    Class *aux_obj=nullptr;
    typename std::vector<Class *>::iterator itr=obj_vector.begin(), itr_end=obj_vector.end();
    QChar oper_uniq_chr='?'; //Char appended at end of operator names in order to resolve conflicts
    ObjectType obj_type;

    if(!obj)
      return("");
    //Cast objects will not have the name changed since their name are automatically generated
    else if(obj->getObjectType()==OBJ_CAST || obj->getObjectType()==OBJ_DATABASE)
      return(obj->getName());

    obj_name=obj->getName(fmt_name);
    obj_type=obj->getObjectType();

    if(obj_type!=OBJ_OPERATOR)
      obj_name += suffix;

    id=QString::number(obj->getObjectId());
    len=obj_name.size() + id.size();

    //If the name length exceeds the maximum size
    if(len > BaseObject::OBJECT_NAME_MAX_LENGTH)
    {
      //Remove the last chars in the count of 3 + length of id
      obj_name.chop(id.size() + 3);

      //Append the id of the object on its name (this is not applied to operators)
      if(obj_type!=OBJ_OPERATOR)
        obj_name+=QString("_") + id;
    }

    aux_name=obj_name;

    //Check if the object's new name conflicts with some of the objects within the list
    while(itr!=itr_end)
    {
      aux_obj=(*itr);
      itr++;

      //If a conflicting object is found
      if(aux_obj!=obj && aux_obj->getName(fmt_name)==aux_name)
      {
        //For operators is appended a '?' on the name
        if(obj_type==OBJ_OPERATOR)
          aux_name=QString("%1%2").arg(obj_name).arg(QString("").leftJustified(counter++, oper_uniq_chr));
        else
          aux_name=QString("%1%2").arg(obj_name).arg(counter++);

        itr=obj_vector.begin();
      }
    }

    if(aux_name!=obj_name)
      obj_name=aux_name;

    return(obj_name);
  }
}

#endif
