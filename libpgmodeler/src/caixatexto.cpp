#include "caixatexto.h"
//***********************************************************
CaixaTexto::CaixaTexto(void)
{
 definirComentario(" ");
 tipo_objeto=OBJETO_CAIXA_TEXTO;
 atributo_texto[0]=atributo_texto[1]=atributo_texto[2]=false;
 atributos[AtributosParsers::ITALICO]="";
 atributos[AtributosParsers::NEGRITO]="";
 atributos[AtributosParsers::SUBLINHADO]="";
 atributos[AtributosParsers::COR]="";
}
//-----------------------------------------------------------
QString CaixaTexto::obterDefinicaoObjeto(void)
{
 definirAtributoPosicao();

 if(atributo_texto[TEXTO_ITALICO])
  atributos[AtributosParsers::ITALICO]="1";

 if(atributo_texto[TEXTO_NEGRITO])
  atributos[AtributosParsers::NEGRITO]="1";

 if(atributo_texto[TEXTO_SUBLINHADO])
  atributos[AtributosParsers::SUBLINHADO]="1";

 if(cor_texto.name()!="#000000")
  atributos[AtributosParsers::COR]=cor_texto.name();

 return(this->ObjetoBase::obterDefinicaoObjeto(ParserEsquema::DEFINICAO_XML));
}
//-----------------------------------------------------------
void CaixaTexto::operator = (CaixaTexto &caixa)
{
 (*dynamic_cast<ObjetoGraficoBase *>(this))=reinterpret_cast<ObjetoGraficoBase &>(caixa);
 this->comentario=caixa.comentario;
 this->atributo_texto[0]=caixa.atributo_texto[0];
 this->atributo_texto[1]=caixa.atributo_texto[1];
 this->atributo_texto[2]=caixa.atributo_texto[2];
 this->cor_texto=caixa.cor_texto;
}
//-----------------------------------------------------------
void CaixaTexto::definirAtributoTexto(unsigned atributo, bool valor)
{
 if(atributo > TEXTO_SUBLINHADO)
  throw Excecao(ERR_PGMODELER_REFATRIBTIPOIDXINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 atributo_texto[atributo]=valor;
}
//-----------------------------------------------------------
void CaixaTexto::definirCorTexto(const QColor &cor)
{
 cor_texto=cor;
}
//-----------------------------------------------------------
QColor CaixaTexto::obterCorTexto(void)
{
 return(cor_texto);
}
//-----------------------------------------------------------
bool CaixaTexto::obterAtributoTexto(unsigned atributo)
{
 if(atributo > TEXTO_SUBLINHADO)
  throw Excecao(ERR_PGMODELER_REFATRIBTIPOIDXINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 return(atributo_texto[atributo]);
}
//***********************************************************
