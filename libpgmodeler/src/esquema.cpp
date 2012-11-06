#include "esquema.h"

unsigned Esquema::schema_id=30000;

Esquema::Esquema(void)
{
 obj_type=OBJ_SCHEMA;
 object_id=Esquema::schema_id++;
}

void Esquema::setName(const QString &nome)
{
 /* Tratando nomes de esquemas iniciados em pg_ pois são reservados para o SGBD e
    não podem ser criados pelo usuário */
 if(nome.mid(0,3)=="pg_")
  throw Exception(Exception::getErrorMessage(ERR_ASG_RESERVED_NAME)
                         .arg(QString::fromUtf8(this->getName()))
                         .arg(BaseObject::getTypeName(OBJ_SCHEMA)),
                ERR_ASG_RESERVED_NAME,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 BaseObject::setName(nome); //Chama o método da classe descendente
}

QString Esquema::getCodeDefinition(unsigned def_type)
{
 return(BaseObject::__getCodeDefinition(def_type));
}

