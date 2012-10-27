#include "resultset.h"

ResultSet::ResultSet(void)
{
 sql_result=NULL;
 empty_result=false;
 is_res_copied=false;
 current_tuple=-1;
}

ResultSet::ResultSet(PGresult *sql_result)
{
 QString str_aux;
 int res_state;

 if(!sql_result)
  throw Exception(ERR_CONEXBD_ATRRESSQLNAOALOC, __PRETTY_FUNCTION__, __FILE__, __LINE__);

 this->sql_result=sql_result;
 res_state=PQresultStatus(this->sql_result);

 //Trantando o estado do resultado
 switch(res_state)
 {
  //Gerando um erro caso o servidor retorna uma resposta incompreensível
  case PGRES_BAD_RESPONSE:
   throw Exception(ERR_CONEXBD_SGBDRESPINCOMP, __PRETTY_FUNCTION__, __FILE__, __LINE__);
  break;

  //Gerando um erro caso o servidor retorne um erro fatal
  case PGRES_FATAL_ERROR:
   str_aux=QString(Exception::getErrorMessage(ERR_CONEXBD_SGBDERROFATAL))
           .arg(PQresultErrorMessage(sql_result));
   throw Exception(str_aux,ERR_CONEXBD_SGBDERROFATAL, __PRETTY_FUNCTION__, __FILE__, __LINE__);
  break;

  //Gerando um erro caso o usuário tente obter resultado a partir de uma query vazia
  case PGRES_EMPTY_QUERY:
   throw Exception(ERR_CONEXBD_COMANDOSQLVAZIO, __PRETTY_FUNCTION__, __FILE__, __LINE__);
  break;

  //Estados de sucesso, o resultado será criado
  case PGRES_COMMAND_OK:
  case PGRES_TUPLES_OK:
  case PGRES_COPY_OUT:
  case PGRES_COPY_IN:
  default:
   empty_result=(res_state!=PGRES_TUPLES_OK);
   current_tuple=0;
   is_res_copied=false;
  break;
 }
}

ResultSet::~ResultSet(void)
{
 destroyResultSet();
}

void ResultSet::destroyResultSet(void)
{
 /* Destrói o result-set do objeto caso este não foi copiado
    para outra instância de classe (ver operador = ) */
 if(sql_result && !is_res_copied)
  PQclear(sql_result);

 //Reinicia os demais atributos
 sql_result=NULL;
 empty_result=false;
 is_res_copied=false;
 current_tuple=-1;
}

QString ResultSet::getColumnName(int column_idx)
{
 //Dispara um erro caso o índice da coluna seja inválido
 if(column_idx < 0 || column_idx >= getColumnCount())
  throw Exception(ERR_CONEXBD_REFCOLTUPLAIDXINV, __PRETTY_FUNCTION__, __FILE__, __LINE__);

 //Retorna o nome da coluna com índice especificado
 return(QString(PQfname(sql_result, column_idx)));
}

int ResultSet::getColumnIndex(const QString &column_name)
{
 int col_idx=-1;

 //Obtém o índice da coluna usando o nome
 col_idx=PQfnumber(sql_result, column_name.toStdString().c_str());

 /* Caso o índice seja negativo indica que a coluna não existe na tupla
    desta forma um erro é disparado */
 if(col_idx < 0)
  throw Exception(ERR_CONEXBD_REFCOLTUPLANOMEINV, __PRETTY_FUNCTION__, __FILE__, __LINE__);

 return(col_idx);
}

char *ResultSet::getColumnValue(const QString &column_name)
{
 int col_idx=-1;

 try
 {
  /* Dispara um erro caso o usuário tente obter o valor de uma coluna em 
     uma tupla de um resultado vazio ou gerado a partir de um comando INSERT, DELETE, UPDATE,
     ou seja, de comando os quais não retornam linhas mas apenas a atualizam/remove */
  if(getTupleCount()==0 || empty_result)
   throw Exception(ERR_CONEXBD_REFTUPLANAOEXISTE, __PRETTY_FUNCTION__, __FILE__, __LINE__);

  //Obtém o índice da coluna através do nome
  col_idx=getColumnIndex(column_name);
 }
 catch(Exception &e)
 {
  //Captura e redireciona qualquer exceção gerada
  throw Exception(e.getErrorMessage(), e.getErrorType(), __PRETTY_FUNCTION__, __FILE__, __LINE__, &e);
 }

 //Retorna o valor da coluna (idx_coluna) na linha atual (tupla_atual)
 return(PQgetvalue(sql_result, current_tuple, col_idx));
}

char *ResultSet::getColumnValue(int column_idx)
{
 //Dispara um erro caso o índice da coluna seja inválido
 if(column_idx < 0 || column_idx >= getColumnCount())
  throw Exception(ERR_CONEXBD_REFCOLTUPLAIDXINV, __PRETTY_FUNCTION__, __FILE__, __LINE__);
/* Dispara um erro caso o usuário tente obter o valor de uma coluna em 
     uma tupla de um resultado vazio ou gerado a partir de um comando INSERT, DELETE, UPDATE,
     ou seja, de comando os quais não retornam linhas mas apenas a atualizam/remove */
 else if(getTupleCount()==0 || empty_result)
  throw Exception(ERR_CONEXBD_REFTUPLANAOEXISTE, __PRETTY_FUNCTION__, __FILE__, __LINE__);

 //Retorna o valor da coluna (idx_coluna) na linha atual (tupla_atual)
 return(PQgetvalue(sql_result, current_tuple, column_idx));
}

int ResultSet::getColumnSize(const QString &column_name)
{
 int col_idx=-1;

 try
 {
  //Obtém o índice da coluna a ser detectado o comprimento
  col_idx=getColumnIndex(column_name);
 }
 catch(Exception &e)
 {
  //Captura e redireciona qualquer exceção gerada
  throw Exception(e.getErrorMessage(), e.getErrorType(), __PRETTY_FUNCTION__, __FILE__, __LINE__, &e);
 }

 //Retorna o comprimento do valor da coluna (idx_coluna) na linha atual (tupla_atual)
 return(PQgetlength(sql_result, current_tuple, col_idx));
}

int ResultSet::getColumnSize(int column_idx)
{
 //Dispara um erro caso o índice da coluna seja inválido
 if(column_idx < 0 || column_idx >= getColumnCount())
  throw Exception(ERR_CONEXBD_REFCOLTUPLAIDXINV, __PRETTY_FUNCTION__, __FILE__, __LINE__);

 //Retorna o comprimento do valor da coluna (idx_coluna) na linha atual (tupla_atual)
 return(PQgetlength(sql_result, current_tuple, column_idx));
}

int ResultSet::getTupleCount(void)
{
 //Caso o resultado possua tuplas
 if(!empty_result)
  //Retorna a quantidade de tuplas obtidas a partir do comando
  return(PQntuples(sql_result));
 else
  /* Retorna o número de linhas afetadas pelo comando (caso
     seja comandos do tipo INSERT, DELETE, UPDATE) */
  return(atoi(PQcmdTuples(sql_result)));
}

int ResultSet::getColumnCount(void)
{
 return(PQnfields(sql_result));
}

int ResultSet::getCurrentTuple(void)
{
 return(current_tuple);
}

bool ResultSet::isColumnBinaryFormat(const QString &column_name)
{
 int col_idx=-1;

 try
 {
  col_idx=getColumnIndex(column_name);
 }
 catch(Exception &e)
 {
  //Captura e redireciona qualquer exceção gerada
  throw Exception(e.getErrorMessage(), e.getErrorType(), __PRETTY_FUNCTION__, __FILE__, __LINE__, &e);
 }

 /* Retorna o formato da coluna (idx_coluna) na linha atual (tupla_atual).
    De acordo cmo a documentação da libpq, valor = 0, indica coluna em formato
    texto, já valor = 1 é coluna em formato binário, os demais valores
    são de uso reservado */
 return(PQfformat(sql_result, col_idx)==1);
}

bool ResultSet::isColumnBinaryFormat(int column_idx)
{
 //Dispara um erro caso o índice da coluna seja inválido
 if(column_idx < 0 || column_idx >= getColumnCount())
  throw Exception(ERR_CONEXBD_REFCOLTUPLAIDXINV, __PRETTY_FUNCTION__, __FILE__, __LINE__);

 /* Retorna o formato da coluna (idx_coluna) na linha atual (tupla_atual).
    De acordo cmo a documentação da libpq, valor = 0, indica coluna em formato
    texto, já valor = 1 é coluna em formato binário, os demais valores
    são de uso reservado */
 return(PQfformat(sql_result, column_idx)==1);
}

bool ResultSet::accessTuple(unsigned tuple_type)
{
 int tuple_count=getTupleCount();
 bool accessed=true;

 /* Dispara um erro caso o resultado não possua linhas ou
    seja derivado de um comando o qual apenas afeta linhas ou
    se o tipo de tupla a ser acessado seja inválido, fora do 
    conjunto definido pela classe */
 if(tuple_count==0 || empty_result || tuple_type > NEXT_TUPLE)
  throw Exception(ERR_CONEXBD_REFTUPLANAOEXISTE, __PRETTY_FUNCTION__, __FILE__, __LINE__);

 switch(tuple_type)
 {
  case FIRST_TUPLE:
   current_tuple=0;
  break;

  case LAST_TUPLE:
   current_tuple=tuple_count-1;
  break;

  case PREVIOUS_TUPLE:
   accessed=(current_tuple > 0);
   if(accessed) current_tuple--;
  break;

  case NEXT_TUPLE:
   accessed=(current_tuple < (tuple_count-1));
   if(accessed) current_tuple++;
  break;
 }

 return(accessed);
}

void ResultSet::operator = (ResultSet &res)
{
 /* Marca que o resultado do parâmetro foi copiado, evitando
    que seu result set seja desalocado */
 res.is_res_copied=true;

 /* Caso o resultado this esteja alocado,
    o mesmo será desalocado para evitar memory leaks */
 destroyResultSet();

 //Copia os atributos do resultado do parâmetro para o resultado this
 this->current_tuple=res.current_tuple;
 this->empty_result=res.empty_result;
 this->sql_result=res.sql_result;
 this->is_res_copied=false;
}

