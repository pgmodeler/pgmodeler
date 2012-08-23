#include "conexaobd.h"
//***********************************************************
const QString ConexaoBD::SSL_DESATIVADO="disable";
const QString ConexaoBD::SSL_PERMITIR="allow";
const QString ConexaoBD::SSL_PREFERIR="prefer";
const QString ConexaoBD::SSL_REQUERER="require";
const QString ConexaoBD::SSL_VERIF_AUT_CERT="verify-ca";
const QString ConexaoBD::SSL_VERIF_COMPLETA="verify-full";
const QString ConexaoBD::PARAM_FQDN_SERVIDOR="host";
const QString ConexaoBD::PARAM_IP_SERVIDOR="hostaddr";
const QString ConexaoBD::PARAM_PORTA="port";
const QString ConexaoBD::PARAM_NOME_BD="dbname";
const QString ConexaoBD::PARAM_USUARIO="user";
const QString ConexaoBD::PARAM_SENHA="password";
const QString ConexaoBD::PARAM_TEMPO_CONEXAO="connect_timeout";
const QString ConexaoBD::PARAM_OPCOES="options";
const QString ConexaoBD::PARAM_MODO_SSL="sslmode";
const QString ConexaoBD::PARAM_CERT_SSL="sslcert";
const QString ConexaoBD::PARAM_CHAVE_SSL="sslkey";
const QString ConexaoBD::PARAM_CERT_RAIZ_SSL="sslrootcert";
const QString ConexaoBD::PARAM_CRL_SSL="sslcrl";
const QString ConexaoBD::PARAM_SERVIDOR_KERBEROS="krbsrvname";
const QString ConexaoBD::PARAM_LIB_GSSAPI="gsslib";
//-----------------------------------------------------------
ConexaoBD::ConexaoBD(void)
{
 conexao=NULL;
}
//-----------------------------------------------------------
ConexaoBD::ConexaoBD(const QString &servidor, const QString &porta, const QString &usuario, const QString &senha, const QString &nome_bd)
{
 //Configura os parâmetros básicos de conexão
 definirParamConexao(PARAM_FQDN_SERVIDOR, servidor);
 definirParamConexao(PARAM_PORTA, porta);
 definirParamConexao(PARAM_USUARIO, usuario);
 definirParamConexao(PARAM_SENHA, senha);
 definirParamConexao(PARAM_NOME_BD, nome_bd);

 //Estabelece a conexão
 conectar();
}
//-----------------------------------------------------------
ConexaoBD::~ConexaoBD(void)
{
 if(conexao)
  PQfinish(conexao);
}
//-----------------------------------------------------------
void ConexaoBD::definirParamConexao(const QString &parametro, const QString &valor)
{
 //Dispara um erro caso se tente atribuir um valor a um parâmetro vazio
 if(parametro=="")
  throw Excecao(ERR_CONEXBD_ATRPARAMINVCONEX, __PRETTY_FUNCTION__, __FILE__, __LINE__);

 //Atribui o valor ao parâmetro selecionado e gera a string de conexão
 params_conexao[parametro]=valor;
 gerarStringConexao();
}
//-----------------------------------------------------------
void ConexaoBD::gerarStringConexao(void)
{
 map<QString, QString>::iterator itr;

 itr=params_conexao.begin();

 //Varre o mapa de parâmetros de conexão
 str_conexao="";
 while(itr!=params_conexao.end())
 {
  //Concatena cada parâmetro a seu valor, separândo-os por um sinal de igual
  if(!itr->second.isEmpty())
   str_conexao+=itr->first + "=" + itr->second + " ";
  itr++;
 }
}
//-----------------------------------------------------------
void ConexaoBD::conectar(void)
{
 QString str_aux;

 /* Caso a string de conexão não esteja estabelecida indica que o usuário
    está tentado conectar sem configurar os parâmetros de conexão, sendo
    assim um erro será disparado */
 if(str_conexao=="")
  throw Excecao(ERR_CONEXBD_CONEXSEMPARAMCONFIG, __PRETTY_FUNCTION__, __FILE__, __LINE__);

 //Tenta conectar ao banco de dados
 conexao=PQconnectdb(str_conexao.toStdString().c_str());

 /* Caso o descritor de conexão não foi alocado ou se o estado da conexão
    seja CONNECTION_BAD indica que a conexão não foi bem sucedida */
 if(conexao==NULL || PQstatus(conexao)==CONNECTION_BAD)
 {
  //Dispara o erro gerado pelo SGBD
  str_aux=QString(Excecao::obterMensagemErro(ERR_CONEXBD_CONEXNAOESTABELECIDA))
          .arg(PQerrorMessage(conexao));
  throw Excecao(str_aux, ERR_CONEXBD_CONEXNAOESTABELECIDA,
                __PRETTY_FUNCTION__, __FILE__, __LINE__);
 }
}
//-----------------------------------------------------------
void ConexaoBD::fechar(void)
{
 //Dispara um erro caso o usuário tente fechar uma conexão não iniciada
 if(!conexao)
  throw Excecao(ERR_CONEXBD_OPRCONEXNAOALOC, __PRETTY_FUNCTION__, __FILE__, __LINE__);

 //Finaliza a conexão
 PQfinish(conexao);
 conexao=NULL;
}
//-----------------------------------------------------------
void ConexaoBD::reiniciar(void)
{
 //Dispara um erro caso o usuário tente reiniciar uma conexão não iniciada
 if(!conexao)
  throw Excecao(ERR_CONEXBD_OPRCONEXNAOALOC, __PRETTY_FUNCTION__, __FILE__, __LINE__);

 //Reinicia a conexão
 PQreset(conexao);
}
//-----------------------------------------------------------
QString ConexaoBD::obterParamConexao(const QString &parametro)
{
 return(params_conexao[parametro]);
}
//-----------------------------------------------------------
map<QString, QString> ConexaoBD::obterParamsConexao(void)
{
 return(params_conexao);
}
//-----------------------------------------------------------
QString ConexaoBD::obterStringConexao(void)
{
 return(str_conexao);
}
//-----------------------------------------------------------
bool ConexaoBD::conexaoEstabelecida(void)
{
 return(conexao!=NULL);
}
//-----------------------------------------------------------
QString  ConexaoBD::obterVersaoSGBD(void)
{
 QString versao;

 if(!conexao)
  throw Excecao(ERR_CONEXBD_OPRCONEXNAOALOC, __PRETTY_FUNCTION__, __FILE__, __LINE__);

 versao=QString("%1").arg(PQserverVersion(conexao));

 return(QString("%1.%2.%3")
        .arg(versao.mid(0,2).toInt()/10)
        .arg(versao.mid(2,2).toInt()/10)
        .arg(versao.mid(4,1).toInt()));
}
//-----------------------------------------------------------
void ConexaoBD::executarComandoDML(const QString &sql, Resultado &resultado)
{
 Resultado *novo_res=NULL;
 PGresult *res_sql=NULL;

 //Dispara um erro caso o usuário tente reiniciar uma conexão não iniciada
 if(!conexao)
  throw Excecao(ERR_CONEXBD_OPRCONEXNAOALOC, __PRETTY_FUNCTION__, __FILE__, __LINE__);

 //Aloca um novo resultado para receber o result-set vindo da execução do comando sql
 res_sql=PQexec(conexao, sql.toStdString().c_str());
 if(strlen(PQerrorMessage(conexao))>0)
 {
  throw Excecao(QString(Excecao::obterMensagemErro(ERR_CONEXBD_CMDSQLNAOEXECUTADO))
                .arg(PQerrorMessage(conexao)),
                ERR_CONEXBD_CMDSQLNAOEXECUTADO, __PRETTY_FUNCTION__, __FILE__, __LINE__, NULL,
                QString(PQresultErrorField(res_sql, PG_DIAG_SQLSTATE)));
 }

 novo_res=new Resultado(res_sql);
 //Copia o novo resultado para o resultado do parâmetro
 resultado=*(novo_res);
 //Desaloca o resultado criado
 delete(novo_res);
}
//-----------------------------------------------------------
void ConexaoBD::executarComandoDDL(const QString &sql)
{
 PGresult *res_sql=NULL;

 //Dispara um erro caso o usuário tente reiniciar uma conexão não iniciada
 if(!conexao)
  throw Excecao(ERR_CONEXBD_OPRCONEXNAOALOC, __PRETTY_FUNCTION__, __FILE__, __LINE__);

 res_sql=PQexec(conexao, sql.toStdString().c_str());

 if(strlen(PQerrorMessage(conexao)) > 0)
 {
  throw Excecao(QString(Excecao::obterMensagemErro(ERR_CONEXBD_CMDSQLNAOEXECUTADO))
                .arg(PQerrorMessage(conexao)),
                ERR_CONEXBD_CMDSQLNAOEXECUTADO, __PRETTY_FUNCTION__, __FILE__, __LINE__, NULL,
                QString(PQresultErrorField(res_sql, PG_DIAG_SQLSTATE)));
 }
}
//-----------------------------------------------------------
void ConexaoBD::operator = (ConexaoBD &conex)
{
 if(this->conexaoEstabelecida())
  this->fechar();

 this->params_conexao=conex.params_conexao;
 this->str_conexao=conex.str_conexao;
 conectar();
}
//***********************************************************
