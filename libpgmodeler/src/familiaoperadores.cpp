#include "familiaoperadores.h"

FamiliaOperadores::FamiliaOperadores(void)
{
 obj_type=OBJ_OPFAMILY;
 attributes[ParsersAttributes::INDEX_TYPE]="";
}

void FamiliaOperadores::definirTipoIndexacao(TipoIndexacao tipo)
{
 tipo_index=tipo;
}

TipoIndexacao FamiliaOperadores::obterTipoIndexacao(void)
{
 return(tipo_index);
}

QString FamiliaOperadores::getCodeDefinition(unsigned tipo_def)
{
 return(this->getCodeDefinition(tipo_def, false));
}

QString FamiliaOperadores::getCodeDefinition(unsigned tipo_def, bool forma_reduzida)
{
 attributes[ParsersAttributes::INDEX_TYPE]=(~tipo_index);
 return(BaseObject::getCodeDefinition(tipo_def,forma_reduzida));
}

