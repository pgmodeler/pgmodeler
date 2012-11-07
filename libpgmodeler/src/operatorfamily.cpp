#include "operatorfamily.h"

OperatorFamily::OperatorFamily(void)
{
 obj_type=OBJ_OPFAMILY;
 attributes[ParsersAttributes::INDEX_TYPE]="";
}

void OperatorFamily::setIndexingType(TipoIndexacao idx_type)
{
 indexing_type=idx_type;
}

TipoIndexacao OperatorFamily::getIndexingType(void)
{
 return(indexing_type);
}

QString OperatorFamily::getCodeDefinition(unsigned def_type)
{
 return(this->getCodeDefinition(def_type, false));
}

QString OperatorFamily::getCodeDefinition(unsigned def_type, bool reduced_form)
{
 attributes[ParsersAttributes::INDEX_TYPE]=(~indexing_type);
 return(BaseObject::getCodeDefinition(def_type,reduced_form));
}

