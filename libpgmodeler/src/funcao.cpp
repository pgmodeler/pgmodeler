#include "funcao.h"

Parametro::Parametro(void)
{
 obj_type=OBJ_PARAMETER;
 entrada=saida=false;
}

void Parametro::definirTipo(TipoPgSQL tipo)
{
 this->tipo=tipo;
}

void Parametro::definirEntrada(bool valor)
{
 entrada=valor;
}

void Parametro::definirSaida(bool valor)
{
 saida=valor;
}

bool Parametro::parametroEntrada(void)
{
 return(entrada);
}

bool Parametro::parametroSaida(void)
{
 return(saida);
}

void Parametro::operator = (const Parametro &param)
{
 this->obj_name=param.obj_name;
 this->tipo=param.tipo;
 this->valor_padrao=param.valor_padrao;
 this->entrada=param.entrada;
 this->saida=param.saida;
}

QString Parametro::getCodeDefinition(unsigned tipo_def)
{
 //map<QString, QString> atributos;
 QString val_true, val_false;

 if(tipo_def==SchemaParser::SQL_DEFINITION)
  attributes[ParsersAttributes::NAME]=BaseObject::formatName(obj_name);
 else
  attributes[ParsersAttributes::NAME]=obj_name;

 attributes[ParsersAttributes::PARAM_IN]=(entrada ? "1" : "");
 attributes[ParsersAttributes::PARAM_OUT]=(saida ? "1" : "");
 attributes[ParsersAttributes::DEFAULT_VALUE]=valor_padrao;
 attributes[ParsersAttributes::TYPE]=tipo.obterDefinicaoObjeto(tipo_def);

 //return(ParserEsquema::obterDefinicaoObjeto(AtributosParsers::PARAMETRO,atributos, tipo_def));
 return(BaseObject::getCodeDefinition(tipo_def));
}

unsigned Funcao::function_id=40000;

Funcao::Funcao(void)
{
 object_id=Funcao::function_id++;
 tipo_retorno=TipoPgSQL("void");
 linguagem=NULL;
 retorna_setof=false;
 funcao_janela=false;
 obj_type=OBJ_FUNCTION;

 //Este dois valores são criados por padrão pelo SGBD
 custo_execucao=100;
 qtd_linhas=1000;

 attributes[ParsersAttributes::PARAMETERS]="";
 attributes[ParsersAttributes::EXECUTION_COST]="";
 attributes[ParsersAttributes::ROW_AMOUNT]="";
 attributes[ParsersAttributes::RETURN_TYPE]="";
 attributes[ParsersAttributes::FUNCTION_TYPE]="";
 attributes[ParsersAttributes::LANGUAGE]="";
 attributes[ParsersAttributes::RETURNS_SETOF]="";
 attributes[ParsersAttributes::SECURITY_TYPE]="";
 attributes[ParsersAttributes::BEHAVIOR_TYPE]="";
 attributes[ParsersAttributes::DEFINITION]="";
 attributes[ParsersAttributes::SIGNATURE]="";
 attributes[ParsersAttributes::REF_TYPE]="";
 attributes[ParsersAttributes::WINDOW_FUNC]="";
 attributes[ParsersAttributes::RETURN_TABLE]="";
 attributes[ParsersAttributes::LIBRARY]="";
 attributes[ParsersAttributes::SYMBOL]="";
}

void Funcao::setName(const QString &nome)
{
 BaseObject::setName(nome);
 criarAssinatura();
}

void Funcao::setSchema(BaseObject *esquema)
{
 BaseObject::setSchema(esquema);
 criarAssinatura();
}

void Funcao::adicionarParametro(Parametro param)
{
 vector<Parametro>::iterator itr,itr_end;
 bool enc=false;

 itr=params.begin();
 itr_end=params.end();

 /* Faz uma busca pela lista de parâmetros verificando se
    não existe um parâmetro com mesmo nome daquele que está
    sendo inserido. */
 while(itr!=itr_end && !enc)
 {
  /* Verifica se o nome do parametro atual ('itr->nome')
     é igual ao nome do novo param. ('nome') */
  enc=(itr->getName()==param.getName());
  itr++;
 }

 //Caso seja encontrado um parâmetro com mesmo nome
 if(enc)
  //Dispara exceção relatando o erro
  throw Exception(Exception::getErrorMessage(ERR_ASG_DUPLIC_PARAM_FUNCTION)
                .arg(QString::fromUtf8(param.getName()))
                .arg(QString::fromUtf8(this->assinatura)),
                ERR_ASG_DUPLIC_PARAM_FUNCTION,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
 {
  //Insere o parâmetro na lista de parâmetros
  params.push_back(param);
 }
 criarAssinatura();
}

void Funcao::adicionarTipoRetTabela(const QString &nome, TipoPgSQL tipo)
{
 //Verifica se o nome do elemento não está vazio
 if(nome=="")
  //Caso esteja vazio, dispara uma exceção relatando o erro
  throw Exception(ERR_ASG_EMPTY_NAME_RET_TABLE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
 //Caso não esteja vazio
 {
  vector<Parametro>::iterator itr,itr_end;
  bool enc=false;

  itr=tipos_ret_tabela.begin();
  itr_end=tipos_ret_tabela.end();

  /* Faz uma busca pela lista de de retorno verificando se
     não existe um elemento com mesmo nome daquele que está
     sendo inserido. */
  while(itr!=itr_end && !enc)
  {
   /* Verifica se o nome do parametro atual ('itr->nome')
      é igual ao nome do novo param. ('nome') */
   enc=(itr->getName()==nome);
   itr++;
  }

  //Caso seja encontrado um parâmetro com mesmo nome
  if(enc)
   //Dispara exceção relatando o erro
   throw Exception(Exception::getErrorMessage(ERR_INS_DUPLIC_RET_TAB_TYPE)
                 .arg(QString::fromUtf8(nome))
                 .arg(QString::fromUtf8(this->assinatura)),
                 ERR_INS_DUPLIC_RET_TAB_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  else
  {//Dispara exceção relatando o erro
   Parametro p;

   //Insere o parâmetro na lista de parâmetros
   p.setName(nome);
   p.definirTipo(tipo);
   tipos_ret_tabela.push_back(p);
  }
 }
}

void Funcao::definirAtributoParametros(unsigned tipo_def)
{
 QString str_param;
 unsigned i, qtd;

 qtd=params.size();
 for(i=0; i < qtd; i++)
 {
  str_param+=params[i].getCodeDefinition(tipo_def);
 }

 if(tipo_def==SchemaParser::SQL_DEFINITION)
  str_param.remove(str_param.size()-2,2);

 attributes[ParsersAttributes::PARAMETERS]=str_param;
}

void Funcao::definirAtributoTipoRetTabela(unsigned tipo_def)
{
 QString str_tipo;
 unsigned i, qtd;

 qtd=tipos_ret_tabela.size();
 for(i=0; i < qtd; i++)
 {
  str_tipo+=tipos_ret_tabela[i].getCodeDefinition(tipo_def);
 }

 if(tipo_def==SchemaParser::SQL_DEFINITION)
  str_tipo.remove(str_tipo.size()-2,2);

 attributes[ParsersAttributes::RETURN_TABLE]=str_tipo;
}

void Funcao::definirCustoExecucao(unsigned custo)
{
 custo_execucao=custo;
}

void Funcao::definirQuantidadeLinhas(unsigned qtd_linhas)
{
 this->qtd_linhas=qtd_linhas;
}

void Funcao::definirBiblioteca(const QString &biblioteca)
{
 if(linguagem->getName().toLower()!=~TipoLinguagem("c"))
  throw Exception(Exception::getErrorMessage(ERR_ASG_FUNC_REFLIB_LANG_NOT_C)
                .arg(QString::fromUtf8(this->obterAssinatura())),
                ERR_ASG_FUNC_REFLIB_LANG_NOT_C,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 this->biblioteca=biblioteca;
}

void Funcao::definirSimbolo(const QString &simbolo)
{
 if(linguagem->getName().toLower()!=~TipoLinguagem("c"))
  throw Exception(Exception::getErrorMessage(ERR_ASG_FUNC_REFLIB_LANG_NOT_C)
                .arg(QString::fromUtf8(this->obterAssinatura())),
                ERR_ASG_FUNC_REFLIB_LANG_NOT_C,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 this->simbolo=simbolo;
}

void Funcao::definirTipoRetorno(TipoPgSQL tipo)
{
 tipo_retorno=tipo;
}

void Funcao::definirTipoFuncao(TipoFuncao tipo)
{
 tipo_funcao=tipo;
}

void Funcao::definirLinguagem(BaseObject *linguagem)
{
 /* Caso se tente atribuir uma linguagem não alocada  função
    um erro é gerado */
 if(!linguagem)
  throw Exception(ERR_ASG_NOT_ALOC_LANGUAGE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 /* Caso se tente inserir um objeto alocado porém que não é uma linguagem
    um erro é gerado */
 else if(linguagem->getType()!=OBJ_LANGUAGE)
  throw Exception(ERR_ASG_INV_LANGUAGE_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
 {
  this->linguagem=linguagem;
 }
}

void Funcao::definirRetornaSetOf(bool valor)
{
 retorna_setof=valor;
}

void Funcao::definirFuncaoJanela(bool valor)
{
 funcao_janela=valor;
}

void Funcao::definirTipoSeguranca(TipoSeguranca tipo)
{
 tipo_seg=tipo;
}

void Funcao::definirTipoComportamento(TipoComportamento tipo)
{
 tipo_comportamento=tipo;
}

void Funcao::definirCodigoFonte(const QString &codigo)
{
 if(linguagem->getName().toLower()==~TipoLinguagem("c"))
  throw Exception(Exception::getErrorMessage(ERR_ASG_CODE_FUNC_C_LANGUAGE)
                .arg(QString::fromUtf8(this->obterAssinatura())),
                ERR_ASG_CODE_FUNC_C_LANGUAGE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 this->codigo_fonte=codigo;
}

TipoPgSQL Funcao::obterTipoRetorno(void)
{
 return(tipo_retorno);
}

TipoFuncao Funcao::obterTipoFuncao(void)
{
 return(tipo_funcao);
}

BaseObject *Funcao::obterLinguagem(void)
{
 return(linguagem);
}

unsigned Funcao::obterNumParams(void)
{
 return(params.size());
}

unsigned Funcao::obterNumTiposRetTabela(void)
{
 return(tipos_ret_tabela.size());
}

bool Funcao::retornaSetOf(void)
{
 return(retorna_setof);
}

bool Funcao::retornaTabela(void)
{
 return(tipos_ret_tabela.size() > 0);
}

bool Funcao::funcaoJanela(void)
{
 return(funcao_janela);
}

TipoSeguranca Funcao::obterTipoSeguranca(void)
{
 return(tipo_seg);
}

TipoComportamento Funcao::obterTipoComportamento(void)
{
 return(tipo_comportamento);
}

QString Funcao::obterCodigoFonte(void)
{
 return(codigo_fonte);
}

Parametro Funcao::obterParametro(unsigned idx_param)
{
 /* Caso o índice do parâmtro esteja fora da capacidade
    da lista de parâmetros */
 if(idx_param>=params.size())
  //Dispara exceção relatando o erro
  throw Exception(ERR_REF_PARAM_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
  //Retorna o parâmetro no índice desejado
  return(params[idx_param]);
}

Parametro Funcao::obterTipoRetTabela(unsigned idx_tipo)
{
 /* Caso o índice do tipo esteja fora da capacidade
    da lista de tipos de retorno */
 if(idx_tipo>=tipos_ret_tabela.size())
  //Dispara exceção relatando o erro
  throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
  //Retorna o item no índice desejado
  return(tipos_ret_tabela[idx_tipo]);
}

unsigned Funcao::obterCustoExecucao(void)
{
 return(custo_execucao);
}

unsigned Funcao::obterQuantidadeLinhas(void)
{
 return(qtd_linhas);
}

QString Funcao::obterBiblioteca(void)
{
 return(biblioteca);
}

QString Funcao::obterSimbolo(void)
{
 return(simbolo);
}

void Funcao::removerParametros(void)
{
 params.clear();
 criarAssinatura();
}

void Funcao::removerTiposRetTabela(void)
{
 tipos_ret_tabela.clear();
}

void Funcao::removerParametro(const QString &nome, TipoPgSQL tipo)
{
 vector<Parametro>::iterator itr,itr_end;

 itr=params.begin();
 itr_end=params.end();

 //Faz uma busca pela lista de parâmetros
 while(itr!=itr_end)
 {
  /* Verifica se o parâmetro atual (itr) tem o nome igual ao passado pelo método,
     o mesmo vale para o tipo */
  if(itr->getName()==nome && itr->obterTipo()==(~tipo))
  {
   //Caso o parâmetro seja encontrado
   params.erase(itr); //Exclui da lista
   break;
  }
  itr++;
 }

 criarAssinatura();
}

void Funcao::removerParametro(unsigned idx_param)
{
 /* Caso o índice do parâmtro esteja fora da capacidade
    da lista de parâmetros */
 if(idx_param>=params.size())
  //Dispara exceção relatando o erro
  throw Exception(ERR_REF_PARAM_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
 {
  vector<Parametro>::iterator itr;
  itr=params.begin()+idx_param;
  params.erase(itr); //Remove o parâmetro encontrado
 }

 criarAssinatura();
}

void Funcao::removerTipoRetTabela(unsigned idx_tipo)
{
 /* Caso o índice do parâmtro esteja fora da capacidade
    da lista de parâmetros */
 if(idx_tipo>=tipos_ret_tabela.size())
  //Dispara exceção relatando o erro
  throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
 {
  vector<Parametro>::iterator itr;
  itr=tipos_ret_tabela.begin()+idx_tipo;
  tipos_ret_tabela.erase(itr); //Remove o parâmetro encontrado
 }
}

QString Funcao::obterAssinatura(void)
{
 return(assinatura);
}

void Funcao::criarAssinatura(bool formatar)
{
 Parametro param;
 QString str_param;
 unsigned i, qtd;

 qtd=params.size();
 for(i=0; i < qtd; i++)
 {
  param=params[i];
  str_param+=(*param.obterTipo());

  if(i < (qtd-1)) str_param+=",";
 }

 //Formato da assinatura NOME(TIPO_PARAM1,TIPO_PARAM2,...,TIPO_PARAMn)
 assinatura=this->getName(formatar) + QString("(") + str_param + QString(")");
}

QString Funcao::getCodeDefinition(unsigned tipo_def)
{
 return(this->getCodeDefinition(tipo_def, false));
}

QString Funcao::getCodeDefinition(unsigned tipo_def, bool forma_reduzida)
{
 definirAtributoParametros(tipo_def);

 attributes[ParsersAttributes::EXECUTION_COST]=QString("%1").arg(custo_execucao);
 attributes[ParsersAttributes::ROW_AMOUNT]=QString("%1").arg(qtd_linhas);
 attributes[ParsersAttributes::FUNCTION_TYPE]=(~tipo_funcao);

 if(tipo_def==SchemaParser::SQL_DEFINITION)
 {
  attributes[ParsersAttributes::LANGUAGE]=linguagem->getName(false);
  attributes[ParsersAttributes::RETURN_TYPE]=(*tipo_retorno);
 }
 else
 {
  attributes[ParsersAttributes::LANGUAGE]=linguagem->getCodeDefinition(tipo_def,true);
  attributes[ParsersAttributes::RETURN_TYPE]=tipo_retorno.obterDefinicaoObjeto(tipo_def);
 }

 definirAtributoTipoRetTabela(tipo_def);

 attributes[ParsersAttributes::RETURNS_SETOF]=(retorna_setof ? "1" : "");
 attributes[ParsersAttributes::WINDOW_FUNC]=(funcao_janela ? "1" : "");
 attributes[ParsersAttributes::SECURITY_TYPE]=(~tipo_seg);
 attributes[ParsersAttributes::BEHAVIOR_TYPE]=(~tipo_comportamento);
 attributes[ParsersAttributes::DEFINITION]=codigo_fonte;

 if(linguagem->getName()==~TipoLinguagem(TipoLinguagem::c))
 {
  attributes[ParsersAttributes::SYMBOL]=simbolo;
  attributes[ParsersAttributes::LIBRARY]=biblioteca;
 }

 attributes[ParsersAttributes::SIGNATURE]=assinatura;
 return(BaseObject::getCodeDefinition(tipo_def, forma_reduzida));
}

