#include "confbasewidget.h"

void ConfBaseWidget::adicionarParamConfiguracao(const QString &param, const map<QString, QString> &atributos)
{
 //Adiciona um parâmetro somente se o identificador 'param' e os atributos não estejam vazio
 if(!param.isEmpty() && !atributos.empty())
  params_config[param]=atributos;
}

map<QString, map<QString, QString> > ConfBaseWidget::obterParamsConfiguracao(void)
{
 return(params_config);
}

void ConfBaseWidget::excluirParamConfiguracao(const QString &param)
{
 params_config.erase(param);
}

void ConfBaseWidget::excluirParamsConfiguracao(void)
{
 params_config.clear();
}

void ConfBaseWidget::salvarConfiguracao(const QString &id_conf)
{
 QString buf,
         //Configura o nome do arquivo de modelo (esquema) de configuração
         nome_arq_sch=GlobalAttributes::CONFIGURATIONS_DIR +
                      GlobalAttributes::DIR_SEPARATOR +
                      GlobalAttributes::SCHEMAS_DIR +
                      GlobalAttributes::DIR_SEPARATOR +
                      id_conf +
                      GlobalAttributes::SCHEMA_EXT,
         //Configura o nome do arquivo de configuração
         nome_arq=GlobalAttributes::CONFIGURATIONS_DIR +
                  GlobalAttributes::DIR_SEPARATOR +
                  id_conf +
                  GlobalAttributes::CONFIGURATION_EXT;
 QFile saida(nome_arq);
 map<QString, QString> atribs;
 map<QString, map<QString, QString> >::iterator itr, itr_end;

 try
 {
  itr=params_config.begin();
  itr_end=params_config.end();

  while(itr!=itr_end)
  {
   atribs.insert((itr->second).begin(), (itr->second).end());
   itr++;
  }

  //Gera o modelo de configuração com base nos parâmetros atuais
  buf=SchemaParser::getObjectDefinition(nome_arq_sch, atribs);

  //Abre o arquivo de configuração para gravação
  saida.open(QFile::WriteOnly);

  //Caso não consiga abrir o arquivo para gravação
  if(!saida.isOpen())
   throw Exception(Exception::getErrorMessage(ERR_FILE_NOT_WRITTEN).arg(QString::fromUtf8(nome_arq)),
                 ERR_FILE_NOT_WRITTEN,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  //Grava o buffer gerado no arquivo
  saida.write(buf.toStdString().c_str(), buf.size());
  saida.close();
 }
 catch(Exception &e)
 {
  if(saida.isOpen()) saida.close();
  throw Exception(Exception::getErrorMessage(ERR_FILE_NOT_WRITTER_INV_DEF).arg(QString::fromUtf8(nome_arq)),
                ERR_FILE_NOT_WRITTER_INV_DEF,__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}

void ConfBaseWidget::restaurarPadroes(const QString &id_conf)
{
 QString arq_atual, arq_orig;

 //Monta o caminho para o arquivo de configuração atual (conf/arquivo.conf)
 arq_atual=GlobalAttributes::CONFIGURATIONS_DIR +
           GlobalAttributes::DIR_SEPARATOR +
           id_conf +
           GlobalAttributes::CONFIGURATION_EXT;

 //Monta o caminho para o arquivo de configuração original (padrão) (conf/defaults/arquivo.conf)
 arq_orig=GlobalAttributes::CONFIGURATIONS_DIR +
          GlobalAttributes::DIR_SEPARATOR +
          GlobalAttributes::DEFAULT_CONFS_DIR+
          GlobalAttributes::DIR_SEPARATOR +
          id_conf +
          GlobalAttributes::CONFIGURATION_EXT;

 //Verifica a existência do arquivo padrão, caso não existe emite uma exceção e a restauração é abortada
 if(!QFile::exists(arq_orig))
  throw Exception(Exception::getErrorMessage(ERR_DEFAULT_CONFIG_NOT_REST).arg(QString::fromUtf8(arq_orig)),
                ERR_DEFAULT_CONFIG_NOT_REST,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
 {
  //Remove o arquivo atual
  QFile::remove(arq_atual);
  //Copia o arquivo original no lugar do arquivo atual
  QFile::copy(arq_orig, arq_atual);
 }
}

void ConfBaseWidget::carregarConfiguracao(const QString &id_conf, const vector<QString> &atribs_chave)
{
 try
 {
  //Limpa os parâmetros de configuração atuais
  params_config.clear();

  //Reinicia o parser XML para a leitura do arquivo
  XMLParser::restartParser();

  XMLParser::setDTDFile(GlobalAttributes::CONFIGURATIONS_DIR +
                               GlobalAttributes::DIR_SEPARATOR +
                               GlobalAttributes::OBJECT_DTD_DIR +
                               GlobalAttributes::DIR_SEPARATOR +
                               id_conf +
                               GlobalAttributes::OBJECT_DTD_EXT,
                               id_conf);

  XMLParser::loadXMLFile(GlobalAttributes::CONFIGURATIONS_DIR +
                                GlobalAttributes::DIR_SEPARATOR +
                                id_conf +
                                GlobalAttributes::CONFIGURATION_EXT);

  if(XMLParser::accessElement(XMLParser::CHILD_ELEMENT))
  {
   do
   {
    /* Certificando que só elementos xml serão lidos do parser,
      qualquer outro tipo de objeto xml será ignorado */
    if(XMLParser::getElementType()==XML_ELEMENT_NODE)
    {
     this->obterParamsConfiguracao(atribs_chave);

     if(XMLParser::hasElement(XMLParser::CHILD_ELEMENT))
     {
      XMLParser::savePosition();
      XMLParser::accessElement(XMLParser::CHILD_ELEMENT);

      do
      {
       this->obterParamsConfiguracao(atribs_chave);
      }
      while(XMLParser::accessElement(XMLParser::NEXT_ELEMENT));

      XMLParser::restorePosition();
     }
    }
   }
   while(XMLParser::accessElement(XMLParser::NEXT_ELEMENT));
  }
 }
 catch(Exception &e)
 {
  //Redireciona qualquer exceção capturada
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}

void ConfBaseWidget::obterParamsConfiguracao(const vector<QString> &atribs_chave)
{
 map<QString, QString> atrib_aux;
 map<QString, QString>::iterator itr, itr_end;
 QString chave;

 //Obtém os atributos do elemento atual
 XMLParser::getElementAttributes(atrib_aux);

 itr=atrib_aux.begin();
 itr_end=atrib_aux.end();

 //Substituindo a chave pelo atributo chave passado
 while(itr!=itr_end && chave.isEmpty())
 {
  //Caso o atributo chave seja encontrado em um dos atributos obtidos do elemento
  if(chave.isEmpty() && find(atribs_chave.begin(), atribs_chave.end(), itr->first)!=atribs_chave.end())
   //A chave passa a ser o valor do atributo encontrado
   chave=itr->second;
  itr++;
 }

 //Caso a chave não foi definida, a chave padrão será o nome do elemento
 if(chave.isEmpty())
  chave=XMLParser::getElementName();

 //Atribui os dados obtidos do elemento atual   chave do mapa de parâmetros de configuração
 if(!atrib_aux.empty())
  params_config[chave]=atrib_aux;
}

