#include "conversaocodificacao.h"
//***********************************************************
ConversaoCodificacao::ConversaoCodificacao(void)
{
 tipo_objeto=OBJETO_CONV_CODIFICACAO;
 funcao_conv=NULL;
 padrao=false;
 atributos[AtributosParsers::PADRAO]="";
 atributos[AtributosParsers::COD_ORIGEM]="";
 atributos[AtributosParsers::COD_DESTINO]="";
 atributos[AtributosParsers::FUNCAO]="";
}
//-----------------------------------------------------------
void ConversaoCodificacao::definirCodificacao(unsigned idx_tipo, TipoCodificacao tipo_cod)
{
 //Verifica se o índice da codificação a ser acessada é válido
 if(idx_tipo<=CONV_COD_DESTINO)
 {
  //Se o tipo de codificação passado é nulo, dispara uma exceção
  if((~tipo_cod)=="")
   throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELER_ATRTIPONULO)
                         .arg(this->obterNome())
                         .arg(ObjetoBase::obterNomeTipoObjeto(OBJETO_CONV_CODIFICACAO)),
                 ERR_PGMODELER_ATRTIPONULO,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  //Atribui o tipo de codificação ao vetor de tipos codificações
  this->codificacoes[idx_tipo]=tipo_cod;
 }
 else
  //Dispara uma exceção caso o índice do tipo é inválido
  throw Excecao(ERR_PGMODELER_REFTIPOIDXINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
}
//-----------------------------------------------------------
void ConversaoCodificacao::definirFuncaoConversao(Funcao *funcao_conv)
{
 //Caso a função de conversão passada seja nula, dispara uma exceção
 if(!funcao_conv)
  throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELER_ATRFUNCNAOALOC)
                         .arg(this->obterNome(true))
                         .arg(ObjetoBase::obterNomeTipoObjeto(OBJETO_CONV_CODIFICACAO)),
                ERR_PGMODELER_ATRFUNCNAOALOC,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 //A função de conversão deve obrigatoriamente possuir 5 parâmetros
 else if(funcao_conv->obterNumParams()!=5)
  throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELER_ATRFUNCNUMPARAMINV)
                         .arg(this->obterNome(true))
                         .arg(ObjetoBase::obterNomeTipoObjeto(OBJETO_CONV_CODIFICACAO)),
                ERR_PGMODELER_ATRFUNCNUMPARAMINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 /* Verifica os tipos dos parâmetros da função de conversão.
    Os mesmos devem ser, em ordem, do tipo integer, integer, cstring,
    internal, integer */
 else if(funcao_conv->obterParametro(0).obterTipo()!="integer" ||
         funcao_conv->obterParametro(1).obterTipo()!="integer" ||
         funcao_conv->obterParametro(2).obterTipo()!="cstring" ||
         funcao_conv->obterParametro(3).obterTipo()!="internal" ||
         funcao_conv->obterParametro(4).obterTipo()!="integer")
  throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELER_ATRFUNCPARAMINV)
                         .arg(this->obterNome(true))
                         .arg(ObjetoBase::obterNomeTipoObjeto(OBJETO_CONV_CODIFICACAO)),
                ERR_PGMODELER_ATRFUNCPARAMINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 //O tipo de retorno da função de conversão deve ser 'void'
 else if(funcao_conv->obterTipoRetorno()!="void")
  throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELER_ATRFUNCRETINV)
                         .arg(this->obterNome(true))
                         .arg(ObjetoBase::obterNomeTipoObjeto(OBJETO_CONV_CODIFICACAO)),
                ERR_PGMODELER_ATRFUNCRETINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 this->funcao_conv=funcao_conv;
}
//-----------------------------------------------------------
void ConversaoCodificacao::definirPadrao(bool valor)
{
 padrao=valor;
}
//-----------------------------------------------------------
TipoCodificacao ConversaoCodificacao::obterCodificacao(unsigned idx_tipo)
{
 if(idx_tipo<=CONV_COD_DESTINO)
  return(this->codificacoes[idx_tipo]);
 else
  //Caso o usuário não referencie a codificação com o índice correto
  throw Excecao(ERR_PGMODELER_REFTIPOIDXINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
}
//-----------------------------------------------------------
Funcao *ConversaoCodificacao::obterFuncaoConversao(void)
{
 return(funcao_conv);
}
//-----------------------------------------------------------
bool ConversaoCodificacao::conversaoPadrao(void)
{
 return(padrao);
}
//-----------------------------------------------------------
QString ConversaoCodificacao::obterDefinicaoObjeto(unsigned tipo_def)
{
 atributos[AtributosParsers::PADRAO]=(padrao ? "1" : "");
 atributos[AtributosParsers::COD_ORIGEM]=(~codificacoes[CONV_COD_ORIGEM]);
 atributos[AtributosParsers::COD_DESTINO]=(~codificacoes[CONV_COD_DESTINO]);

 if(funcao_conv)
 {
  if(tipo_def==ParserEsquema::DEFINICAO_SQL)
   atributos[AtributosParsers::FUNCAO]=funcao_conv->obterNome(true);
  else
   atributos[AtributosParsers::FUNCAO]=funcao_conv->obterDefinicaoObjeto(tipo_def, true);
 }

 return(ObjetoBase::obterDefinicaoObjeto(tipo_def));
}
//***********************************************************
