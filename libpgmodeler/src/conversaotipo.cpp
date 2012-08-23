#include "conversaotipo.h"
//***********************************************************
ConversaoTipo::ConversaoTipo(void)
{
 tipo_objeto=OBJETO_CONV_TIPO;
 funcao_conv=NULL;
 tipo=CONV_IMPLICITA;
 entrada_saida=false;
 atributos[AtributosParsers::TIPO_ORIGEM]="";
 atributos[AtributosParsers::TIPO_DESTINO]="";
 atributos[AtributosParsers::ASSINATURA]="";
 atributos[AtributosParsers::TIPO_CONVERSAO]="";
 atributos[AtributosParsers::CONV_ENTRADA_SAIDA]="";
}
//-----------------------------------------------------------
void ConversaoTipo::definirTipoDado(unsigned idx_tipo, TipoPgSQL tipo_dado)
{
 //Verifica se o índice do tipo de dado é válido
 if(idx_tipo<=CONV_TIPO_DESTINO)
 {
  /* Caso o tipo de dado que está sendo atribuido seja nulo (vazio)
     é disparada uma exceção, pois um tipo de dado nulo não deve participar
     de uma conversão */
  if((*tipo_dado)=="")
   throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELER_ATRTIPONULO)
                         .arg(this->obterNome())
                         .arg(ObjetoBase::obterNomeTipoObjeto(OBJETO_CONV_TIPO)),
                 ERR_PGMODELER_ATRTIPONULO,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  /* Atribui o tipo de dado ao índice especifico de tipos de dados
     envolvidos na conversão */
  this->tipos[idx_tipo]=tipo_dado;
 }
 else
  /* Caso o índice usado para referenciar o tipo de dado seja inválido
     é disparada um exceção */
  throw Excecao(ERR_PGMODELER_REFTIPOIDXINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 this->nome=QString("cast(%1,%2)").arg(~tipos[CONV_TIPO_ORIGEM]).arg(~tipos[CONV_TIPO_DESTINO]);
}
//-----------------------------------------------------------
void ConversaoTipo::definirTipoConversao(unsigned tipo)
{
 /* Caso se tente atribuir um tipo inválido de conversão
    uma exceção será disparada */
 if(tipo!=CONV_ATRIBUICAO && tipo!=CONV_IMPLICITA)
  throw Excecao(ERR_PGMODELER_ATRTIPOINVOBJ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 this->tipo=tipo;
}
//-----------------------------------------------------------
void ConversaoTipo::definirEntradaSaida(bool valor)
{
 entrada_saida=valor;
}
//-----------------------------------------------------------
void ConversaoTipo::definirFuncaoConversao(Funcao *funcao_conv)
{
 unsigned qtd_param;
 bool erro=false;

 if(!funcao_conv)
  throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELER_ATRFUNCNAOALOC)
                         .arg(this->obterNome())
                         .arg(ObjetoBase::obterNomeTipoObjeto(OBJETO_CONV_TIPO)),
                ERR_PGMODELER_ATRFUNCNAOALOC,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Obtém o número de parâmetros da função para validações específicas
 qtd_param=funcao_conv->obterNumParams();

 //A função deve possuir número de parâmetros de 1 a 3
 if(qtd_param==0 || qtd_param > 3)
  throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELER_ATRFUNCNUMPARAMINV)
                         .arg(this->obterNome())
                         .arg(ObjetoBase::obterNomeTipoObjeto(OBJETO_CONV_TIPO)),
                ERR_PGMODELER_ATRFUNCNUMPARAMINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
 {
  /* Verifica se o tipo do primeiro parâmetro é diferente do tipo do
     tipo de dado de origem da conversão */
  erro=(funcao_conv->obterParametro(0).obterTipo()!=this->tipos[CONV_TIPO_ORIGEM]);

  //Verifica se o tipo do segundo parâmetro é diferente do tipo integer
  if(!erro && qtd_param>=2)
   erro=(funcao_conv->obterParametro(1).obterTipo()!="integer");

  //Verifica se o tipo do terceiro parâmetro é diferente do tipo boolean
  if(!erro && qtd_param==3)
   erro=(funcao_conv->obterParametro(2).obterTipo()!="boolean");

  //Caso alguma das validações acima falhe dispara-se uma exceção
  if(erro)
   throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELER_ATRFUNCPARAMINV)
                         .arg(this->obterNome())
                         .arg(ObjetoBase::obterNomeTipoObjeto(OBJETO_CONV_TIPO)),
                 ERR_PGMODELER_ATRFUNCPARAMINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 }

 //Verifica se o tipo de retorno da função é diferente do tipo de destino da conversão
 if(funcao_conv->obterTipoRetorno()!=this->tipos[CONV_TIPO_DESTINO])
  throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELER_ATRFUNCRETINV)
                         .arg(this->obterNome())
                         .arg(ObjetoBase::obterNomeTipoObjeto(OBJETO_CONV_TIPO)),
                ERR_PGMODELER_ATRFUNCRETINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 this->funcao_conv=funcao_conv;
}
//-----------------------------------------------------------
TipoPgSQL ConversaoTipo::obterTipoDado(unsigned idx_tipo)
{
 /* Retorna um tipo de dado envolvido na conversão caso
    o índice seja válido */
 if(idx_tipo<=CONV_TIPO_DESTINO)
  return(this->tipos[idx_tipo]);
 else
  //Caso o índice usado seja inválido, dispare-se uma exceção
  throw Excecao(ERR_PGMODELER_REFTIPOIDXINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
}
//-----------------------------------------------------------
bool ConversaoTipo::obterEntradaSaida(void)
{
 return(entrada_saida);
}
//-----------------------------------------------------------
Funcao *ConversaoTipo::obterFuncaoConversao(void)
{
 return(funcao_conv);
}
//-----------------------------------------------------------
unsigned ConversaoTipo::obterTipoConversao(void)
{
 return(tipo);
}
//-----------------------------------------------------------
QString ConversaoTipo::obterDefinicaoObjeto(unsigned tipo_def)
{
 if(tipo_def==ParserEsquema::DEFINICAO_SQL)
 {
  atributos[AtributosParsers::TIPO_ORIGEM]=(*tipos[CONV_TIPO_ORIGEM]);
  atributos[AtributosParsers::TIPO_DESTINO]=(*tipos[CONV_TIPO_DESTINO]);
 }
 else
 {
  atributos[AtributosParsers::TIPO_ORIGEM]=tipos[CONV_TIPO_ORIGEM].obterDefinicaoObjeto(tipo_def);
  atributos[AtributosParsers::TIPO_DESTINO]=tipos[CONV_TIPO_DESTINO].obterDefinicaoObjeto(tipo_def);
 }

 if(funcao_conv)
 {
  if(tipo_def==ParserEsquema::DEFINICAO_SQL)
   atributos[AtributosParsers::ASSINATURA]=funcao_conv->obterAssinatura();
  else
   atributos[AtributosParsers::ASSINATURA]=funcao_conv->obterDefinicaoObjeto(tipo_def, true);
 }

 if(tipo==CONV_ATRIBUICAO)
  atributos[AtributosParsers::TIPO_CONVERSAO]=AtributosParsers::ATRIBUICAO;
 else
  atributos[AtributosParsers::TIPO_CONVERSAO]=AtributosParsers::IMPLICITA;

 if(tipo_def==ParserEsquema::DEFINICAO_SQL)
  atributos[AtributosParsers::TIPO_CONVERSAO]=atributos[AtributosParsers::TIPO_CONVERSAO].toUpper();

 atributos[AtributosParsers::CONV_ENTRADA_SAIDA]=(entrada_saida ? "1" : "");

 return(ObjetoBase::obterDefinicaoObjeto(tipo_def));
}
//***********************************************************
