#include "gatilho.h"
//***********************************************************
Gatilho::Gatilho(void)
{
 unsigned i, tipos[4]={TipoEvento::on_insert, TipoEvento::on_delete,
                       TipoEvento::on_update, TipoEvento::on_truncate};

 funcao=NULL;
 por_linha=false;
 tipo_objeto=OBJETO_GATILHO;
 tabela_ref=NULL;

 for(i=0; i < 4; i++)
  eventos[tipos[i]]=false;

 atributos[AtributosParsers::ARGUMENTOS]="";
 atributos[AtributosParsers::EVENTOS]="";
 atributos[AtributosParsers::FUNCAO_GAT]="";
 atributos[AtributosParsers::TABELA]="";
 atributos[AtributosParsers::COLUNAS]="";
 atributos[AtributosParsers::TIPO_DISPARO]="";
 atributos[AtributosParsers::POR_LINHA]="";
 atributos[AtributosParsers::EVENTO_INS]="";
 atributos[AtributosParsers::EVENTO_DEL]="";
 atributos[AtributosParsers::EVENTO_UPD]="";
 atributos[AtributosParsers::EVENTO_TRUNC]="";
 atributos[AtributosParsers::CONDICAO]="";
 atributos[AtributosParsers::TABELA_REF]="";
 atributos[AtributosParsers::TIPO_POSTERGACAO]="";
 atributos[AtributosParsers::POSTERGAVEL]="";
 atributos[AtributosParsers::DECL_DENTRO_TABELA]="";
}
//-----------------------------------------------------------
void Gatilho::adicionarArgumento(const QString &arg)
{
 argumentos.push_back(arg);
}
//-----------------------------------------------------------
void Gatilho::definirAtributoArgumentos(unsigned tipo_def)
{
 QString str_args;
 unsigned i, qtd;

 qtd=argumentos.size();
 for(i=0; i < qtd; i++)
 {
  if(tipo_def==ParserEsquema::DEFINICAO_SQL)
   str_args+="'" + argumentos[i] + "'";
  else
   str_args+=argumentos[i];

  if(i < (qtd-1)) str_args+=",";
 }

 atributos[AtributosParsers::ARGUMENTOS]=str_args;
}
//-----------------------------------------------------------
void Gatilho::definirTipoDisparo(TipoDisparo tipo_disp)
{
 tipo_disparo=tipo_disp;
}
//-----------------------------------------------------------
void Gatilho::definirEvento(TipoEvento evento, bool valor)
{
 if(evento==TipoEvento::on_select)
  throw Excecao(ERR_PGMODELER_REFEVENTOGATINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 eventos[!evento]=valor;
}
//-----------------------------------------------------------
void Gatilho::definirFuncao(Funcao *funcao)
{
 //Caso a função a ser atribuida ao gatilho esteja nula
 if(!funcao)
  //Dispara exceção relatando o erro
  throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELER_ATRFUNCNAOALOC)
                         .arg(this->obterNome())
                         .arg(ObjetoBase::obterNomeTipoObjeto(OBJETO_GATILHO)),
                ERR_PGMODELER_ATRFUNCNAOALOC,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
 {
  //Caso a função não possua tipo de retorno 'trigger', ela não pode ser usada em um gatilho
  if(funcao->obterTipoRetorno()!="trigger")
   //Dispara exceção relatando o erro
   throw Excecao(ERR_PGMODELER_ATRFUNCGATINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  //Caso a função não possua parâmetros, ela não pode ser usada em um gatilho
  else if(funcao->obterNumParams()==0)
   //Dispara exceção relatando o erro
   throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELER_ATRFUNCNUMPARAMINV)
                         .arg(this->obterNome())
                         .arg(ObjetoBase::obterNomeTipoObjeto(OBJETO_GATILHO)),
                 ERR_PGMODELER_ATRFUNCNUMPARAMINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  else
   this->funcao=funcao;
 }
}
//-----------------------------------------------------------
void Gatilho::definirCondicao(const QString &cond)
{
 this->condicao=cond;
}
//-----------------------------------------------------------
void Gatilho::adicionarColuna(Coluna *coluna)
{
 if(!coluna)
  throw Excecao(QString(Excecao::obterMensagemErro(ERR_PGMODELER_ATRCOLNAOALOC))
                .arg(this->obterNome(true))
                .arg(this->obterNomeTipoObjeto()),
                ERR_PGMODELER_ATRCOLNAOALOC,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else if(coluna->obterTabelaPai() != this->obterTabelaPai())
  throw Excecao(QString(Excecao::obterMensagemErro(ERR_PGMODELER_ATRCOLINVGATILHO))
                .arg(coluna->obterNome(true))
                .arg(this->obterNome(true)),
                ERR_PGMODELER_ATRCOLINVGATILHO,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  colunas_upd.push_back(coluna);
}
//-----------------------------------------------------------
void Gatilho::editarArgumento(unsigned idx_arg, const QString &novo_arg)
{
 /* Caso o índice do argumento esteja fora da capacidade
    da lista de argumentos */
 if(idx_arg>=argumentos.size())
  //Dispara exceção relatando o erro
  throw Excecao(ERR_PGMODELER_REFARGIDXINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
 {
  vector<QString>::iterator itr;

  itr=argumentos.begin()+idx_arg;
  (*itr)=novo_arg; //Muda o valor do argumento
 }
}
//-----------------------------------------------------------
void Gatilho::executarPorLinha(bool valor)
{
 por_linha=valor;
}
//-----------------------------------------------------------
bool Gatilho::executaNoEvento(TipoEvento evento)
{
 if(evento==TipoEvento::on_select)
  throw Excecao(ERR_PGMODELER_REFEVENTOGATINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 return(eventos.at(!evento));
}
//-----------------------------------------------------------
QString Gatilho::obterArgumento(unsigned idx_arg)
{
 /* Caso o índice do argumento esteja fora da capacidade
    da lista de argumentos */
 if(idx_arg>=argumentos.size())
  //Dispara exceção relatando o erro
  throw Excecao(ERR_PGMODELER_REFARGIDXINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
  //Retorna o argumento no índice desejado
  return(argumentos[idx_arg]);
}
//-----------------------------------------------------------
Coluna *Gatilho::obterColuna(unsigned idx_col)
{
 if(idx_col>=colunas_upd.size())
  throw Excecao(ERR_PGMODELER_REFCOLIDXINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
  //Retorna a coluna no índice desejado
  return(colunas_upd[idx_col]);
}
//-----------------------------------------------------------
unsigned Gatilho::obterNumArgs(void)
{
 return(argumentos.size());
}
//-----------------------------------------------------------
unsigned Gatilho::obterNumColunas(void)
{
 return(colunas_upd.size());
}
//-----------------------------------------------------------
Funcao *Gatilho::obterFuncao(void)
{
 return(funcao);
}
//-----------------------------------------------------------
QString Gatilho::obterCondicao(void)
{
 return(condicao);
}
//-----------------------------------------------------------
TipoDisparo Gatilho::obterTipoDisparo(void)
{
 return(tipo_disparo);
}
//-----------------------------------------------------------
void Gatilho::removerArgumento(unsigned idx_arg)
{
 /* Caso o índice do argumento steja fora da capacidade
    da lista de argumentos */
 if(idx_arg>=argumentos.size())
  //Dispara exceção relatando o erro
  throw Excecao(ERR_PGMODELER_REFARGIDXINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
 {
  vector<QString>::iterator itr;
  itr=argumentos.begin()+idx_arg;
  argumentos.erase(itr); //Remove o argumento encontrado
 }
}
//-----------------------------------------------------------
void Gatilho::removerArgumentos(void)
{
 argumentos.clear();
}
//-----------------------------------------------------------
void Gatilho::removerColunas(void)
{
 colunas_upd.clear();
}
//-----------------------------------------------------------
void Gatilho::definirTabReferenciada(ObjetoBase *tabela_ref)
{
 /* Caso a tabela referenciada a ser atribuída esteja alocada, porém
    seu tipo não seja OBJETO_TABELA, isso gera um erro */
 if(tabela_ref && tabela_ref->obterTipoObjeto()!=OBJETO_TABELA)
  throw Excecao(ERR_PGMODELER_ATROBJTIPOINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Atribui a tabela referenciada ao gatilho
 this->tabela_ref=tabela_ref;
}
//-----------------------------------------------------------
void Gatilho::definirTipoPostergacao(TipoPostergacao tipo)
{
 tipo_postergacao=tipo;
}
//-----------------------------------------------------------
void Gatilho::definirPostergavel(bool valor)
{
 postergavel=valor;
}
//-----------------------------------------------------------
ObjetoBase *Gatilho::obterTabReferenciada(void)
{
 return(tabela_ref);
}
//-----------------------------------------------------------
TipoPostergacao Gatilho::obterTipoPostergacao(void)
{
 return(tipo_postergacao);
}
//-----------------------------------------------------------
bool Gatilho::gatilhoPostergavel(void)
{
 return(postergavel);
}
//-----------------------------------------------------------
bool Gatilho::referenciaColunaIncRelacao(void)
{
 vector<Coluna *>::iterator itr, itr_end;
 Coluna *col=NULL;
 bool enc=false;

 itr=colunas_upd.begin();
 itr_end=colunas_upd.end();

 /* Efetua uma iteração verifica se as colunas da lista
    foram incluídas por relacionamento, caso uma coluna
    for detectada como incluída desta forma é suficiente
    dizer que o gatilho referencia colunas vindas de
    relacionamento fazendo com que esta seja tratada de forma
    especial evitando a quebra de referêncais */
 while(itr!=itr_end && !enc)
 {
  //Obtém a coluna
  col=(*itr);
  //Obtém se a coluna foi incluída por relacionamento ou não
  enc=col->incluidoPorRelacionamento();
  //Passa para a próxima coluna
  itr++;
 }

 return(enc);
}
//-----------------------------------------------------------
void Gatilho::definirAtributosBasicosGatilho(unsigned tipo_def)
{
 QString str_aux,
         atribs[4]={AtributosParsers::EVENTO_INS, AtributosParsers::EVENTO_DEL,
                    AtributosParsers::EVENTO_TRUNC, AtributosParsers::EVENTO_UPD },
         sql_evento[4]={"INSERT OR ", "DELETE OR ", "TRUNCATE OR ", "UPDATE   "};
 unsigned qtd, i, i1, tipo_eventos[4]={TipoEvento::on_insert, TipoEvento::on_delete,
                                   TipoEvento::on_truncate, TipoEvento::on_update};


 definirAtributoArgumentos(tipo_def);

 for(i=0; i < 4; i++)
 {
  if(eventos.at(tipo_eventos[i]))
  {
   str_aux+=sql_evento[i];
   atributos[atribs[i]]="1";

   if(tipo_eventos[i]==TipoEvento::on_update)
   {
    qtd=colunas_upd.size();
    atributos[AtributosParsers::COLUNAS]="";
    for(i1=0; i1 < qtd; i1++)
    {
     atributos[AtributosParsers::COLUNAS]+=colunas_upd.at(i1)->obterNome(true);
     if(i1 < qtd-1)
      atributos[AtributosParsers::COLUNAS]+=",";
    }
   }
  }
 }

 if(str_aux!="") str_aux.remove(str_aux.size()-3,3);
 atributos[AtributosParsers::EVENTOS]=str_aux;

 if(funcao)
 {
  if(tipo_def==ParserEsquema::DEFINICAO_SQL)
   atributos[AtributosParsers::FUNCAO_GAT]=funcao->obterNome(true);
  else
   atributos[AtributosParsers::FUNCAO_GAT]=funcao->obterDefinicaoObjeto(tipo_def, true);
 }
}
//-----------------------------------------------------------
QString Gatilho::obterDefinicaoObjeto(unsigned tipo_def)
{
 definirAtributosBasicosGatilho(tipo_def);

 /* Caso o gatilho não esteja referenciando alguma coluna incluída por relacionamento
    a mesma será declarada dentro do código da tabela pai e para tanto existe um atributo
    específico na definição SQL/XML do objeto chamado 'decl-in-table' que é usado
    para indicar ao parser quando a declaração do gatilho está dentro da declaração da
    tabela pai. Este atributo é usado apenas para ajudar na formatação do código SQL e
    não tem nenhuma outra utilidade. */
 if(!referenciaColunaIncRelacao())
  atributos[AtributosParsers::DECL_DENTRO_TABELA]="1";

 if(this->tabela_pai)
  atributos[AtributosParsers::TABELA]=this->tabela_pai->obterNome(true);

 atributos[AtributosParsers::TIPO_DISPARO]=(~tipo_disparo);

 //** Gatilhos Restrições SEMPRE devem executar por linha (FOR EACH ROW) **
 atributos[AtributosParsers::POR_LINHA]=((por_linha && !tabela_ref) || tabela_ref ? "1" : "");

 atributos[AtributosParsers::CONDICAO]=condicao;

 if(tabela_ref)
 {
  atributos[AtributosParsers::TABELA_REF]=tabela_ref->obterNome(true);
  atributos[AtributosParsers::POSTERGAVEL]=(postergavel ? "1" : "");
  atributos[AtributosParsers::TIPO_POSTERGACAO]=(~tipo_postergacao);
 }

 return(ObjetoBase::obterDefinicaoObjeto(tipo_def));
}
//***********************************************************
