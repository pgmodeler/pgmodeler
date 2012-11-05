#include "conversaocodificacao.h"

ConversaoCodificacao::ConversaoCodificacao(void)
{
 tipo_objeto=OBJ_CONVERSION;
 funcao_conv=NULL;
 padrao=false;
 atributos[ParsersAttributes::DEFAULT]="";
 atributos[ParsersAttributes::SRC_ENCODING]="";
 atributos[ParsersAttributes::DST_ENCODING]="";
 atributos[ParsersAttributes::FUNCTION]="";
}

void ConversaoCodificacao::definirCodificacao(unsigned idx_tipo, TipoCodificacao tipo_cod)
{
 //Verifica se o índice da codificação a ser acessada é válido
 if(idx_tipo<=CONV_COD_DESTINO)
 {
  //Se o tipo de codificação passado é nulo, dispara uma exceção
  if((~tipo_cod)=="")
   throw Exception(Exception::getErrorMessage(ERR_ASG_NULL_TYPE_OBJECT)
                         .arg(QString::fromUtf8(this->obterNome()))
                         .arg(BaseObject::obterNomeTipoObjeto(OBJ_CONVERSION)),
                 ERR_ASG_NULL_TYPE_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  //Atribui o tipo de codificação ao vetor de tipos codificações
  this->codificacoes[idx_tipo]=tipo_cod;
 }
 else
  //Dispara uma exceção caso o índice do tipo é inválido
  throw Exception(ERR_REF_TYPE_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);
}

void ConversaoCodificacao::definirFuncaoConversao(Funcao *funcao_conv)
{
 //Caso a função de conversão passada seja nula, dispara uma exceção
 if(!funcao_conv)
  throw Exception(Exception::getErrorMessage(ERR_ASG_NOT_ALOC_FUNCTION)
                         .arg(QString::fromUtf8(this->obterNome(true)))
                         .arg(BaseObject::obterNomeTipoObjeto(OBJ_CONVERSION)),
                ERR_ASG_NOT_ALOC_FUNCTION,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 //A função de conversão deve obrigatoriamente possuir 5 parâmetros
 else if(funcao_conv->obterNumParams()!=5)
  throw Exception(Exception::getErrorMessage(ERR_ASG_FUNC_INV_PARAM_COUNT)
                         .arg(QString::fromUtf8(this->obterNome(true)))
                         .arg(BaseObject::obterNomeTipoObjeto(OBJ_CONVERSION)),
                ERR_ASG_FUNC_INV_PARAM_COUNT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 /* Verifica os tipos dos parâmetros da função de conversão.
    Os mesmos devem ser, em ordem, do tipo integer, integer, cstring,
    internal, integer */
 else if(funcao_conv->obterParametro(0).obterTipo()!="integer" ||
         funcao_conv->obterParametro(1).obterTipo()!="integer" ||
         funcao_conv->obterParametro(2).obterTipo()!="cstring" ||
         funcao_conv->obterParametro(3).obterTipo()!="internal" ||
         funcao_conv->obterParametro(4).obterTipo()!="integer")
  throw Exception(Exception::getErrorMessage(ERR_ASG_FUNCTION_INV_PARAMS)
                         .arg(QString::fromUtf8(this->obterNome(true)))
                         .arg(BaseObject::obterNomeTipoObjeto(OBJ_CONVERSION)),
                ERR_ASG_FUNCTION_INV_PARAMS,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 //O tipo de retorno da função de conversão deve ser 'void'
 else if(funcao_conv->obterTipoRetorno()!="void")
  throw Exception(Exception::getErrorMessage(ERR_ASG_FUNCTION_INV_RET_TYPE)
                         .arg(QString::fromUtf8(this->obterNome(true)))
                         .arg(BaseObject::obterNomeTipoObjeto(OBJ_CONVERSION)),
                ERR_ASG_FUNCTION_INV_RET_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 this->funcao_conv=funcao_conv;
}

void ConversaoCodificacao::definirPadrao(bool valor)
{
 padrao=valor;
}

TipoCodificacao ConversaoCodificacao::obterCodificacao(unsigned idx_tipo)
{
 if(idx_tipo<=CONV_COD_DESTINO)
  return(this->codificacoes[idx_tipo]);
 else
  //Caso o usuário não referencie a codificação com o índice correto
  throw Exception(ERR_REF_TYPE_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);
}

Funcao *ConversaoCodificacao::obterFuncaoConversao(void)
{
 return(funcao_conv);
}

bool ConversaoCodificacao::conversaoPadrao(void)
{
 return(padrao);
}

QString ConversaoCodificacao::obterDefinicaoObjeto(unsigned tipo_def)
{
 atributos[ParsersAttributes::DEFAULT]=(padrao ? "1" : "");
 atributos[ParsersAttributes::SRC_ENCODING]=(~codificacoes[CONV_COD_ORIGEM]);
 atributos[ParsersAttributes::DST_ENCODING]=(~codificacoes[CONV_COD_DESTINO]);

 if(funcao_conv)
 {
  if(tipo_def==SchemaParser::SQL_DEFINITION)
   atributos[ParsersAttributes::FUNCTION]=funcao_conv->obterNome(true);
  else
   atributos[ParsersAttributes::FUNCTION]=funcao_conv->obterDefinicaoObjeto(tipo_def, true);
 }

 return(BaseObject::obterDefinicaoObjeto(tipo_def));
}

