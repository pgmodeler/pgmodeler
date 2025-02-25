#include "extension.h"
#include "pgsqltypes/pgsqltype.h"

Extension::Extension()
{
	obj_type=ObjectType::Extension;
	handles_type=false;
	attributes[Attributes::CurVersion]="";
	attributes[Attributes::OldVersion]="";
	attributes[Attributes::Types]="";
}

void Extension::setSchema(BaseObject *schema)
{
	if(!schema)
		this->schema = schema;
	else
	{
		QString new_type_name;

		#warning "Test schema change and update types signatures!"
		for(auto &tp_name : type_names)
		{
			new_type_name = QString("%1.%2").arg(schema->getName(true, false), tp_name);

			if(PgSqlType::getUserTypeIndex(new_type_name, nullptr, getDatabase()) != PgSqlType::Null)
			{
				PgSqlType user_type(new_type_name);

				if(!user_type.getObject()->isDependingOn(this))
				{
					throw Exception(Exception::getErrorMessage(ErrorCode::AsgSchExtTypeConflict)
															 .arg(schema->getName(), obj_name, tp_name, BaseObject::getTypeName(ObjectType::Type)),
													 ErrorCode::AsgSchExtTypeConflict, __PRETTY_FUNCTION__, __FILE__, __LINE__);
				}
			}
		}

		BaseObject::setSchema(schema);
	}
}

void Extension::addObject(const ExtObject &ext_obj)
{
	if(!ext_obj.isValid())
	{
		throw Exception(Exception::getErrorMessage(ErrorCode::InvExtensionObject).arg(obj_name),
										ErrorCode::InvExtensionObject, __PRETTY_FUNCTION__, __FILE__, __LINE__, nullptr,
										QString(QT_TR_NOOP("Invalid object: %1")).arg(ext_obj.toString()));
	}

	if(containsObject(ext_obj))
		return;

	ext_objects[ext_obj.obj_type].push_back(ext_obj);
	setCodeInvalidated(true);
}

void Extension::removeObjects(ObjectType obj_type)
{
	if(obj_type == ObjectType::BaseObject)
		ext_objects.clear();
	else
		ext_objects.erase(obj_type);
}

std::vector<Extension::ExtObject> Extension::getObjects(ObjectType obj_type)
{
	if(obj_type != ObjectType::Type && obj_type != ObjectType::Schema)
	{
		throw Exception(ErrorCode::ObtObjectInvalidType, __PRETTY_FUNCTION__, __FILE__, __LINE__, nullptr,
										QString(QT_TR_NOOP("Invalid object type: %1")).arg(BaseObject::getTypeName(obj_type)));
	}

	return ext_objects[obj_type];
}

void Extension::setVersion(VersionId ver, const QString &value)
{
	if(ver > OldVersion)
		throw Exception(ErrorCode::RefAttributeInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	setCodeInvalidated(versions[ver] != value);
	versions[ver]=value;
}

QString Extension::getVersion(VersionId ver)
{
	if(ver > OldVersion)
		throw Exception(ErrorCode::RefAttributeInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return versions[ver];
}

bool Extension::containsObject(const ExtObject &ext_obj)
{
	if(!ext_obj.isValid() || !ext_objects.count(ext_obj.obj_type))
		return false;

	QString obj_signature;

	for(auto &obj : ext_objects[ext_obj.obj_type])
	{
		/* If the extension object is a schema child one and the attribute
		 * parent is empty, we adopt the name of the same schema of the
		 * parent extension in the comparison below */
		if(BaseObject::isChildObjectType(ObjectType::Schema, obj.obj_type) &&
			 obj.parent.isEmpty())
		{
			obj_signature = BaseObject::formatName(schema->getName()) + "." + BaseObject::formatName(obj.name);
		}
		else
			obj_signature = obj.signature;

		/* If the extension objects themselves aren't equal,
		 * as a fallback, we compare the signature and their types */
		if(obj == ext_obj ||
			 (!obj_signature.isEmpty() &&
				obj_signature == ext_obj.signature))
			return true;
	}

	return false;
}

QString Extension::getSourceCode(SchemaParser::CodeType def_type)
{
	QString code_def=getCachedCode(def_type, false);
	if(!code_def.isEmpty()) return code_def;

	attributes[Attributes::Name] = getName(def_type==SchemaParser::SqlCode, false);
	attributes[Attributes::CurVersion] = versions[CurVersion];
	attributes[Attributes::OldVersion] = versions[OldVersion];
	attributes[Attributes::Types] = "";

	if(def_type == SchemaParser::XmlCode && !type_names.isEmpty())
	{
		attribs_map type_attr;

		#warning "Change from <type> to <object>!"
		for(auto &tp_name : type_names)
		{
			type_attr[Attributes::Name] = tp_name;

			schparser.ignoreUnkownAttributes(true);
			schparser.ignoreEmptyAttributes(true);
			attributes[Attributes::Types] += schparser.getSourceCode(Attributes::PgSqlBaseType, type_attr, def_type);
		}
	}

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
