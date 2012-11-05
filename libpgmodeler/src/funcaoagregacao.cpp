#include "funcaoagregacao.h"

FuncaoAgregacao::FuncaoAgregacao(void)
{
 obj_type=OBJ_AGGREGATE;
 funcoes[0]=funcoes[1]=NULL;
 op_ordenacao=NULL;
 attributes[ParsersAttributes::TYPES]="";
 attributes[ParsersAttributes::TRANSITION_FUNC]="";
 attributes[ParsersAttributes::STATE_TYPE]="";
 attributes[ParsersAttributes::BASE_TYPE]="";
 attributes[ParsersAttributes::FINAL_FUNC]="";
 attributes[ParsersAttributes::INITIAL_COND]="";
 attributes[ParsersAttributes::SORT_OP]="";
}

void FuncaoAgregacao::definirFuncao(unsigned idx_func, Funcao *func)
{
 /* Caso o usuário tente acessar um índice de função inválido,
    é gerado um erro */
 if(idx_func!=FUNCAO_FINAL && idx_func!=FUNCAO_TRANSICAO)
  throw Exception(ERR_REF_FUNCTION_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 /* Verfica se a função a qual está sendo atribuída, obedece algumas
    regras necessária para o tipo especfico */
 if(!funcaoValida(idx_func, func))
  throw Exception(Exception::getErrorMessage(ERR_USING_INV_FUNC_CONFIG)
                         .arg(QString::fromUtf8(this->getName()))
                         .arg(BaseObject::getTypeName(OBJ_AGGREGATE)),
                ERR_USING_INV_FUNC_CONFIG,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 funcoes[idx_func]=func;
}

bool FuncaoAgregacao::funcaoValida(unsigned idx_func, Funcao *func)
{
 if(func)
 {
  if(idx_func==FUNCAO_FINAL)
  {
   /* A função final deve possuir apenas 1 parâmetro de tipo
      igual ao atributo 'tipo_estado' */
   return((func->obterNumParams()==1 &&
           func->obterParametro(0).obterTipo()==tipo_estado));
  }
  else
  {
   unsigned qtd, i;
   bool cond1,cond2=true;

   /* A função de transição de estados deve possuir n+1 argumentos,
      onde n é a quantidade de tipos na lista de tipos de dados aceitos.
      Além disso o primeiro parâmetro da função e o tipo de retorno devem
      ser iguais ao tipo de dados de estado (tipo_estado) da função de agregação.
      E por fim, os demais parâmetros da função devem ser iguais aos tipos
      da lista de tipos da função de agregação. */
   cond1=(func->obterTipoRetorno()==tipo_estado) &&
         (func->obterNumParams()==tipo_dados.size() + 1) &&
         (func->obterParametro(0).obterTipo()==tipo_estado);

   qtd=func->obterNumParams();
   for(i=1 ; i < qtd && cond2; i++)
    cond2=(func->obterParametro(i).obterTipo()==tipo_dados[i-1]);

   return(cond1 && cond2);
  }
 }
 else return(true);
}

void FuncaoAgregacao::definirTipoEstado(TipoPgSQL tipo_estado)
{
 this->tipo_estado=tipo_estado;
}

void FuncaoAgregacao::definirCondicaoInicial(const QString &cond)
{
 cond_inicial=cond;
}

void FuncaoAgregacao::definirOperadorOrdenacao(Operador *op_ordenacao)
{
 if(op_ordenacao)
 {
  Funcao *func=NULL;

  /* De acordo com a documentação, um operador só pode ser atribuido a uma
     função de agregação quando:
	1) A função de agregação aceita apenas 1 tipo de dado
	2) A função que representa o operador possui parâmetros de tipos
           iguais ao tipo de entrada da função de agregação */
  func=op_ordenacao->obterFuncao(Operador::FUNC_OPERADOR);
  //Validando a condição 1
  if(tipo_dados.size()!=1)
   throw Exception(ERR_ASG_INV_OPER_ARGS,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  //Validando a condição 2
  else if(func->obterParametro(0).obterTipo()!=tipo_dados[0] ||
         (func->obterNumParams()==2 && func->obterParametro(1).obterTipo()!=tipo_dados[0]))
   throw Exception(ERR_ASG_INV_OPERATOR_TYPES,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 }

 this->op_ordenacao=op_ordenacao;
}

void FuncaoAgregacao::definirAtributoTipos(unsigned tipo_def)
{
 QString str_tipos;
 unsigned i, qtd;

 qtd=tipo_dados.size();
 for(i=0; i < qtd; i++)
 {
  if(tipo_def==SchemaParser::SQL_DEFINITION)
  {
   str_tipos+=*(tipo_dados[i]);
   if(i < (qtd-1)) str_tipos+=",";
  }
  else str_tipos+=tipo_dados[i].obterDefinicaoObjeto(tipo_def);
 }

 /* Caso o usuário não especifique nenhum tipo de entrada para a função
    cria uma função em parâmetros no seguinte formato: funcao(*). */
 if(str_tipos.isEmpty()) str_tipos="*";

 attributes[ParsersAttributes::TYPES]=str_tipos;
}

void FuncaoAgregacao::adicionarTipoDado(TipoPgSQL tipo)
{
 //Verifica se o tipo de dado já foi inserido na função
 if(tipoDadoExiste(tipo))
  //Caso existe um erro é gerado
  throw Exception(Exception::getErrorMessage(ERR_INS_DUPLIC_TYPE)
                .arg(QString::fromUtf8(~tipo))
                .arg(QString::fromUtf8(this->getName(true))),
                ERR_INS_DUPLIC_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Insere o tipo ao final da lista
 tipo_dados.push_back(tipo);
}

void FuncaoAgregacao::removerTipoDado(unsigned idx_tipo)
{
 //Verifica se o índice do tipo a ser removido é inválido
 if(idx_tipo >= tipo_dados.size())
  //Caso seja inválido, dispara uma exceção
  throw Exception(ERR_REF_TYPE_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Remove o elemento na posição especificada pelo índice
 tipo_dados.erase(tipo_dados.begin() + idx_tipo);
}

void FuncaoAgregacao::removerTiposDado(void)
{
 tipo_dados.clear();
}

bool FuncaoAgregacao::tipoDadoExiste(TipoPgSQL tipo)
{
 vector<TipoPgSQL>::iterator itr, itr_end;
 bool enc=false;

 itr=tipo_dados.begin();
 itr_end=tipo_dados.end();

 while(itr!=itr_end && !enc)
 {
  enc=((*itr)==tipo);
  itr++;
 }

 return(enc);
}

unsigned FuncaoAgregacao::obterNumTipoDados(void)
{
 return(tipo_dados.size());
}

Funcao *FuncaoAgregacao::obterFuncao(unsigned idx_func)
{
 //Verifica se o índice usado para referenciar a função é inválido
 if(idx_func!=FUNCAO_FINAL && idx_func!=FUNCAO_TRANSICAO)
  throw Exception(ERR_REF_FUNCTION_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Retorna a função no indice especificado
 return(funcoes[idx_func]);
}

TipoPgSQL FuncaoAgregacao::obterTipoEstado(void)
{
 return(tipo_estado);
}

QString FuncaoAgregacao::obterCondicaoInicial(void)
{
 return(cond_inicial);
}

Operador *FuncaoAgregacao::obterOperadorOrdenacao(void)
{
 return(op_ordenacao);
}

TipoPgSQL FuncaoAgregacao::obterTipoDado(unsigned idx_tipo)
{
 //Verifica se o índice usado para referenciar o tipo de dado é inválido
 if(idx_tipo >= tipo_dados.size())
  //Dispara uma exceção caso isso ocorra
  throw Exception(ERR_REF_TYPE_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Retorna o tipo na posição especificada
 return(tipo_dados[idx_tipo]);
}

QString FuncaoAgregacao::getCodeDefinition(unsigned tipo_def)
{
 definirAtributoTipos(tipo_def);

 if(funcoes[FUNCAO_TRANSICAO])
 {
  if(tipo_def==SchemaParser::SQL_DEFINITION)
   attributes[ParsersAttributes::TRANSITION_FUNC]=funcoes[FUNCAO_TRANSICAO]->obterAssinatura();
  else
  {
   funcoes[FUNCAO_TRANSICAO]->setAttribute(ParsersAttributes::REF_TYPE,
                                                     ParsersAttributes::TRANSITION_FUNC);
   attributes[ParsersAttributes::TRANSITION_FUNC]=funcoes[FUNCAO_TRANSICAO]->getCodeDefinition(tipo_def,true);
  }
 }

 if(funcoes[FUNCAO_FINAL])
 {
  if(tipo_def==SchemaParser::SQL_DEFINITION)
   attributes[ParsersAttributes::FINAL_FUNC]=funcoes[FUNCAO_FINAL]->obterAssinatura();
  else
  {
   funcoes[FUNCAO_FINAL]->setAttribute(ParsersAttributes::REF_TYPE,
                                                 ParsersAttributes::FINAL_FUNC);
   attributes[ParsersAttributes::FINAL_FUNC]=funcoes[FUNCAO_FINAL]->getCodeDefinition(tipo_def,true);
  }
 }

 if(op_ordenacao)
 {
  if(tipo_def==SchemaParser::SQL_DEFINITION)
   attributes[ParsersAttributes::SORT_OP]=op_ordenacao->getName(true);
  else
   attributes[ParsersAttributes::SORT_OP]=op_ordenacao->getCodeDefinition(tipo_def,true);
 }

 if(cond_inicial!="")
  attributes[ParsersAttributes::INITIAL_COND]=cond_inicial;

 if(tipo_def==SchemaParser::SQL_DEFINITION)
  attributes[ParsersAttributes::STATE_TYPE]=*(tipo_estado);
 else
  attributes[ParsersAttributes::STATE_TYPE]=tipo_estado.obterDefinicaoObjeto(tipo_def,ParsersAttributes::STATE_TYPE);

 return(BaseObject::getCodeDefinition(tipo_def));
}

