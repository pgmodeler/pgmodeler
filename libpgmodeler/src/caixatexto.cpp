#include "caixatexto.h"

CaixaTexto::CaixaTexto(void)
{
 definirComentario(" ");
 tipo_objeto=OBJETO_CAIXA_TEXTO;
 atributo_texto[0]=atributo_texto[1]=atributo_texto[2]=false;
 atributos[ParsersAttributes::ITALIC]="";
 atributos[ParsersAttributes::BOLD]="";
 atributos[ParsersAttributes::UNDERLINE]="";
 atributos[ParsersAttributes::COLOR]="";
}

QString CaixaTexto::obterDefinicaoObjeto(void)
{
 definirAtributoPosicao();

 if(atributo_texto[TEXTO_ITALICO])
  atributos[ParsersAttributes::ITALIC]="1";

 if(atributo_texto[TEXTO_NEGRITO])
  atributos[ParsersAttributes::BOLD]="1";

 if(atributo_texto[TEXTO_SUBLINHADO])
  atributos[ParsersAttributes::UNDERLINE]="1";

 if(cor_texto.name()!="#000000")
  atributos[ParsersAttributes::COLOR]=cor_texto.name();

 return(this->ObjetoBase::obterDefinicaoObjeto(ParserEsquema::DEFINICAO_XML));
}

void CaixaTexto::operator = (CaixaTexto &caixa)
{
 (*dynamic_cast<ObjetoGraficoBase *>(this))=reinterpret_cast<ObjetoGraficoBase &>(caixa);
 this->comentario=caixa.comentario;
 this->atributo_texto[0]=caixa.atributo_texto[0];
 this->atributo_texto[1]=caixa.atributo_texto[1];
 this->atributo_texto[2]=caixa.atributo_texto[2];
 this->cor_texto=caixa.cor_texto;
}

void CaixaTexto::definirAtributoTexto(unsigned atributo, bool valor)
{
 if(atributo > TEXTO_SUBLINHADO)
  throw Exception(ERR_PGMODELER_REFATRIBTIPOIDXINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);

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
  throw Exception(ERR_PGMODELER_REFATRIBTIPOIDXINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 return(atributo_texto[atributo]);
}

