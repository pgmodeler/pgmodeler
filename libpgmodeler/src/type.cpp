/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2017 - Raphael Araújo e Silva <raphael@pgmodeler.com.br>
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

#include "type.h"

Type::Type(void)
{
	obj_type=OBJ_TYPE;
	setConfiguration(ENUMERATION_TYPE);

	attributes[ParsersAttributes::BASE_TYPE]=QString();
	attributes[ParsersAttributes::COMPOSITE_TYPE]=QString();
	attributes[ParsersAttributes::RANGE_TYPE]=QString();
	attributes[ParsersAttributes::TYPE_ATTRIBUTE]=QString();
	attributes[ParsersAttributes::ENUM_TYPE]=QString();
	attributes[ParsersAttributes::ENUMERATIONS]=QString();
	attributes[ParsersAttributes::INPUT_FUNC]=QString();
	attributes[ParsersAttributes::OUTPUT_FUNC]=QString();
	attributes[ParsersAttributes::RECV_FUNC]=QString();
	attributes[ParsersAttributes::SEND_FUNC]=QString();
	attributes[ParsersAttributes::TPMOD_IN_FUNC]=QString();
	attributes[ParsersAttributes::TPMOD_OUT_FUNC]=QString();
	attributes[ParsersAttributes::ANALYZE_FUNC]=QString();
	attributes[ParsersAttributes::INTERNAL_LENGTH]=QString();
	attributes[ParsersAttributes::BY_VALUE]=QString();
	attributes[ParsersAttributes::ALIGNMENT]=QString();
	attributes[ParsersAttributes::STORAGE]=QString();
	attributes[ParsersAttributes::DEFAULT_VALUE]=QString();
	attributes[ParsersAttributes::ELEMENT]=QString();
	attributes[ParsersAttributes::DELIMITER]=QString();
	attributes[ParsersAttributes::REDUCED_FORM]=QString();
	attributes[ParsersAttributes::CATEGORY]=QString();
	attributes[ParsersAttributes::PREFERRED]=QString();
	attributes[ParsersAttributes::LIKE_TYPE]=QString();
	attributes[ParsersAttributes::COLLATABLE]=QString();
	attributes[ParsersAttributes::SUBTYPE]=QString();
	attributes[ParsersAttributes::SUBTYPE_DIFF_FUNC]=QString();
	attributes[ParsersAttributes::CANONICAL_FUNC]=QString();
	attributes[ParsersAttributes::OP_CLASS]=QString();
}

void Type::setName(const QString &name)
{
	QString prev_name;

	prev_name=this->getName(true);//this->nome;
	BaseObject::setName(name);
	PgSQLType::renameUserType(prev_name, this, this->getName(true));
}

void Type::setSchema(BaseObject *schema)
{
	QString prev_name;

	prev_name=this->getName(true);
	BaseObject::setSchema(schema);
	PgSQLType::renameUserType(prev_name, this, this->getName(true));
}

int Type::getAttributeIndex(const QString &attrib_name)
{
	vector<TypeAttribute>::iterator itr, itr_end;
	int idx=-1;

	itr=type_attribs.begin();
	itr_end=type_attribs.end();

	while(itr!=itr_end)
	{
		if(itr->getName()==attrib_name)
		{
			idx=(itr - type_attribs.begin());
			break;
		}

		itr++;
	}

	return(idx);
}

void Type::addAttribute(TypeAttribute attrib)
{
	//Raises an error if the attribute has an empty name or null type
	if(attrib.getName().isEmpty() || attrib.getType()==PgSQLType::null)
		throw Exception(ERR_INS_INV_TYPE_ATTRIB,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	//Raises an error if the passed attribute has the same type as the defining type (this)
	else if(PgSQLType::getUserTypeIndex(this->getName(true), this) == !attrib.getType())
		throw Exception(Exception::getErrorMessage(ERR_USER_TYPE_SELF_REFERENCE).arg(this->getName(true)),
						ERR_USER_TYPE_SELF_REFERENCE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	//Raises an error when the attribute already exists
	else if(getAttributeIndex(attrib.getName()) >= 0)
		throw Exception(ERR_INS_DUPLIC_ITEMS,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	type_attribs.push_back(attrib);
	setCodeInvalidated(true);
}

void Type::removeAttribute(unsigned attrib_idx)
{
	//Raises an error if the attribute index is out of bound
	if(attrib_idx >= type_attribs.size())
		throw Exception(ERR_REF_ATTRIB_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	type_attribs.erase(type_attribs.begin() + attrib_idx);
	setCodeInvalidated(true);
}

void Type::removeAttributes(void)
{
	type_attribs.clear();
	setCodeInvalidated(true);
}

bool Type::isEnumerationExists(const QString &enum_name)
{
	vector<QString>::iterator itr, itr_end;
	bool found=false;

	itr=enumerations.begin();
	itr_end=enumerations.end();

	while(itr!=itr_end && !found)
	{
		found=((*itr)==enum_name);
		itr++;
	}

	return(found);
}

void Type::addEnumeration(const QString &enum_name)
{
	//Raises an error if the enumaration name is empty
	if(enum_name.isEmpty())
		throw Exception(ERR_INS_INV_TYPE_ENUM_ITEM,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	//Raises an error if the enumeration name is invalid (exceeds the maximum length)
	else if(enum_name.size() > BaseObject::OBJECT_NAME_MAX_LENGTH)
		throw Exception(Exception::getErrorMessage(ERR_ASG_ENUM_LONG_NAME).arg(enum_name).arg(this->getName(true)),
						ERR_ASG_ENUM_LONG_NAME,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else if(enum_name.contains(QChar(',')))
		throw Exception(Exception::getErrorMessage(ERR_ASG_ENUM_INV_CHARS).arg(enum_name).arg(this->getName(true)),
						ERR_ASG_ENUM_INV_CHARS,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	//Raises an error if the enumeration already exists
	else if(isEnumerationExists(enum_name))
		throw Exception(ERR_INS_DUPLIC_ENUM_ITEM,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	enumerations.push_back(enum_name);
	setCodeInvalidated(true);
}

void Type::removeEnumeration(unsigned enum_idx)
{
	if(enum_idx >= enumerations.size())
		throw Exception(ERR_REF_ENUM_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	enumerations.erase(enumerations.begin() + enum_idx);
	setCodeInvalidated(true);
}

void Type::removeEnumerations(void)
{
	enumerations.clear();
	setCodeInvalidated(true);
}

void Type::setConfiguration(unsigned conf)
{
	//Raises an error if the configuration type is invalid
	if(conf < BASE_TYPE || conf > RANGE_TYPE)
		throw Exception(ERR_ASG_INV_TYPE_CONFIG,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	type_attribs.clear();
	enumerations.clear();

	for(unsigned idx=0; idx < sizeof(functions)/sizeof(Function *); idx++)
		functions[idx]=nullptr;

	setCollation(nullptr);
	subtype_opclass=nullptr;

	alignment=QString("integer");
	delimiter='\0';
	storage=StorageType::plain;
	element=QString("\"any\"");
	internal_len=0;
	category=CategoryType::userdefined;
	preferred=collatable=by_value=false;
	like_type=QString("\"any\"");

	this->config=conf;
	setCodeInvalidated(true);
}

void Type::setFunction(unsigned func_id, Function *func)
{
	unsigned param_count=0;
	LanguageType lang;
	lang=LanguageType::c;
	unsigned funcs_len=sizeof(functions)/sizeof(Function *);

	//Raises an error if the function id is invalid
	if(func_id >= funcs_len)
		throw Exception(ERR_REF_FUNCTION_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	if(func)
		param_count=func->getParameterCount();

	/* Raises an error if the user try to reference a function id which is incompatible according
	to the type's configuraiton */
	if((config==BASE_TYPE && func_id >= CANONICAL_FUNC) ||
			(config==RANGE_TYPE && func_id <= ANALYZE_FUNC))
		throw Exception(ERR_REF_FUNCTION_INV_TYPE_CONF,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	/* Raises an error if the function isn't defined and the function id is INPUT or OUTPUT,
		because this function is mandatory for base types */
	else if(!func && (func_id==INPUT_FUNC || func_id==OUTPUT_FUNC))
		throw Exception(Exception::getErrorMessage(ERR_ASG_NOT_ALOC_FUNCTION)
						.arg(this->getName(true))
						.arg(BaseObject::getTypeName(OBJ_TYPE)),
						ERR_ASG_NOT_ALOC_FUNCTION,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	else if(func)
	{
		/* Raises an error if the function language is not C.
		 Functions assigned to base type must be written in C */
		if((func_id!=CANONICAL_FUNC && func_id!=SUBTYPE_DIFF_FUNC) &&
				func->getLanguage()->getName()!=~LanguageType(LanguageType::c) &&
				func->getLanguage()->getName()!=~LanguageType(LanguageType::internal))
			throw Exception(ERR_ASG_FUNC_INV_LANGUAGE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		/* Raises an error if the parameter count for INPUT and RECV functions
		 is different from 1 or 3. */
		else if((param_count!=1 && param_count!=3 &&
				 (func_id==INPUT_FUNC || func_id==RECV_FUNC)) ||
				(param_count!=2 && func_id==SUBTYPE_DIFF_FUNC) ||
				(param_count!=1 &&
				 (func_id==OUTPUT_FUNC   || func_id==SEND_FUNC ||
				  func_id==TPMOD_IN_FUNC || func_id==TPMOD_OUT_FUNC ||
				  func_id==ANALYZE_FUNC  || func_id==CANONICAL_FUNC)))
			throw Exception(Exception::getErrorMessage(ERR_ASG_FUNC_INV_PARAM_COUNT)
							.arg(this->getName())
							.arg(BaseObject::getTypeName(OBJ_TYPE)),
							ERR_ASG_FUNC_INV_PARAM_COUNT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		/* Checking the return types of function in relation to type.
		 INPUT, RECV and CANONICAL functions must return the data type that is being defined according to the
		 documentation, but to facilitate the implementation the function must return data type
		 'any' which will be replaced by the defined type name at the moment of generation of SQL.
		 OUTPUT and TPMOD_OUT should return cstring.
		 The other functions SEND, TPMOD_IN and ANALYZE should return bytea, integer and boolean,
		 respectively. Raises an error if some of conditions above is not satisfied. */
		else if((func_id==INPUT_FUNC && func->getReturnType()!=QString("\"any\"")) ||
				(func_id==OUTPUT_FUNC && func->getReturnType()!=QString("cstring")) ||
				(func_id==RECV_FUNC && func->getReturnType()!=QString("\"any\"")) ||
				(func_id==SEND_FUNC && func->getReturnType()!=QString("bytea")) ||
				(func_id==TPMOD_IN_FUNC && func->getReturnType()!=QString("integer")) ||
				(func_id==TPMOD_OUT_FUNC && func->getReturnType()!=QString("cstring")) ||
				(func_id==ANALYZE_FUNC && func->getReturnType()!=QString("boolean")) ||
				(func_id==CANONICAL_FUNC && func->getReturnType()!=QString("\"any\"")) ||
				(func_id==SUBTYPE_DIFF_FUNC && func->getReturnType()!=QString("double precision")))
			throw Exception(Exception::getErrorMessage(ERR_ASG_FUNCTION_INV_RET_TYPE)
							.arg(this->getName())
							.arg(BaseObject::getTypeName(OBJ_TYPE)),
							ERR_ASG_FUNCTION_INV_RET_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		/* Validating the parameter types of function in relation to the type configuration.
		 The INPUT function must have parameters with type (cstring, oid, integer).
		 SEND, OUTPUT, CANONICAL must have a parameter of the same type being defined
		 in this case, to facilitate implementation simply use a type parameter "any".
		 The RECV function must have parameters (internal, oid, integer).
		 The function TPMOD_IN must have a type parameter (ctring []).
		 TPMOD_OUT function must have a parameter of type (integer).
		 The ANALYZE function must have a parameter of type (internal).
		 Raises an error if some of above conditions is not satisfied.*/
		else if((func_id==INPUT_FUNC &&
				 (func->getParameter(0).getType()!=QString("cstring") ||
				  (param_count==3 &&
				   (func->getParameter(1).getType()!=QString("oid") ||
					func->getParameter(2).getType()!=QString("integer"))))) ||
				(func_id==RECV_FUNC &&
				 (func->getParameter(0).getType()!=QString("internal") ||
				  (param_count==3 &&
				   (func->getParameter(1).getType()!=QString("oid") ||
					func->getParameter(2).getType()!=QString("integer"))))) ||
				((func_id==SEND_FUNC || func_id==CANONICAL_FUNC || func_id==OUTPUT_FUNC) && func->getParameter(0).getType()!=QString("\"any\"")) ||
				(func_id==TPMOD_IN_FUNC && *(func->getParameter(0).getType())!=QString("cstring[]")) ||
				(func_id==TPMOD_OUT_FUNC && func->getParameter(0).getType()!=QString("integer")) ||
				(func_id==ANALYZE_FUNC && func->getParameter(0).getType()!=QString("internal")) ||
				(func_id==SUBTYPE_DIFF_FUNC &&
				 (func->getParameter(0).getType()!=this->subtype ||
				  func->getParameter(1).getType()!=this->subtype)))
			throw Exception(Exception::getErrorMessage(ERR_ASG_FUNCTION_INV_PARAMS)
							.arg(this->getName())
							.arg(this->getTypeName()),
							ERR_ASG_FUNCTION_INV_PARAMS,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		func->setProtected(false);
	}

	setCodeInvalidated(functions[func_id] != func);
	functions[func_id]=func;
}

void Type::convertFunctionParameters(bool inverse_conv)
{
	unsigned i, conf_funcs[]={ INPUT_FUNC, RECV_FUNC,
							   OUTPUT_FUNC, SEND_FUNC };
	Parameter param;
	Function *func=nullptr;

	for(i=0; i < 4; i++)
	{
		func=functions[conf_funcs[i]];

		if(func)
		{
			if(conf_funcs[i]==OUTPUT_FUNC || conf_funcs[i]==SEND_FUNC)
			{
				param=func->getParameter(0);
				func->removeParameter(0);

				if(!inverse_conv)
				{
					param.setType(PgSQLType(this));
					func->addParameter(param);
				}
				else
				{
					param.setType(PgSQLType(QString("\"any\"")));
					func->addParameter(param);
				}
			}
			else if(conf_funcs[i]==INPUT_FUNC || conf_funcs[i]==RECV_FUNC)
			{
				if(!inverse_conv)
					func->setReturnType(PgSQLType(this));
				else
					func->setReturnType(PgSQLType(QString("\"any\"")));
			}
		}
	}

	setCodeInvalidated(true);
}

void Type::setInternalLength(unsigned length)
{
	setCodeInvalidated(internal_len != length);
	internal_len=length;
}

void Type::setByValue(bool value)
{
	setCodeInvalidated(by_value != value);
	by_value=value;
}

void Type::setAlignment(PgSQLType type)
{
	QString tp=(*type);

	//Raises an error if the type assigned to the alignment is invalid according to the rule
	if(tp!=QString("char") && tp!=QString("smallint") && tp!=QString("integer") && tp!=QString("double precision"))
		throw Exception(Exception::getErrorMessage(ERR_ASG_INV_ALIGNMENT_TYPE).arg(this->getName(true)),
						ERR_ASG_INV_ALIGNMENT_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	setCodeInvalidated(alignment != type);
	alignment=tp;
}

void Type::setStorage(StorageType strg)
{
	setCodeInvalidated(storage != strg);
	storage=strg;
}

void Type::setDefaultValue(const QString &value)
{
	QString def=value.trimmed();

	setCodeInvalidated(default_value != def);
	this->default_value=def;
}

void Type::setElement(PgSQLType elem)
{
	if(PgSQLType::getUserTypeIndex(this->getName(true), this) == !elem)
		throw Exception(Exception::getErrorMessage(ERR_USER_TYPE_SELF_REFERENCE).arg(this->getName(true)),
						ERR_USER_TYPE_SELF_REFERENCE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else if(elem!=QString("\"any\"") &&
			(elem.isOIDType() || elem.isPseudoType() ||
			 elem.isUserType() || elem.isArrayType()))
		throw Exception(Exception::getErrorMessage(ERR_ASG_INV_ELEMENT_TYPE).arg(this->getName(true)),
						ERR_ASG_INV_ELEMENT_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	setCodeInvalidated(element != elem);
	this->element=elem;
}

void Type::setDelimiter(char delim)
{
	setCodeInvalidated(delimiter != delim);
	delimiter=delim;
}

void Type::setElementsAttribute(unsigned def_type)
{
	QString str_elem;
	unsigned i, count;

	count=type_attribs.size();
	for(i=0; i < count; i++)
		str_elem+=type_attribs[i].getCodeDefinition(def_type);

	if(def_type==SchemaParser::SQL_DEFINITION)
		str_elem.remove(str_elem.lastIndexOf(','), str_elem.size());

	attributes[ParsersAttributes::TYPE_ATTRIBUTE]=str_elem;
}

void Type::setEnumerationsAttribute(unsigned def_type)
{
	QString str_enum;
	unsigned i, count;

	count=enumerations.size();
	for(i=0; i < count; i++)
	{
		if(def_type==SchemaParser::SQL_DEFINITION)
			str_enum+=QString("'") + enumerations[i] + QString("'");
		else
			str_enum+=enumerations[i];

		if(i < (count-1)) str_enum+=QString(",");
	}

	attributes[ParsersAttributes::ENUMERATIONS]=str_enum;
}

void Type::setCategory(CategoryType categ)
{
	setCodeInvalidated(category != categ);
	this->category=categ;
}

void Type::setPreferred(bool value)
{
	setCodeInvalidated(preferred != value);
	this->preferred=value;
}

void Type::setCollatable(bool value)
{
	setCodeInvalidated(collatable != value);
	this->collatable=value;
}

void Type::setLikeType(PgSQLType like_type)
{
	if(PgSQLType::getUserTypeIndex(this->getName(true), this) == !like_type)
		throw Exception(Exception::getErrorMessage(ERR_USER_TYPE_SELF_REFERENCE).arg(this->getName(true)),
						ERR_USER_TYPE_SELF_REFERENCE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	setCodeInvalidated(this->like_type != like_type);
	this->like_type=like_type;
}

void Type::setSubtype(PgSQLType subtype)
{
	if(PgSQLType::getUserTypeIndex(this->getName(true), this) == !subtype)
		throw Exception(Exception::getErrorMessage(ERR_USER_TYPE_SELF_REFERENCE).arg(this->getName(true)),
						ERR_USER_TYPE_SELF_REFERENCE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	setCodeInvalidated(this->subtype != subtype);
	this->subtype=subtype;
}

void Type::setSubtypeOpClass(OperatorClass *opclass)
{
	if(opclass && opclass->getIndexingType()!=IndexingType::btree)
		throw Exception(Exception::getErrorMessage(ERR_ASG_INV_OPCLASS_OBJ)
						.arg(this->getName(true))
						.arg(this->getTypeName()),
						ERR_ASG_INV_OPCLASS_OBJ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	setCodeInvalidated(subtype_opclass != opclass);
	subtype_opclass=opclass;
}

TypeAttribute Type::getAttribute(unsigned attrib_idx)
{
	if(attrib_idx >= type_attribs.size())
		throw Exception(ERR_REF_ATTRIB_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return(type_attribs[attrib_idx]);
}

unsigned Type::getAttributeCount(void)
{
	return(type_attribs.size());
}

QString Type::getEnumeration(unsigned idx_enum)
{
	if(idx_enum >= enumerations.size())
		throw Exception(ERR_REF_ENUM_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return(enumerations[idx_enum]);
}

unsigned Type::getEnumerationCount(void)
{
	return(enumerations.size());
}

Function *Type::getFunction(unsigned func_id)
{
	if(func_id >= sizeof(functions)/sizeof(Function *))
		throw Exception(ERR_REF_FUNCTION_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return(functions[func_id]);
}

unsigned Type::getInternalLength(void)
{
	return(internal_len);
}

bool Type::isByValue(void)
{
	return(by_value);
}

PgSQLType Type::getAlignment(void)
{
	return(alignment);
}

StorageType Type::getStorage(void)
{
	return(storage);
}

QString Type::getDefaultValue(void)
{
	return(default_value);
}

PgSQLType Type::getElement(void)
{
	return(element);
}

char Type::getDelimiter(void)
{
	return(delimiter);
}

unsigned Type::getConfiguration(void)
{
	return(config);
}

CategoryType Type::getCategory(void)
{
	return(category);
}

bool Type::isPreferred(void)
{
	return(preferred);
}

bool Type::isCollatable(void)
{
	return(collatable);
}

PgSQLType Type::getLikeType(void)
{
	return(like_type);
}

PgSQLType Type::getSubtype(void)
{
	return(subtype);
}

OperatorClass *Type::getSubtypeOpClass(void)
{
	return(subtype_opclass);
}

QString Type::getCodeDefinition(unsigned def_type)
{
	return(this->getCodeDefinition(def_type, false));
}

QString Type::getCodeDefinition(unsigned def_type, bool reduced_form)
{
	QString code_def=getCachedCode(def_type, reduced_form);
	if(!code_def.isEmpty()) return(code_def);

	if(config==ENUMERATION_TYPE)
	{
		attributes[ParsersAttributes::ENUM_TYPE]=ParsersAttributes::_TRUE_;
		setEnumerationsAttribute(def_type);
	}
	else if(config==COMPOSITE_TYPE)
	{
		attributes[ParsersAttributes::COMPOSITE_TYPE]=ParsersAttributes::_TRUE_;
		setElementsAttribute(def_type);
	}
	else if(config==RANGE_TYPE)
	{
		attributes[ParsersAttributes::RANGE_TYPE]=ParsersAttributes::_TRUE_;

		if(def_type==SchemaParser::SQL_DEFINITION)
			attributes[ParsersAttributes::SUBTYPE]=(*subtype);
		else
			attributes[ParsersAttributes::SUBTYPE]=subtype.getCodeDefinition(SchemaParser::XML_DEFINITION);

		if(subtype_opclass)
		{
			if(def_type==SchemaParser::SQL_DEFINITION)
				attributes[ParsersAttributes::OP_CLASS]=subtype_opclass->getName(true);
			else
				attributes[ParsersAttributes::OP_CLASS]=subtype_opclass->getCodeDefinition(def_type, true);
		}
	}
	else
	{
		attributes[ParsersAttributes::BASE_TYPE]=ParsersAttributes::_TRUE_;

		if(internal_len==0 && def_type==SchemaParser::SQL_DEFINITION)
			attributes[ParsersAttributes::INTERNAL_LENGTH]=QString("VARIABLE");
		else
			attributes[ParsersAttributes::INTERNAL_LENGTH]=QString("%1").arg(internal_len);

		attributes[ParsersAttributes::BY_VALUE]=(by_value ? ParsersAttributes::_TRUE_ : QString());
		attributes[ParsersAttributes::ALIGNMENT]=(*alignment);
		attributes[ParsersAttributes::STORAGE]=(~storage);
		attributes[ParsersAttributes::DEFAULT_VALUE]=default_value;

		if(element!=QString("\"any\""))
			attributes[ParsersAttributes::ELEMENT]=(*element);

		if(delimiter!='\0')
			attributes[ParsersAttributes::DELIMITER]=delimiter;

		attributes[ParsersAttributes::CATEGORY]=~(category);

		attributes[ParsersAttributes::PREFERRED]=(preferred ? ParsersAttributes::_TRUE_ : QString());
		attributes[ParsersAttributes::COLLATABLE]=(collatable ? ParsersAttributes::_TRUE_ : QString());

		if(like_type!=QString("\"any\""))
		{
			if(def_type==SchemaParser::SQL_DEFINITION)
				attributes[ParsersAttributes::LIKE_TYPE]=(*like_type);
			else
				attributes[ParsersAttributes::LIKE_TYPE]=like_type.getCodeDefinition(SchemaParser::XML_DEFINITION);
		}
	}

	if(config==BASE_TYPE || config==RANGE_TYPE)
	{
		unsigned i;
		QString func_attrib[]={ParsersAttributes::INPUT_FUNC,
							   ParsersAttributes::OUTPUT_FUNC,
							   ParsersAttributes::RECV_FUNC,
							   ParsersAttributes::SEND_FUNC,
							   ParsersAttributes::TPMOD_IN_FUNC,
							   ParsersAttributes::TPMOD_OUT_FUNC,
							   ParsersAttributes::ANALYZE_FUNC,
							   ParsersAttributes::CANONICAL_FUNC,
							   ParsersAttributes::SUBTYPE_DIFF_FUNC};

		for(i=0; i < sizeof(functions)/sizeof(Function *); i++)
		{
			if(functions[i])
			{
				if(def_type==SchemaParser::SQL_DEFINITION)
					attributes[func_attrib[i]]=functions[i]->getName();
				else
				{
					functions[i]->setAttribute(ParsersAttributes::REF_TYPE, func_attrib[i]);
					attributes[func_attrib[i]]=functions[i]->getCodeDefinition(def_type, true);
				}
			}
		}
	}

	return(BaseObject::getCodeDefinition(def_type, reduced_form));
}

QString Type::getAlterDefinition(BaseObject *object)
{
	Type *type=dynamic_cast<Type *>(object);

	if(!type)
		throw Exception(ERR_OPR_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	try
	{
		attribs_map attribs;
		QString alter_def, prev_val;
		int attrib_idx=-1;

		alter_def=BaseObject::getAlterDefinition(object);

		if(this->config==type->config)
		{
			if(config==ENUMERATION_TYPE)
			{
				for(QString enum_val : type->enumerations)
				{
					if(std::find(this->enumerations.begin(), this->enumerations.end(), enum_val)==this->enumerations.end())
					{
						attribs[ParsersAttributes::BEFORE]=QString();
						if(prev_val.isEmpty())
						{
							attribs[ParsersAttributes::BEFORE]=ParsersAttributes::_TRUE_;
							prev_val=this->enumerations[0];
						}

						attribs[ParsersAttributes::VALUE]=enum_val;
						attribs[ParsersAttributes::EXISTING_VALUE]=prev_val;
						copyAttributes(attribs);
						alter_def+=BaseObject::getAlterDefinition(this->getSchemaName(), attributes, true, true);
						attribs.clear();
					}

					prev_val=enum_val;
				}
			}
			else if(config==COMPOSITE_TYPE)
			{
				//Removing type attributes
				for(TypeAttribute attrib : this->type_attribs)
				{
					if(type->getAttributeIndex(attrib.getName()) < 0)
					{
						attribs[ParsersAttributes::DROP]=ParsersAttributes::_TRUE_;
						attribs[ParsersAttributes::ATTRIBUTE]=attrib.getName(true);
						copyAttributes(attribs);
						alter_def+=BaseObject::getAlterDefinition(this->getSchemaName(), attributes, true, true);
						attribs.clear();
						attributes[ParsersAttributes::DROP]=QString();
					}
				}

				for(TypeAttribute attrib : type->type_attribs)
				{
					attrib_idx=this->getAttributeIndex(attrib.getName());

					//Creating type attributes
					if(attrib_idx < 0)
					{
						attribs[ParsersAttributes::ATTRIBUTE]=attrib.getName(true);
						attribs[ParsersAttributes::TYPE]=attrib.getType().getCodeDefinition(SchemaParser::SQL_DEFINITION);
						attribs[ParsersAttributes::COLLATION]=QString();

						if(attrib.getCollation())
							attribs[ParsersAttributes::COLLATION]=attrib.getCollation()->getName(true);

						copyAttributes(attribs);
						alter_def+=BaseObject::getAlterDefinition(this->getSchemaName(), attributes, true, true);
					}
					//Changing type attributes
					else
					{
						attribs[ParsersAttributes::CHANGE]=ParsersAttributes::_TRUE_;

						if(!type_attribs[attrib_idx].getType().isEquivalentTo(attrib.getType()))
						{
							attribs[ParsersAttributes::ATTRIBUTE]=attrib.getName(true);
							attribs[ParsersAttributes::TYPE]=attrib.getType().getCodeDefinition(SchemaParser::SQL_DEFINITION);
						}

						copyAttributes(attribs);
						alter_def+=BaseObject::getAlterDefinition(this->getSchemaName(), attributes, true, true);
						attributes[ParsersAttributes::CHANGE]=QString();
					}

					attribs.clear();
				}
			}
		}

		return(alter_def);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

void Type::operator = (Type &type)
{
	QString prev_name;
	unsigned i=0;

	prev_name=this->getName(true);
	*(dynamic_cast<BaseObject *>(this))=dynamic_cast<BaseObject &>(type);

	this->config=type.config;
	this->type_attribs=type.type_attribs;
	this->enumerations=type.enumerations;
	this->internal_len=type.internal_len;
	this->by_value=type.by_value;
	this->alignment=type.alignment;
	this->element=type.element;
	this->storage=type.storage;
	this->default_value=type.default_value;
	this->category=type.category;
	this->preferred=type.preferred;
	this->like_type=type.like_type;
	this->delimiter=type.delimiter;
	this->collatable=type.collatable;
	this->subtype=type.subtype;
	this->subtype_opclass=type.subtype_opclass;

	while(i < sizeof(functions)/sizeof(Function *))
	{
		this->functions[i]=type.functions[i];
		i++;
	}

	PgSQLType::renameUserType(prev_name, this, this->getName(true));
}

