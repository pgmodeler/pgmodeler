#include "caixatexto.h"

CaixaTexto::CaixaTexto(void)
{
 setComment(" ");
 obj_type=OBJ_TEXTBOX;
 atributo_texto[0]=atributo_texto[1]=atributo_texto[2]=false;
 attributes[ParsersAttributes::ITALIC]="";
 attributes[ParsersAttributes::BOLD]="";
 attributes[ParsersAttributes::UNDERLINE]="";
 attributes[ParsersAttributes::COLOR]="";
}

QString CaixaTexto::getCodeDefinition(void)
{
 setPositionAttribute();

 if(atributo_texto[TEXTO_ITALICO])
  attributes[ParsersAttributes::ITALIC]="1";

 if(atributo_texto[TEXTO_NEGRITO])
  attributes[ParsersAttributes::BOLD]="1";

 if(atributo_texto[TEXTO_SUBLINHADO])
  attributes[ParsersAttributes::UNDERLINE]="1";

 if(cor_texto.name()!="#000000")
  attributes[ParsersAttributes::COLOR]=cor_texto.name();

 return(this->BaseObject::getCodeDefinition(SchemaParser::XML_DEFINITION));
}

void CaixaTexto::operator = (CaixaTexto &caixa)
{
 (*dynamic_cast<BaseGraphicObject *>(this))=reinterpret_cast<BaseGraphicObject &>(caixa);
 this->comment=caixa.comment;
 this->atributo_texto[0]=caixa.atributo_texto[0];
 this->atributo_texto[1]=caixa.atributo_texto[1];
 this->atributo_texto[2]=caixa.atributo_texto[2];
 this->cor_texto=caixa.cor_texto;
}

void CaixaTexto::definirAtributoTexto(unsigned atributo, bool valor)
{
 if(atributo > TEXTO_SUBLINHADO)
  throw Exception(ERR_REF_ATTRIB_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 atributo_texto[atributo]=valor;
}

void CaixaTexto::definirCorTexto(const QColor &cor)
{
 cor_texto=cor;
}

QColor CaixaTexto::obterCorTexto(void)
{
 return(cor_texto);
}

bool CaixaTexto::obterAtributoTexto(unsigned atributo)
{
 if(atributo > TEXTO_SUBLINHADO)
  throw Exception(ERR_REF_ATTRIB_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 return(atributo_texto[atributo]);
}

