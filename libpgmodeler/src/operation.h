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
\class Operation
\brief Implements the class that stores operations executed over an object
*/

#ifndef OPERATION_H
#define OPERATION_H

#include "baseobject.h"
#include "permission.h"
#include <QString>

class LIBPGMODELER_EXPORT Operation {
  private:
    /*! brief Uniquely identifies the object. This id is used to check if the operation object's somehow
        where delete (changing their addresses). This will avoid the operation list to try to execute
        an invalid operation */
    QString operation_id;

		/*! \brief Reference to the parent object of the original object that has suffered the operation,
		This attribute is used only in the case of table objects such as columns, indexes,
		constraints, rules. For other object types there is no need to use this attribute
		because the parent object for other objects is always the database model */
		BaseObject *parent_obj;

		//! \brief Reference (on the pool) to the copy of the original object
		BaseObject *pool_obj;

		//! \brief Reference to the original object that generates the operation.
		BaseObject *original_obj;

		/*! \brief Stores the XML definition of the special objects this means the objects
		 that reference columns added by relationship. This is the case of triggers,
		 indexes, sequences, constraints. */
		QString xml_definition;

		//! \brief Operation type (Constants OBJECT_[MODIFIED | CREATED | REMOVED | MOVED]
		unsigned op_type;

		//! \brief Operation chain type. This attribute is used to redo/undo several operations at once
		unsigned chain_type;

		//! \brief Object index inside the list on its parent object
		int object_idx;

		//! \brief Stores the object's permission before it's removal
		vector<Permission *> permissions;

    //! brief Generate an unique id for the operation based upon the memory addresses of objects held by it
    QString generateOperationId(void);

	public:
		//! \brief Constants used to reference the type of operations
    static const unsigned NO_OPERATION=0,
                          OBJECT_MODIFIED=1,
                          OBJECT_CREATED=2,
                          OBJECT_REMOVED=3,
                          /*! \brief This type of operation has the same effect of operation OBJECT_MODIFIED
                            except that it not (re)validate relationships as happens with operations.
                            This type of operation (OBJECT_MOVED) is useful to undo position changes of
                            graphical objects without executing unnecessary revalidations of relationships */
                          OBJECT_MOVED=4;

		//! \brief Operation chain types
    static const unsigned NO_CHAIN=0, //! \brief The operation is not part of a chain
                          CHAIN_START=1, //! \brief The operation is the head of the chain
                          CHAIN_MIDDLE=2, //! \brief The operation is in the middle of the chain
                          CHAIN_END=3; //! \brief The operation is the last on the chain


    Operation(void);

    void setObjectIndex(int idx);
    void setChainType(unsigned type);
    void setOperationType(unsigned type);
    void setOriginalObject(BaseObject *object);
    void setPoolObject(BaseObject *object);
    void setParentObject(BaseObject *object);
    void setPermissions(const vector<Permission *> &perms);
    void setXMLDefinition(const QString &xml_def);

    int getObjectIndex(void);
    unsigned getChainType(void);
    unsigned getOperationType(void);
    BaseObject *getOriginalObject(void);
    BaseObject *getPoolObject(void);
    BaseObject *getParentObject(void);
    vector<Permission *> getPermissions(void);
    QString getXMLDefinition(void);
    bool isOperationValid(void);
};

#endif
