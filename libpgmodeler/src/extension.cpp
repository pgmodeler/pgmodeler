#include "extension.h"

Extension::Extension(void)
{
	obj_type=OBJ_EXTENSION;
	handles_type=false;
	attributes[ParsersAttributes::HANDLES_TYPE]="";
	attributes[ParsersAttributes::CUR_VERSION]="";
	attributes[ParsersAttributes::OLD_VERSION]="";
}

Extension::~Extension(void)
{
	if(handles_type)
		PgSQLType::removeUserType(this->getName(true), this);
}

void Extension::setName(const QString &name)
{
	if(!handles_type)
		BaseObject::setName(name);
	else
	{
		QString prev_name, new_name;

		prev_name=this->getName(true);
		BaseObject::setName(name);
		new_name=this->getName(true);

		//Renames the PostgreSQL type represented by the extension
		PgSQLType::renameUserType(prev_name, this, new_name);
	}
}

void Extension::setSchema(BaseObject *schema)
{
	if(!handles_type)
		BaseObject::setSchema(schema);
	else
	{
		QString prev_name;

		prev_name=this->getName(true);
		BaseObject::setSchema(schema);

		//Renames the PostgreSQL type represented by the extension
		PgSQLType::renameUserType(prev_name, this, this->getName(true));
	}
}

void Extension::setHandlesType(bool value)
{
	/* Raises an error if the extension is already registered as a data type and the
	try to change the attribute value. This cannot be done to avoid cascade reference breaking
	on table columns/functions or any other objects that references PgSQLType */
	if(!value && PgSQLType::getUserTypeIndex(this->getName(true), this) != BaseType::null)
		throw Exception(Exception::getErrorMessage(ERR_REG_EXT_NOT_HANDLING_TYPE)
										.arg(Utf8String::create(this->getName(true)))
										,ERR_REG_EXT_NOT_HANDLING_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	this->handles_type=value;
}

void Extension::setVersion(unsigned ver, const QString &value)
{
	if(ver > OLD_VERSION)
		throw Exception(ERR_REF_ATTRIB_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	versions[ver]=value;
}

bool Extension::handlesType(void)
{
	return(handles_type);
}

QString Extension::getVersion(unsigned ver)
{
	if(ver > OLD_VERSION)
		throw Exception(ERR_REF_ATTRIB_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return(versions[ver]);
}

QString Extension::getCodeDefinition(unsigned def_type)
{
	attributes[ParsersAttributes::NAME]=this->getName(true, false);
	attributes[ParsersAttributes::HANDLES_TYPE]=(handles_type ? "1" : "");
	attributes[ParsersAttributes::CUR_VERSION]=versions[CUR_VERSION];
	attributes[ParsersAttributes::OLD_VERSION]=versions[OLD_VERSION];

	attributes[ParsersAttributes::NAME]=this->getName();

	return(BaseObject::__getCodeDefinition(def_type));
}

void Extension::operator = (Extension &ext)
{
	QString prev_name=this->getName(true);

	*(dynamic_cast<BaseObject *>(this))=dynamic_cast<BaseObject &>(ext);
	this->versions[CUR_VERSION]=ext.versions[CUR_VERSION];
	this->versions[OLD_VERSION]=ext.versions[OLD_VERSION];
	this->handles_type=ext.handles_type;

	if(this->handles_type)
		PgSQLType::renameUserType(prev_name, this, this->getName(true));
}
