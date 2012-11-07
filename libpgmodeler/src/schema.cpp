#include "schema.h"

unsigned Schema::schema_id=30000;

Schema::Schema(void)
{
 obj_type=OBJ_SCHEMA;
 object_id=Schema::schema_id++;
}

void Schema::setName(const QString &name)
{
 /* Schema names starting with pg_ is reserved to PostgreSQL if its the case
    raises an error */
 if(name.mid(0,3)=="pg_")
  throw Exception(Exception::getErrorMessage(ERR_ASG_RESERVED_NAME)
                         .arg(QString::fromUtf8(this->getName()))
                         .arg(BaseObject::getTypeName(OBJ_SCHEMA)),
                ERR_ASG_RESERVED_NAME,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 BaseObject::setName(name);
}

QString Schema::getCodeDefinition(unsigned def_type)
{
 return(BaseObject::__getCodeDefinition(def_type));
}

