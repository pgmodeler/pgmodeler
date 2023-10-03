#include "extension.h"
#include "pgsqltypes/pgsqltype.h"

Extension::Extension()
{
	obj_type=ObjectType::Extension;
	handles_type=false;
	attributes[Attributes::HandlesType]="";
	attributes[Attributes::CurVersion]="";
	attributes[Attributes::OldVersion]="";
}

Extension::~Extension()
{
	/* We destroy the children types only if the extension isn't installed in the database model
	 * otherwise the database model takes care of destroying the types */
	if(!getDatabase())
	{
		for(auto & type : types)
			delete type;
	}
}

void Extension::setName(const QString &name)
{
	if(!handles_type)
		BaseObject::setName(name);
	else
	{
		QString prev_name, new_name;

		prev_name = getName(true, true);
		BaseObject::setName(name);
		new_name = getName(true, true);

		#warning "Can be removed!"
		//Renames the PostgreSQL type represented by the extension
		//PgSqlType::renameUserType(prev_name, this, new_name);
	}
}

void Extension::setSchema(BaseObject *schema)
{
	if(!schema)
		this->schema = schema;
	else
	{
		QString prev_name = getName(true, true);
		BaseObject::setSchema(schema);

		#warning "Rename types!"
		//Renames the PostgreSQL type represented by the extension
		if(handles_type)
			PgSqlType::renameUserType(prev_name, this, getName(true, true));
	}
}

void Extension::setTypes(const QStringList &type_names)
{
	std::vector<Type *> new_types;

	if(type_names.isEmpty())
		return;

	if(!type_names.isEmpty() && isTypesReferenced())
	{
		/* Raises an error if the extension is already registered with children data types and
		 * the user tries to change them. This cannot be done to avoid cascade reference breaking
		 * on table columns/functions or any other objects that references PgSQLType */
		throw Exception(Exception::getErrorMessage(ErrorCode::ExtensionHandlingTypeImmutable).arg(getSignature()),
										 ErrorCode::ExtensionHandlingTypeImmutable,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	}

	try
	{
		Type *type = nullptr;

		for(auto &tp_name : type_names)
		{
			type = new Type;
			type->setName(tp_name);
			type->setSchema(schema);
			type->setSystemObject(true);
			type->setConfiguration(Type::EnumerationType);
			type->getSourceCode(SchemaParser::SqlCode);
			new_types.push_back(type);
		}

		types = new_types;
	}
	catch(Exception &e)
	{
		// Destroy all the new types created
		for(auto &type : new_types)
			delete type;

		throw Exception(e.getErrorMessage(), e.getErrorCode(), __PRETTY_FUNCTION__, __FILE__, __LINE__, &e);
	}
}

std::vector<Type *> Extension::getTypes()
{
	return types;
}

bool Extension::isTypesReferenced()
{
	for(auto &type : types)
	{
		if(type->isReferenced())
			return true;
	}

	return false;
}

void Extension::setHandlesType(bool value)
{
	/* Raises an error if the extension is already registered as a data type and the
	try to change the attribute value. This cannot be done to avoid cascade reference breaking
	on table columns/functions or any other objects that references PgSQLType */
	if(!value && PgSqlType::getUserTypeIndex(getName(true, true), this) != PgSqlType::Null)
		throw Exception(Exception::getErrorMessage(ErrorCode::ExtensionHandlingTypeImmutable)
						.arg(getName(true)),
						ErrorCode::ExtensionHandlingTypeImmutable,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	this->handles_type=value;
}

void Extension::setVersion(VersionId ver, const QString &value)
{
	if(ver > OldVersion)
		throw Exception(ErrorCode::RefAttributeInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	setCodeInvalidated(versions[ver] != value);
	versions[ver]=value;
}

bool Extension::handlesType()
{
	return handles_type;
}

QString Extension::getVersion(VersionId ver)
{
	if(ver > OldVersion)
		throw Exception(ErrorCode::RefAttributeInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return versions[ver];
}

QString Extension::getSourceCode(SchemaParser::CodeType def_type)
{
	QString code_def=getCachedCode(def_type, false);
	if(!code_def.isEmpty()) return code_def;

	attributes[Attributes::Name]=getName(def_type==SchemaParser::SqlCode, false);
	attributes[Attributes::HandlesType]=(handles_type ? Attributes::True : "");
	attributes[Attributes::CurVersion]=versions[CurVersion];
	attributes[Attributes::OldVersion]=versions[OldVersion];

	return BaseObject::__getSourceCode(def_type);
}

QString Extension::getAlterCode(BaseObject *object)
{
	Extension *ext=dynamic_cast<Extension *>(object);

	if(!ext)
		throw Exception(ErrorCode::OprNotAllocatedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	try
	{
		attributes[Attributes::AlterCmds]=BaseObject::getAlterCode(object);
		attributes[Attributes::NewVersion]="";

		if(!this->versions[CurVersion].isEmpty() && !ext->versions[CurVersion].isEmpty() &&
				this->versions[CurVersion].isEmpty() < ext->versions[CurVersion].isEmpty())
			attributes[Attributes::NewVersion]=ext->versions[CurVersion];

		return BaseObject::getAlterCode(this->getSchemaName(), attributes, false, true);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

QString Extension::getDropCode(bool cascade)
{
	attributes[Attributes::Name] = getName(true);
	return BaseObject::getDropCode(cascade);
}

QString Extension::getSignature(bool format)
{
	return getName(format, false);
}

QString Extension::getName(bool format, bool prepend_schema)
{
	return BaseObject::getName(format, prepend_schema);
}

void Extension::operator = (Extension &ext)
{
	QString prev_name=getName(true, true);

	*(dynamic_cast<BaseObject *>(this))=dynamic_cast<BaseObject &>(ext);
	this->versions[CurVersion]=ext.versions[CurVersion];
	this->versions[OldVersion]=ext.versions[OldVersion];
	this->handles_type=ext.handles_type;

	if(this->handles_type)
		PgSqlType::renameUserType(prev_name, this, getName(true, true));
}
