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

#include "type.h"
#include "defaultlanguages.h"

Type::Type()
{
	obj_type=ObjectType::Type;
	setConfiguration(EnumerationType);

	attributes[Attributes::BaseType]=QString();
	attributes[Attributes::CompositeType]=QString();
	attributes[Attributes::RangeType]=QString();
	attributes[Attributes::TypeAttribute]=QString();
	attributes[Attributes::EnumType]=QString();
	attributes[Attributes::Enumerations]=QString();
	attributes[Attributes::InputFunc]=QString();
	attributes[Attributes::OutputFunc]=QString();
	attributes[Attributes::RecvFunc]=QString();
	attributes[Attributes::SendFunc]=QString();
	attributes[Attributes::TpmodInFunc]=QString();
	attributes[Attributes::TpmodOutFunc]=QString();
	attributes[Attributes::AnalyzeFunc]=QString();
	attributes[Attributes::InternalLength]=QString();
	attributes[Attributes::ByValue]=QString();
	attributes[Attributes::Alignment]=QString();
	attributes[Attributes::Storage]=QString();
	attributes[Attributes::DefaultValue]=QString();
	attributes[Attributes::Element]=QString();
	attributes[Attributes::Delimiter]=QString();
	attributes[Attributes::ReducedForm]=QString();
	attributes[Attributes::Category]=QString();
	attributes[Attributes::Preferred]=QString();
	attributes[Attributes::LikeType]=QString();
	attributes[Attributes::Collatable]=QString();
	attributes[Attributes::Subtype]=QString();
	attributes[Attributes::SubtypeDiffFunc]=QString();
	attributes[Attributes::CanonicalFunc]=QString();
	attributes[Attributes::OpClass]=QString();
}

void Type::setName(const QString &name)
{
	QString prev_name;

	prev_name=this->getName(true);//this->nome;
	BaseObject::setName(name);
	PgSqlType::renameUserType(prev_name, this, this->getName(true));
}

void Type::setSchema(BaseObject *schema)
{
	QString prev_name;

	prev_name=this->getName(true);
	BaseObject::setSchema(schema);
	PgSqlType::renameUserType(prev_name, this, this->getName(true));
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

	return idx;
}

void Type::addAttribute(TypeAttribute attrib)
{
	//Raises an error if the attribute has an empty name or null type
	if(attrib.getName().isEmpty() || attrib.getType()==PgSqlType::Null)
		throw Exception(ErrorCode::InsInvalidTypeAttribute,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	//Raises an error if the passed attribute has the same type as the defining type (this)
	else if(PgSqlType::getUserTypeIndex(this->getName(true), this) == !attrib.getType())
		throw Exception(Exception::getErrorMessage(ErrorCode::InvUserTypeSelfReference).arg(this->getName(true)),
						ErrorCode::InvUserTypeSelfReference,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	//Raises an error when the attribute already exists
	else if(getAttributeIndex(attrib.getName()) >= 0)
		throw Exception(ErrorCode::InsDuplicatedItems,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	type_attribs.push_back(attrib);
	setCodeInvalidated(true);
}

void Type::removeAttribute(unsigned attrib_idx)
{
	//Raises an error if the attribute index is out of bound
	if(attrib_idx >= type_attribs.size())
		throw Exception(ErrorCode::RefAttributeInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	type_attribs.erase(type_attribs.begin() + attrib_idx);
	setCodeInvalidated(true);
}

void Type::removeAttributes()
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

	return found;
}

void Type::addEnumeration(const QString &enum_name)
{
	//Raises an error if the enumaration name is empty
	if(enum_name.isEmpty())
		throw Exception(ErrorCode::InsInvalidEnumerationItem,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	//Raises an error if the enumeration name is invalid (exceeds the maximum length)
	else if(enum_name.size() > BaseObject::ObjectNameMaxLength)
		throw Exception(Exception::getErrorMessage(ErrorCode::AsgEnumLongName).arg(enum_name).arg(this->getName(true)),
						ErrorCode::AsgEnumLongName,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else if(enum_name.contains(QChar(',')))
		throw Exception(Exception::getErrorMessage(ErrorCode::AsgEnumInvalidChars).arg(enum_name).arg(this->getName(true)),
						ErrorCode::AsgEnumInvalidChars,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	//Raises an error if the enumeration already exists
	else if(isEnumerationExists(enum_name))
		throw Exception(ErrorCode::InsDuplicatedEnumerationItem,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	enumerations.push_back(enum_name);
	setCodeInvalidated(true);
}

void Type::removeEnumeration(unsigned enum_idx)
{
	if(enum_idx >= enumerations.size())
		throw Exception(ErrorCode::RefEnumerationInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	enumerations.erase(enumerations.begin() + enum_idx);
	setCodeInvalidated(true);
}

void Type::removeEnumerations()
{
	enumerations.clear();
	setCodeInvalidated(true);
}

void Type::setConfiguration(unsigned conf)
{
	//Raises an error if the configuration type is invalid
	if(conf < BaseType || conf > RangeType)
		throw Exception(ErrorCode::AsgInvalidTypeConfiguration,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	type_attribs.clear();
	enumerations.clear();

	for(unsigned idx=0; idx < sizeof(functions)/sizeof(Function *); idx++)
		functions[idx]=nullptr;

	setCollation(nullptr);
	subtype_opclass=nullptr;

	alignment=QString("integer");
	delimiter='\0';
	storage=StorageType::Plain;
	element=QString("\"any\"");
	internal_len=0;
	category=CategoryType::UserDefined;
	preferred=collatable=by_value=false;
	like_type=QString("\"any\"");

	this->config=conf;
	setCodeInvalidated(true);
}

void Type::setFunction(unsigned func_id, Function *func)
{
	unsigned param_count=0;
	unsigned funcs_len=sizeof(functions)/sizeof(Function *);

	//Raises an error if the function id is invalid
	if(func_id >= funcs_len)
		throw Exception(ErrorCode::RefFunctionInvalidType,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	if(func)
		param_count=func->getParameterCount();

	/* Raises an error if the user try to reference a function id which is incompatible according
	to the type's configuraiton */
	if((config==BaseType && func_id >= CanonicalFunc) ||
			(config==RangeType && func_id <= AnalyzeFunc))
		throw Exception(ErrorCode::RefInvalidFunctionIdTypeConfig,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	/* Raises an error if the function isn't defined and the function id is INPUT or OUTPUT,
		because this function is mandatory for base types */
	else if(!func && (func_id==InputFunc || func_id==OutputFunc))
		throw Exception(Exception::getErrorMessage(ErrorCode::AsgNotAllocatedFunction)
						.arg(this->getName(true))
						.arg(BaseObject::getTypeName(ObjectType::Type)),
						ErrorCode::AsgNotAllocatedFunction,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	else if(func)
	{
		/* Raises an error if the function language is not C.
		 Functions assigned to base type must be written in C */
		if((func_id!=CanonicalFunc && func_id!=SubtypeDiffFunc) &&
				func->getLanguage()->getName().toLower() != DefaultLanguages::C &&
				func->getLanguage()->getName().toLower() != DefaultLanguages::Internal)
			throw Exception(ErrorCode::AsgFunctionInvalidLanguage,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		/* Raises an error if the parameter count for INPUT and RECV functions
		 is different from 1 or 3. */
		else if((param_count!=1 && param_count!=3 &&
				 (func_id==InputFunc || func_id==RecvFunc)) ||
				(param_count!=2 && func_id==SubtypeDiffFunc) ||
				(param_count!=1 &&
				 (func_id==OutputFunc   || func_id==SendFunc ||
				  func_id==TpmodInFunc || func_id==TpmodOutFunc ||
				  func_id==AnalyzeFunc  || func_id==CanonicalFunc)))
			throw Exception(Exception::getErrorMessage(ErrorCode::AsgFunctionInvalidParamCount)
							.arg(this->getName())
							.arg(BaseObject::getTypeName(ObjectType::Type)),
							ErrorCode::AsgFunctionInvalidParamCount,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		/* Checking the return types of function in relation to type.
		 INPUT, RECV and CANONICAL functions must return the data type that is being defined according to the
		 documentation, but to facilitate the implementation the function must return data type
		 'any' which will be replaced by the defined type name at the moment of generation of SQL.
		 OUTPUT and TPMOD_OUT should return cstring.
		 The other functions SEND, TPMOD_IN and ANALYZE should return bytea, integer and boolean,
		 respectively. Raises an error if some of conditions above is not satisfied. */
		else if((func_id==InputFunc && func->getReturnType()!=QString("\"any\"")) ||
				(func_id==OutputFunc && func->getReturnType()!=QString("cstring")) ||
				(func_id==RecvFunc && func->getReturnType()!=QString("\"any\"")) ||
				(func_id==SendFunc && func->getReturnType()!=QString("bytea")) ||
				(func_id==TpmodInFunc && func->getReturnType()!=QString("integer")) ||
				(func_id==TpmodOutFunc && func->getReturnType()!=QString("cstring")) ||
				(func_id==AnalyzeFunc && func->getReturnType()!=QString("boolean")) ||
				(func_id==CanonicalFunc && func->getReturnType()!=QString("\"any\"")) ||
				(func_id==SubtypeDiffFunc && func->getReturnType()!=QString("double precision")))
			throw Exception(Exception::getErrorMessage(ErrorCode::AsgFunctionInvalidReturnType)
							.arg(this->getName())
							.arg(BaseObject::getTypeName(ObjectType::Type)),
							ErrorCode::AsgFunctionInvalidReturnType,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		/* Validating the parameter types of function in relation to the type configuration.
		 The INPUT function must have parameters with type (cstring, oid, integer).
		 SEND, OUTPUT, CANONICAL must have a parameter of the same type being defined
		 in this case, to facilitate implementation simply use a type parameter "any".
		 The RECV function must have parameters (internal, oid, integer).
		 The function TPMOD_IN must have a type parameter (ctring []).
		 TPMOD_OUT function must have a parameter of type (integer).
		 The ANALYZE function must have a parameter of type (internal).
		 Raises an error if some of above conditions is not satisfied.*/
		else if((func_id==InputFunc &&
				 (func->getParameter(0).getType()!=QString("cstring") ||
				  (param_count==3 &&
				   (func->getParameter(1).getType()!=QString("oid") ||
					func->getParameter(2).getType()!=QString("integer"))))) ||
				(func_id==RecvFunc &&
				 (func->getParameter(0).getType()!=QString("internal") ||
				  (param_count==3 &&
				   (func->getParameter(1).getType()!=QString("oid") ||
					func->getParameter(2).getType()!=QString("integer"))))) ||
				((func_id==SendFunc || func_id==CanonicalFunc || func_id==OutputFunc) && func->getParameter(0).getType()!=QString("\"any\"")) ||
				(func_id==TpmodInFunc && *(func->getParameter(0).getType())!=QString("cstring[]")) ||
				(func_id==TpmodOutFunc && func->getParameter(0).getType()!=QString("integer")) ||
				(func_id==AnalyzeFunc && func->getParameter(0).getType()!=QString("internal")) ||
				(func_id==SubtypeDiffFunc &&
				 (func->getParameter(0).getType()!=this->subtype ||
				  func->getParameter(1).getType()!=this->subtype)))
			throw Exception(Exception::getErrorMessage(ErrorCode::AsgFunctionInvalidParameters)
							.arg(this->getName())
							.arg(this->getTypeName()),
							ErrorCode::AsgFunctionInvalidParameters,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		func->setProtected(false);
	}

	setCodeInvalidated(functions[func_id] != func);
	functions[func_id]=func;
}

void Type::convertFunctionParameters(bool inverse_conv)
{
	unsigned i, conf_funcs[]={ InputFunc, RecvFunc,
							   OutputFunc, SendFunc };
	Parameter param;
	Function *func=nullptr;

	for(i=0; i < 4; i++)
	{
		func=functions[conf_funcs[i]];

		if(func)
		{
			if(conf_funcs[i]==OutputFunc || conf_funcs[i]==SendFunc)
			{
				param=func->getParameter(0);
				func->removeParameter(0);

				if(!inverse_conv)
				{
					param.setType(PgSqlType(this));
					func->addParameter(param);
				}
				else
				{
					param.setType(PgSqlType(QString("\"any\"")));
					func->addParameter(param);
				}
			}
			else if(conf_funcs[i]==InputFunc || conf_funcs[i]==RecvFunc)
			{
				if(!inverse_conv)
					func->setReturnType(PgSqlType(this));
				else
					func->setReturnType(PgSqlType(QString("\"any\"")));
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

void Type::setAlignment(PgSqlType type)
{
	QString tp=(*type);

	//Raises an error if the type assigned to the alignment is invalid according to the rule
	if(tp!=QString("char") && tp!=QString("smallint") && tp!=QString("integer") && tp!=QString("double precision"))
		throw Exception(Exception::getErrorMessage(ErrorCode::AsgInvalidAlignmentType).arg(this->getName(true)),
						ErrorCode::AsgInvalidAlignmentType,__PRETTY_FUNCTION__,__FILE__,__LINE__);

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

void Type::setElement(PgSqlType elem)
{
	if(PgSqlType::getUserTypeIndex(this->getName(true), this) == !elem)
		throw Exception(Exception::getErrorMessage(ErrorCode::InvUserTypeSelfReference).arg(this->getName(true)),
						ErrorCode::InvUserTypeSelfReference,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else if(elem!=QString("\"any\"") &&
			(elem.isOIDType() || elem.isPseudoType() ||
			 elem.isUserType() || elem.isArrayType()))
		throw Exception(Exception::getErrorMessage(ErrorCode::AsgInvalidElementType).arg(this->getName(true)),
						ErrorCode::AsgInvalidElementType,__PRETTY_FUNCTION__,__FILE__,__LINE__);

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

	if(def_type==SchemaParser::SqlDefinition)
		str_elem.remove(str_elem.lastIndexOf(','), str_elem.size());

	attributes[Attributes::TypeAttribute]=str_elem;
}

void Type::setEnumerationsAttribute(unsigned def_type)
{
	QString str_enum;
	unsigned i, count;

	count=enumerations.size();
	for(i=0; i < count; i++)
	{
		if(def_type==SchemaParser::SqlDefinition)
			str_enum+=QString("'") + enumerations[i] + QString("'");
		else
			str_enum+=enumerations[i];

		if(i < (count-1)) str_enum+=QString(",");
	}

	attributes[Attributes::Enumerations]=str_enum;
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

void Type::setLikeType(PgSqlType like_type)
{
	if(PgSqlType::getUserTypeIndex(this->getName(true), this) == !like_type)
		throw Exception(Exception::getErrorMessage(ErrorCode::InvUserTypeSelfReference).arg(this->getName(true)),
						ErrorCode::InvUserTypeSelfReference,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	setCodeInvalidated(this->like_type != like_type);
	this->like_type=like_type;
}

void Type::setSubtype(PgSqlType subtype)
{
	if(PgSqlType::getUserTypeIndex(this->getName(true), this) == !subtype)
		throw Exception(Exception::getErrorMessage(ErrorCode::InvUserTypeSelfReference).arg(this->getName(true)),
						ErrorCode::InvUserTypeSelfReference,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	setCodeInvalidated(this->subtype != subtype);
	this->subtype=subtype;
}

void Type::setSubtypeOpClass(OperatorClass *opclass)
{
	if(opclass && opclass->getIndexingType()!=IndexingType::Btree)
		throw Exception(Exception::getErrorMessage(ErrorCode::AsgInvalidOpClassObject)
						.arg(this->getName(true))
						.arg(this->getTypeName()),
						ErrorCode::AsgInvalidOpClassObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	setCodeInvalidated(subtype_opclass != opclass);
	subtype_opclass=opclass;
}

TypeAttribute Type::getAttribute(unsigned attrib_idx)
{
	if(attrib_idx >= type_attribs.size())
		throw Exception(ErrorCode::RefAttributeInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return type_attribs[attrib_idx];
}

unsigned Type::getAttributeCount()
{
	return type_attribs.size();
}

QString Type::getEnumeration(unsigned idx_enum)
{
	if(idx_enum >= enumerations.size())
		throw Exception(ErrorCode::RefEnumerationInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return enumerations[idx_enum];
}

unsigned Type::getEnumerationCount()
{
	return enumerations.size();
}

Function *Type::getFunction(unsigned func_id)
{
	if(func_id >= sizeof(functions)/sizeof(Function *))
		throw Exception(ErrorCode::RefFunctionInvalidType,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return functions[func_id];
}

unsigned Type::getInternalLength()
{
	return internal_len;
}

bool Type::isByValue()
{
	return by_value;
}

PgSqlType Type::getAlignment()
{
	return alignment;
}

StorageType Type::getStorage()
{
	return storage;
}

QString Type::getDefaultValue()
{
	return default_value;
}

PgSqlType Type::getElement()
{
	return element;
}

char Type::getDelimiter()
{
	return delimiter;
}

unsigned Type::getConfiguration()
{
	return config;
}

CategoryType Type::getCategory()
{
	return category;
}

bool Type::isPreferred()
{
	return preferred;
}

bool Type::isCollatable()
{
	return collatable;
}

PgSqlType Type::getLikeType()
{
	return like_type;
}

PgSqlType Type::getSubtype()
{
	return subtype;
}

OperatorClass *Type::getSubtypeOpClass()
{
	return subtype_opclass;
}

QString Type::getCodeDefinition(unsigned def_type)
{
	return this->getCodeDefinition(def_type, false);
}

QString Type::getCodeDefinition(unsigned def_type, bool reduced_form)
{
	QString code_def=getCachedCode(def_type, reduced_form);
	if(!code_def.isEmpty()) return code_def;

	if(config==EnumerationType)
	{
		attributes[Attributes::EnumType]=Attributes::True;
		setEnumerationsAttribute(def_type);
	}
	else if(config==CompositeType)
	{
		attributes[Attributes::CompositeType]=Attributes::True;
		setElementsAttribute(def_type);
	}
	else if(config==RangeType)
	{
		attributes[Attributes::RangeType]=Attributes::True;

		if(def_type==SchemaParser::SqlDefinition)
			attributes[Attributes::Subtype]=(*subtype);
		else
			attributes[Attributes::Subtype]=subtype.getCodeDefinition(SchemaParser::XmlDefinition);

		if(subtype_opclass)
		{
			if(def_type==SchemaParser::SqlDefinition)
				attributes[Attributes::OpClass]=subtype_opclass->getName(true);
			else
				attributes[Attributes::OpClass]=subtype_opclass->getCodeDefinition(def_type, true);
		}
	}
	else
	{
		attributes[Attributes::BaseType]=Attributes::True;

		if(internal_len==0 && def_type==SchemaParser::SqlDefinition)
			attributes[Attributes::InternalLength]=QString("VARIABLE");
		else
			attributes[Attributes::InternalLength]=QString("%1").arg(internal_len);

		attributes[Attributes::ByValue]=(by_value ? Attributes::True : QString());
		attributes[Attributes::Alignment]=(*alignment);
		attributes[Attributes::Storage]=(~storage);
		attributes[Attributes::DefaultValue]=default_value;

		if(element!=QString("\"any\""))
			attributes[Attributes::Element]=(*element);

		if(delimiter!='\0')
			attributes[Attributes::Delimiter]=delimiter;

		attributes[Attributes::Category]=~(category);

		attributes[Attributes::Preferred]=(preferred ? Attributes::True : QString());
		attributes[Attributes::Collatable]=(collatable ? Attributes::True : QString());

		if(like_type!=QString("\"any\""))
		{
			if(def_type==SchemaParser::SqlDefinition)
				attributes[Attributes::LikeType]=(*like_type);
			else
				attributes[Attributes::LikeType]=like_type.getCodeDefinition(SchemaParser::XmlDefinition);
		}
	}

	if(config==BaseType || config==RangeType)
	{
		unsigned i;
		QString func_attrib[]={Attributes::InputFunc,
							   Attributes::OutputFunc,
							   Attributes::RecvFunc,
							   Attributes::SendFunc,
							   Attributes::TpmodInFunc,
							   Attributes::TpmodOutFunc,
							   Attributes::AnalyzeFunc,
							   Attributes::CanonicalFunc,
							   Attributes::SubtypeDiffFunc};

		for(i=0; i < sizeof(functions)/sizeof(Function *); i++)
		{
			if(functions[i])
			{
				if(def_type==SchemaParser::SqlDefinition)
					attributes[func_attrib[i]]=functions[i]->getName();
				else
				{
					functions[i]->setAttribute(Attributes::RefType, func_attrib[i]);
					attributes[func_attrib[i]]=functions[i]->getCodeDefinition(def_type, true);
				}
			}
		}
	}

	return BaseObject::getCodeDefinition(def_type, reduced_form);
}

QString Type::getAlterDefinition(BaseObject *object)
{
	Type *type=dynamic_cast<Type *>(object);

	if(!type)
		throw Exception(ErrorCode::OprNotAllocatedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	try
	{
		attribs_map attribs;
		QString alter_def, prev_val;
		int attrib_idx=-1;

		alter_def=BaseObject::getAlterDefinition(object);

		if(this->config==type->config)
		{
			if(config==EnumerationType)
			{
				for(QString enum_val : type->enumerations)
				{
					if(std::find(this->enumerations.begin(), this->enumerations.end(), enum_val)==this->enumerations.end())
					{
						attribs[Attributes::Before]=QString();
						if(prev_val.isEmpty())
						{
							attribs[Attributes::Before]=Attributes::True;
							prev_val=this->enumerations[0];
						}

						attribs[Attributes::Value]=enum_val;
						attribs[Attributes::ExistingValue]=prev_val;
						copyAttributes(attribs);
						alter_def+=BaseObject::getAlterDefinition(this->getSchemaName(), attributes, true, true);
						attribs.clear();
					}

					prev_val=enum_val;
				}
			}
			else if(config==CompositeType)
			{
				//Removing type attributes
				for(TypeAttribute attrib : this->type_attribs)
				{
					if(type->getAttributeIndex(attrib.getName()) < 0)
					{
						attribs[Attributes::Drop]=Attributes::True;
						attribs[Attributes::Attribute]=attrib.getName(true);
						copyAttributes(attribs);
						alter_def+=BaseObject::getAlterDefinition(this->getSchemaName(), attributes, true, true);
						attribs.clear();
						attributes[Attributes::Drop]=QString();
					}
				}

				for(TypeAttribute attrib : type->type_attribs)
				{
					attrib_idx=this->getAttributeIndex(attrib.getName());

					//Creating type attributes
					if(attrib_idx < 0)
					{
						attribs[Attributes::Attribute]=attrib.getName(true);
						attribs[Attributes::Type]=attrib.getType().getCodeDefinition(SchemaParser::SqlDefinition);
						attribs[Attributes::Collation]=QString();

						if(attrib.getCollation())
							attribs[Attributes::Collation]=attrib.getCollation()->getName(true);

						copyAttributes(attribs);
						alter_def+=BaseObject::getAlterDefinition(this->getSchemaName(), attributes, true, true);
					}
					//Changing type attributes
					else
					{
						attribs[Attributes::Change]=Attributes::True;

						if(!type_attribs[attrib_idx].getType().isEquivalentTo(attrib.getType()))
						{
							attribs[Attributes::Attribute]=attrib.getName(true);
							attribs[Attributes::Type]=attrib.getType().getCodeDefinition(SchemaParser::SqlDefinition);
						}

						copyAttributes(attribs);
						alter_def+=BaseObject::getAlterDefinition(this->getSchemaName(), attributes, true, true);
						attributes[Attributes::Change]=QString();
					}

					attribs.clear();
				}
			}
		}

		return alter_def;
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
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

	PgSqlType::renameUserType(prev_name, this, this->getName(true));
}

