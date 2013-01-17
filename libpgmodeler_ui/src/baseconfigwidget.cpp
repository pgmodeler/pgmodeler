#include "baseconfigwidget.h"

void BaseConfigWidget::addConfigurationParam(const QString &param, const map<QString, QString> &attribs)
{
 //Adiciona um parâmetro somente se o identificador 'param' e os atributos não estejam vazio
 if(!param.isEmpty() && !attribs.empty())
  config_params[param]=attribs;
}

map<QString, map<QString, QString> > BaseConfigWidget::getConfigurationParams(void)
{
 return(config_params);
}

void BaseConfigWidget::removeConfigurationParam(const QString &param)
{
 config_params.erase(param);
}

void BaseConfigWidget::removeConfigurationParams(void)
{
 config_params.clear();
}

void BaseConfigWidget::saveConfiguration(const QString &conf_id)
{
 QString buf,
         //Configura o nome do arquivo de modelo (esquema) de configuração
         sch_filename=GlobalAttributes::CONFIGURATIONS_DIR +
                      GlobalAttributes::DIR_SEPARATOR +
                      GlobalAttributes::SCHEMAS_DIR +
                      GlobalAttributes::DIR_SEPARATOR +
                      conf_id +
                      GlobalAttributes::SCHEMA_EXT,
         //Configura o nome do arquivo de configuração
         cfg_filename=GlobalAttributes::CONFIGURATIONS_DIR +
                      GlobalAttributes::DIR_SEPARATOR +
                      conf_id +
                      GlobalAttributes::CONFIGURATION_EXT;
 QFile output(cfg_filename);
 map<QString, QString> attribs;
 map<QString, map<QString, QString> >::iterator itr, itr_end;

 try
 {
  itr=config_params.begin();
  itr_end=config_params.end();

  while(itr!=itr_end)
  {
   attribs.insert((itr->second).begin(), (itr->second).end());
   itr++;
  }

  //Gera o modelo de configuração com base nos parâmetros atuais
  buf=SchemaParser::getCodeDefinition(sch_filename, attribs);

  //Abre o arquivo de configuração para gravação
  output.open(QFile::WriteOnly);

  //Caso não consiga abrir o arquivo para gravação
  if(!output.isOpen())
   throw Exception(Exception::getErrorMessage(ERR_FILE_NOT_WRITTEN).arg(QString::fromUtf8(cfg_filename)),
                 ERR_FILE_NOT_WRITTEN,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  //Grava o buffer gerado no arquivo
  output.write(buf.toStdString().c_str(), buf.size());
  output.close();
 }
 catch(Exception &e)
 {
  if(output.isOpen()) output.close();
  throw Exception(Exception::getErrorMessage(ERR_FILE_NOT_WRITTER_INV_DEF).arg(QString::fromUtf8(cfg_filename)),
                ERR_FILE_NOT_WRITTER_INV_DEF,__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}

void BaseConfigWidget::restoreDefaults(const QString &conf_id)
{
 QString current_file, original_file;

 //Monta o caminho para o arquivo de configuração atual (conf/arquivo.conf)
 current_file=GlobalAttributes::CONFIGURATIONS_DIR +
           GlobalAttributes::DIR_SEPARATOR +
           conf_id +
           GlobalAttributes::CONFIGURATION_EXT;

 //Monta o caminho para o arquivo de configuração original (padrão) (conf/defaults/arquivo.conf)
 original_file=GlobalAttributes::CONFIGURATIONS_DIR +
               GlobalAttributes::DIR_SEPARATOR +
               GlobalAttributes::DEFAULT_CONFS_DIR+
               GlobalAttributes::DIR_SEPARATOR +
               conf_id +
               GlobalAttributes::CONFIGURATION_EXT;

 //Verifica a existência do arquivo padrão, caso não existe emite uma exceção e a restauração é abortada
 if(!QFile::exists(original_file))
  throw Exception(Exception::getErrorMessage(ERR_DEFAULT_CONFIG_NOT_REST).arg(QString::fromUtf8(original_file)),
                ERR_DEFAULT_CONFIG_NOT_REST,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
 {
  //Remove o arquivo atual
  QFile::remove(current_file);
  //Copia o arquivo original no lugar do arquivo atual
  QFile::copy(original_file, current_file);
 }
}

void BaseConfigWidget::loadConfiguration(const QString &conf_id, const vector<QString> &key_attribs)
{
 try
 {
  //Limpa os parâmetros de configuração atuais
  config_params.clear();

  //Reinicia o parser XML para a leitura do arquivo
  XMLParser::restartParser();

  XMLParser::setDTDFile(GlobalAttributes::CONFIGURATIONS_DIR +
                               GlobalAttributes::DIR_SEPARATOR +
                               GlobalAttributes::OBJECT_DTD_DIR +
                               GlobalAttributes::DIR_SEPARATOR +
                               conf_id +
                               GlobalAttributes::OBJECT_DTD_EXT,
                               conf_id);

  XMLParser::loadXMLFile(GlobalAttributes::CONFIGURATIONS_DIR +
                                GlobalAttributes::DIR_SEPARATOR +
                                conf_id +
                                GlobalAttributes::CONFIGURATION_EXT);

  if(XMLParser::accessElement(XMLParser::CHILD_ELEMENT))
  {
   do
   {
    /* Certificando que só elementos xml serão lidos do parser,
      qualquer outro tipo de objeto xml será ignorado */
    if(XMLParser::getElementType()==XML_ELEMENT_NODE)
    {
     this->getConfigurationParams(key_attribs);

     if(XMLParser::hasElement(XMLParser::CHILD_ELEMENT))
     {
      XMLParser::savePosition();
      XMLParser::accessElement(XMLParser::CHILD_ELEMENT);

      do
      {
       this->getConfigurationParams(key_attribs);
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

void BaseConfigWidget::getConfigurationParams(const vector<QString> &key_attribs)
{
 map<QString, QString> aux_attribs;
 map<QString, QString>::iterator itr, itr_end;
 QString key;

 //Obtém os atributos do elemento atual
 XMLParser::getElementAttributes(aux_attribs);

 itr=aux_attribs.begin();
 itr_end=aux_attribs.end();

 //Substituindo a chave pelo atributo chave passado
 while(itr!=itr_end && key.isEmpty())
 {
  //Caso o atributo chave seja encontrado em um dos atributos obtidos do elemento
  if(key.isEmpty() && find(key_attribs.begin(), key_attribs.end(), itr->first)!=key_attribs.end())
   //A chave passa a ser o valor do atributo encontrado
   key=itr->second;
  itr++;
 }

 //Caso a chave não foi definida, a chave padrão será o nome do elemento
 if(key.isEmpty())
  key=XMLParser::getElementName();

 //Atribui os dados obtidos do elemento atual   chave do mapa de parâmetros de configuração
 if(!aux_attribs.empty())
  config_params[key]=aux_attribs;
}

