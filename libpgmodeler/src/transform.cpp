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

#include "transform.h"

Transform::Transform()
{
	obj_type = ObjectType::Transform;
	language = nullptr;
	functions[FromSqlFunc] = nullptr;
	functions[ToSqlFunc] = nullptr;

	attributes[Attributes::Type] = "";
	attributes[Attributes::Language] = "";
	attributes[Attributes::ToSqlFunc] = "";
	attributes[Attributes::FromSqlFunc] = "";

	setName("");
}

void Transform::setName(const QString &)
{
	QString tp_name = ~type;
	BaseObject *type_obj = nullptr;

	//	If the type is based upon a user-defined type we remove the schema name form its name
	if(type.isUserType())
	{
		type_obj = reinterpret_cast<BaseObject *>(type.getUserTypeReference());

		if(type_obj && type_obj->getSchema())
			tp_name.remove(type_obj->getSchema()->getName() + ".");
	}

	// The name format for a transform is "type_language" or "type_undefined_lang" for transform without a language defined (initial state)
	obj_name = QString("%1_%2").arg((tp_name).replace(' ', '_')).arg(language ? language->getName() : Attributes::Undefined);
}

void Transform::setType(PgSqlType tp)
{
	if(tp == PgSqlType::Null)
		throw Exception(ErrorCode::AsgInvalidTypeObject, __PRETTY_FUNCTION__, __FILE__, __LINE__);

	try
	{
		if(functions[ToSqlFunc])
			validateFunction(functions[ToSqlFunc], ToSqlFunc);

		setCodeInvalidated(type != tp);
		type = tp;
		setName("");
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(), __PRETTY_FUNCTION__, __FILE__, __LINE__, &e);
	}
}

void Transform::setLanguage(Language *lang)
{
	if(!lang)
		throw Exception(ErrorCode::AsgNotAllocatedLanguage, __PRETTY_FUNCTION__, __FILE__, __LINE__);

	setCodeInvalidated(language != lang);
	language = lang;
	setName("");
}

void Transform::setFunction(Function *func, unsigned func_id)
{
	try
	{
		validateFunction(func, func_id);
		setCodeInvalidated(true);
		functions[func_id] = func;
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(), __PRETTY_FUNCTION__, __FILE__, __LINE__, &e);
	}
}

void Transform::validateFunction(Function *func, unsigned func_id)
{
	if(func_id > ToSqlFunc)
		throw Exception(ErrorCode::RefFunctionInvalidType, __PRETTY_FUNCTION__, __FILE__, __LINE__);

	if(!func)
		return;

	// The function to be assigned must have only one parameter
	if(func->getParameterCount() != 1)
	{
		throw Exception(Exception::getErrorMessage(ErrorCode::AsgFunctionInvalidParamCount)
						.arg(this->getName())
						.arg(this->getTypeName()),
						ErrorCode::AsgFunctionInvalidParamCount,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	}

	// The function must have a single parameter of the type "internal"
	if(func->getParameter(0).getType() != "internal")
	{
		throw Exception(Exception::getErrorMessage(ErrorCode::AsgFunctionInvalidParameters)
						.arg(this->getName())
						.arg(this->getTypeName()),
						ErrorCode::AsgFunctionInvalidParameters,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	}

	/* ToSqlFunc must return a type that is equivalent to the type being handled by the transform
	 * FromSqlFunc must always return "internal" data type */
	if((func_id == ToSqlFunc && !func->getReturnType().isEquivalentTo(type)) ||
		 (func_id == FromSqlFunc && func->getReturnType() != "internal"))
	{
		throw Exception(Exception::getErrorMessage(ErrorCode::AsgFunctionInvalidReturnType)
						.arg(this->getName())
						.arg(this->getTypeName()),
						ErrorCode::AsgFunctionInvalidReturnType,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	}
}

PgSqlType Transform::getType()
{
	return type;
}

Language *Transform::getLanguage()
{
	return language;
}

Function *Transform::getFunction(unsigned func_id)
{
	if(func_id > ToSqlFunc)
		throw Exception(ErrorCode::RefFunctionInvalidType, __PRETTY_FUNCTION__, __FILE__, __LINE__);

	return functions[func_id];
}

QString Transform::getCodeDefinition(unsigned def_type)
{
	QString code_def=getCachedCode(def_type, false);
	if(!code_def.isEmpty()) return code_def;

	QStringList funcs_attr = {  Attributes::FromSqlFunc,Attributes::ToSqlFunc };


	if(def_type == SchemaParser::SqlDefinition)
	{
		attributes[Attributes::Type] = ~type;

		if(language)
			attributes[Attributes::Language] = language->getName(true);

		for(unsigned func_id = FromSqlFunc; func_id <= ToSqlFunc; func_id++)
		{
			if(functions[func_id])
				attributes[funcs_attr[func_id]] = functions[func_id]->getSignature();
		}
	}
	else
	{
		attributes[Attributes::Type] = type.getCodeDefinition(def_type);

		if(language)
			attributes[Attributes::Language] = language->getCodeDefinition(def_type, true);

		for(unsigned func_id = FromSqlFunc; func_id <= ToSqlFunc; func_id++)
		{
			if(functions[func_id])
			{
				functions[func_id]->setAttribute(Attributes::RefType, funcs_attr[func_id]);
				attributes[funcs_attr[func_id]] = functions[func_id]->getCodeDefinition(def_type, true);
			}
		}
	}

	return BaseObject::__getCodeDefinition(def_type);
}

QString Transform::getSignature(bool)
{
	return obj_name;
}

QString Transform::getDropDefinition(bool cascade)
{
	attributes[Attributes::Signature] = QString("FOR %1 LANGUAGE %2").arg(~type).arg(language ? language->getName(true) : Attributes::Undefined);
	return BaseObject::getDropDefinition(cascade);
}

void Transform::operator = (Transform &transf)
{
	*(dynamic_cast<BaseObject *>(this))=dynamic_cast<BaseObject &>(transf);
	type = transf.type;
	language = transf.language;
	functions[ToSqlFunc] = transf.functions[ToSqlFunc];
	functions[FromSqlFunc] = transf.functions[FromSqlFunc];
}
