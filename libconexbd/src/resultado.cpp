#include "resultado.h"
//***********************************************************
Resultado::Resultado(void)
{
 resultado_sql=NULL;
 res_sem_tuplas=false;
 res_copiado=false;
 tupla_atual=-1;
}
//-----------------------------------------------------------
Resultado::Resultado(PGresult *resultado_sql)
{
 QString str_aux;
 int estado_res;

 if(!resultado_sql)
  throw Excecao(ERR_CONEXBD_ATRRESSQLNAOALOC, __PRETTY_FUNCTION__, __FILE__, __LINE__);

 this->resultado_sql=resultado_sql;
 estado_res=PQresultStatus(this->resultado_sql);

 //Trantando o estado do resultado
 switch(estado_res)
 {
  //Gerando um erro caso o servidor retorna uma resposta incompreensível
  case PGRES_BAD_RESPONSE:
   throw Excecao(ERR_CONEXBD_SGBDRESPINCOMP, __PRETTY_FUNCTION__, __FILE__, __LINE__);
  break;

  //Gerando um erro caso o servidor retorne um erro fatal
  case PGRES_FATAL_ERROR:
   str_aux=QString(Excecao::obterMensagemErro(ERR_CONEXBD_SGBDERROFATAL))
           .arg(PQresultErrorMessage(resultado_sql));
   throw Excecao(str_aux,ERR_CONEXBD_SGBDERROFATAL, __PRETTY_FUNCTION__, __FILE__, __LINE__);
  break;

  //Gerando um erro caso o usuário tente obter resultado a partir de uma query vazia
  case PGRES_EMPTY_QUERY:
   throw Excecao(ERR_CONEXBD_COMANDOSQLVAZIO, __PRETTY_FUNCTION__, __FILE__, __LINE__);
  break;

  //Estados de sucesso, o resultado será criado
  case PGRES_COMMAND_OK:
  case PGRES_TUPLES_OK:
  case PGRES_COPY_OUT:
  case PGRES_COPY_IN:
  default:
   res_sem_tuplas=(estado_res!=PGRES_TUPLES_OK);
   tupla_atual=0;
   res_copiado=false;
  break;
 }
}
//-----------------------------------------------------------
Resultado::~Resultado(void)
{
 destruirResultado();
}
//-----------------------------------------------------------
void Resultado::destruirResultado(void)
{
 /* Destrói o result-set do objeto caso este não foi copiado
    para outra instância de classe (ver operador = ) */
 if(resultado_sql && !res_copiado)
  PQclear(resultado_sql);

 //Reinicia os demais atributos
 resultado_sql=NULL;
 res_sem_tuplas=false;
 res_copiado=false;
 tupla_atual=-1;
}
//-----------------------------------------------------------
QString Resultado::obterNomeColuna(int idx_coluna)
{
 //Dispara um erro caso o índice da coluna seja inválido
 if(idx_coluna < 0 || idx_coluna >= obterNumColunas())
  throw Excecao(ERR_CONEXBD_REFCOLTUPLAIDXINV, __PRETTY_FUNCTION__, __FILE__, __LINE__);

 //Retorna o nome da coluna com índice especificado
 return(QString(PQfname(resultado_sql, idx_coluna)));
}
//-----------------------------------------------------------
int Resultado::obterIndiceColuna(const QString &nome_coluna)
{
 int idx_coluna=-1;

 //Obtém o índice da coluna usando o nome
 idx_coluna=PQfnumber(resultado_sql, nome_coluna.toStdString().c_str());

 /* Caso o índice seja negativo indica que a coluna não existe na tupla
    desta forma um erro é disparado */
 if(idx_coluna < 0)
  throw Excecao(ERR_CONEXBD_REFCOLTUPLANOMEINV, __PRETTY_FUNCTION__, __FILE__, __LINE__);

 return(idx_coluna);
}
//-----------------------------------------------------------
char *Resultado::obterValorColuna(const QString &nome_coluna)
{
 int idx_coluna=-1;

 try
 {
  /* Dispara um erro caso o usuário tente obter o valor de uma coluna em 
     uma tupla de um resultado vazio ou gerado a partir de um comando INSERT, DELETE, UPDATE,
     ou seja, de comando os quais não retornam linhas mas apenas a atualizam/remove */
  if(obterNumTuplas()==0 || res_sem_tuplas)
   throw Excecao(ERR_CONEXBD_REFTUPLANAOEXISTE, __PRETTY_FUNCTION__, __FILE__, __LINE__);

  //Obtém o índice da coluna através do nome
  idx_coluna=obterIndiceColuna(nome_coluna);
 }
 catch(Excecao &e)
 {
  //Captura e redireciona qualquer exceção gerada
  throw Excecao(e.obterMensagemErro(), e.obterTipoErro(), __PRETTY_FUNCTION__, __FILE__, __LINE__, &e);
 }

 //Retorna o valor da coluna (idx_coluna) na linha atual (tupla_atual)
 return(PQgetvalue(resultado_sql, tupla_atual, idx_coluna));
}
//-----------------------------------------------------------
char *Resultado::obterValorColuna(int idx_coluna)
{
 //Dispara um erro caso o índice da coluna seja inválido
 if(idx_coluna < 0 || idx_coluna >= obterNumColunas())
  throw Excecao(ERR_CONEXBD_REFCOLTUPLAIDXINV, __PRETTY_FUNCTION__, __FILE__, __LINE__);
/* Dispara um erro caso o usuário tente obter o valor de uma coluna em 
     uma tupla de um resultado vazio ou gerado a partir de um comando INSERT, DELETE, UPDATE,
     ou seja, de comando os quais não retornam linhas mas apenas a atualizam/remove */
 else if(obterNumTuplas()==0 || res_sem_tuplas)
  throw Excecao(ERR_CONEXBD_REFTUPLANAOEXISTE, __PRETTY_FUNCTION__, __FILE__, __LINE__);

 //Retorna o valor da coluna (idx_coluna) na linha atual (tupla_atual)
 return(PQgetvalue(resultado_sql, tupla_atual, idx_coluna));
}
//-----------------------------------------------------------
int Resultado::obterTamanhoColuna(const QString &nome_coluna)
{
 int idx_coluna=-1;

 try
 {
  //Obtém o índice da coluna a ser detectado o comprimento
  idx_coluna=obterIndiceColuna(nome_coluna);
 }
 catch(Excecao &e)
 {
  //Captura e redireciona qualquer exceção gerada
  throw Excecao(e.obterMensagemErro(), e.obterTipoErro(), __PRETTY_FUNCTION__, __FILE__, __LINE__, &e);
 }

 //Retorna o comprimento do valor da coluna (idx_coluna) na linha atual (tupla_atual)
 return(PQgetlength(resultado_sql, tupla_atual, idx_coluna));
}
//-----------------------------------------------------------
int Resultado::obterTamanhoColuna(int idx_coluna)
{
 //Dispara um erro caso o índice da coluna seja inválido
 if(idx_coluna < 0 || idx_coluna >= obterNumColunas())
  throw Excecao(ERR_CONEXBD_REFCOLTUPLAIDXINV, __PRETTY_FUNCTION__, __FILE__, __LINE__);

 //Retorna o comprimento do valor da coluna (idx_coluna) na linha atual (tupla_atual)
 return(PQgetlength(resultado_sql, tupla_atual, idx_coluna));
}
//-----------------------------------------------------------
int Resultado::obterNumTuplas(void)
{
 //Caso o resultado possua tuplas
 if(!res_sem_tuplas)
  //Retorna a quantidade de tuplas obtidas a partir do comando
  return(PQntuples(resultado_sql));
 else
  /* Retorna o número de linhas afetadas pelo comando (caso
     seja comandos do tipo INSERT, DELETE, UPDATE) */
  return(atoi(PQcmdTuples(resultado_sql)));
}
//-----------------------------------------------------------
int Resultado::obterNumColunas(void)
{
 return(PQnfields(resultado_sql));
}
//-----------------------------------------------------------
int Resultado::obterTuplaAtual(void)
{
 return(tupla_atual);
}
//-----------------------------------------------------------
bool Resultado::colunaFormatoBinario(const QString &nome_coluna)
{
 int idx_coluna=-1;

 try
 {
  idx_coluna=obterIndiceColuna(nome_coluna);
 }
 catch(Excecao &e)
 {
  //Captura e redireciona qualquer exceção gerada
  throw Excecao(e.obterMensagemErro(), e.obterTipoErro(), __PRETTY_FUNCTION__, __FILE__, __LINE__, &e);
 }

 /* Retorna o formato da coluna (idx_coluna) na linha atual (tupla_atual).
    De acordo cmo a documentação da libpq, valor = 0, indica coluna em formato
    texto, já valor = 1 é coluna em formato binário, os demais valores
    são de uso reservado */
 return(PQfformat(resultado_sql, idx_coluna)==1);
}
//-----------------------------------------------------------
bool Resultado::colunaFormatoBinario(int idx_coluna)
{
 //Dispara um erro caso o índice da coluna seja inválido
 if(idx_coluna < 0 || idx_coluna >= obterNumColunas())
  throw Excecao(ERR_CONEXBD_REFCOLTUPLAIDXINV, __PRETTY_FUNCTION__, __FILE__, __LINE__);

 /* Retorna o formato da coluna (idx_coluna) na linha atual (tupla_atual).
    De acordo cmo a documentação da libpq, valor = 0, indica coluna em formato
    texto, já valor = 1 é coluna em formato binário, os demais valores
    são de uso reservado */
 return(PQfformat(resultado_sql, idx_coluna)==1);
}
//-----------------------------------------------------------
bool Resultado::acessarTupla(unsigned tipo_tupla)
{
 int num_tuplas=obterNumTuplas();
 bool acessado=true;

 /* Dispara um erro caso o resultado não possua linhas ou
    seja derivado de um comando o qual apenas afeta linhas ou
    se o tipo de tupla a ser acessado seja inválido, fora do 
    conjunto definido pela classe */
 if(num_tuplas==0 || res_sem_tuplas || tipo_tupla > TUPLA_POSTERIOR)
  throw Excecao(ERR_CONEXBD_REFTUPLANAOEXISTE, __PRETTY_FUNCTION__, __FILE__, __LINE__);

 switch(tipo_tupla)
 {
  case TUPLA_INICIAL:
   tupla_atual=0;
  break;

  case TUPLA_FINAL:
   tupla_atual=num_tuplas-1;
  break;

  case TUPLA_ANTERIOR:
   acessado=(tupla_atual > 0);
   if(acessado) tupla_atual--;
  break;

  case TUPLA_POSTERIOR:
   acessado=(tupla_atual < (num_tuplas-1));
   if(acessado) tupla_atual++;
  break;
 }

 return(acessado);
}
//-----------------------------------------------------------
void Resultado::operator = (Resultado &res)
{
 /* Marca que o resultado do parâmetro foi copiado, evitando
    que seu result set seja desalocado */
 res.res_copiado=true;

 /* Caso o resultado this esteja alocado,
    o mesmo será desalocado para evitar memory leaks */
 destruirResultado();

 //Copia os atributos do resultado do parâmetro para o resultado this
 this->tupla_atual=res.tupla_atual;
 this->res_sem_tuplas=res.res_sem_tuplas;
 this->resultado_sql=res.resultado_sql;
 this->res_copiado=false;
}
//***********************************************************
