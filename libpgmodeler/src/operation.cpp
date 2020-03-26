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

#include "operation.h"

Operation::Operation()
{
	parent_obj=nullptr;
	pool_obj=nullptr;
	original_obj=nullptr;
	object_idx=-1;
	chain_type=NoChain;
	op_type=NoOperation;
}

QString Operation::generateOperationId()
{
	QString addr;
	QTextStream stream(&addr);
	QCryptographicHash hash(QCryptographicHash::Md5);

	//Stores the permission address on a string
	stream << reinterpret_cast<unsigned *>(original_obj);
	stream << reinterpret_cast<unsigned *>(pool_obj);
	stream << reinterpret_cast<unsigned *>(parent_obj);

	//Generates an unique id through md5 hash
	hash.addData(QByteArray(addr.toStdString().c_str()));
	return hash.result().toHex();
}

void Operation::setObjectIndex(int idx)
{
	object_idx=idx;
}

void Operation::setChainType(unsigned type)
{
	chain_type=(type > ChainEnd ? NoChain : type);
}

void Operation::setOperationType(unsigned type)
{
	op_type=(type > ObjectMoved ? NoOperation : type);
}

void Operation::setOriginalObject(BaseObject *object)
{
	original_obj=object;
	operation_id=generateOperationId();
}

void Operation::setPoolObject(BaseObject *object)
{
	pool_obj=object;
	operation_id=generateOperationId();
}

void Operation::setParentObject(BaseObject *object)
{
	parent_obj=object;
	operation_id=generateOperationId();
}

void Operation::setPermissions(const vector<Permission *> &perms)
{
	permissions=perms;
}

void Operation::setXMLDefinition(const QString &xml_def)
{
	xml_definition=xml_def;
}

int Operation::getObjectIndex()
{
	return object_idx;
}

unsigned Operation::getChainType()
{
	return chain_type;
}

unsigned Operation::getOperationType()
{
	return op_type;
}

BaseObject *Operation::getOriginalObject()
{
	return original_obj;
}

BaseObject *Operation::getPoolObject()
{
	return pool_obj;
}

BaseObject *Operation::getParentObject()
{
	return parent_obj;
}

vector<Permission *> Operation::getPermissions()
{
	return permissions;
}

QString Operation::getXMLDefinition()
{
	return xml_definition;
}

bool Operation::isOperationValid()
{
	return (operation_id==generateOperationId());
}
