#include "confbasewidget.h"
//***********************************************************
void ConfBaseWidget::adicionarParamConfiguracao(const QString &param, const map<QString, QString> &atributos)
{
 //Adiciona um parâmetro somente se o identificador 'param' e os atributos não estejam vazio
 if(!param.isEmpty() && !atributos.empty())
  params_config[param]=atributos;
}
//-----------------------------------------------------------
map<QString, map<QString, QString> > ConfBaseWidget::obterParamsConfiguracao(void)
{
 return(params_config);
}
//-----------------------------------------------------------
void ConfBaseWidget::excluirParamConfiguracao(const QString &param)
{
 params_config.erase(param);
}
//-----------------------------------------------------------
void ConfBaseWidget::excluirParamsConfiguracao(void)
{
 params_config.clear();
}
//-----------------------------------------------------------
void ConfBaseWidget::salvarConfiguracao(const QString &id_conf)
{
 QString buf,
         //Configura o nome do arquivo de modelo (esquema) de configuração
         nome_arq_sch=AtributosGlobais::DIR_CONFIGURACOES +
                      AtributosGlobais::SEP_DIRETORIO +
                      AtributosGlobais::DIR_ESQUEMAS +
                      AtributosGlobais::SEP_DIRETORIO +
                      id_conf +
                      AtributosGlobais::EXT_ESQUEMA,
         //Configura o nome do arquivo de configuração
         nome_arq=AtributosGlobais::DIR_CONFIGURACOES +
                  AtributosGlobais::SEP_DIRETORIO +
                  id_conf +
                  AtributosGlobais::EXT_CONFIGURACAO;
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
  buf=ParserEsquema::obterDefinicaoObjeto(nome_arq_sch, atribs);

  //Abre o arquivo de configuração para gravação
  saida.open(QFile::WriteOnly);

  //Caso não consiga abrir o arquivo para gravação
  if(!saida.isOpen())
   throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELER_ARQNAOGRAVADO).arg(nome_arq),
                 ERR_PGMODELER_ARQNAOGRAVADO,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  //Grava o buffer gerado no arquivo
  saida.write(buf.toStdString().c_str(), buf.size());
  saida.close();
 }
 catch(Excecao &e)
 {
  if(saida.isOpen()) saida.close();
  throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELER_ARQNAOGRAVADODEFINV).arg(nome_arq),
                ERR_PGMODELER_ARQNAOGRAVADODEFINV,__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}
//-----------------------------------------------------------
void ConfBaseWidget::restaurarPadroes(const QString &id_conf)
{
 QString arq_atual, arq_orig;

 //Monta o caminho para o arquivo de configuração atual (conf/arquivo.conf)
 arq_atual=AtributosGlobais::DIR_CONFIGURACOES +
           AtributosGlobais::SEP_DIRETORIO +
           id_conf +
           AtributosGlobais::EXT_CONFIGURACAO;

 //Monta o caminho para o arquivo de configuração original (padrão) (conf/defaults/arquivo.conf)
 arq_orig=AtributosGlobais::DIR_CONFIGURACOES +
          AtributosGlobais::SEP_DIRETORIO +
          AtributosGlobais::DIR_CONF_PADRAO+
          AtributosGlobais::SEP_DIRETORIO +
          id_conf +
          AtributosGlobais::EXT_CONFIGURACAO;

 //Verifica a existência do arquivo padrão, caso não existe emite uma exceção e a restauração é abortada
 if(!QFile::exists(arq_orig))
  throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELERUI_CONFPADRAONAORESTAURADA).arg(arq_orig),
                ERR_PGMODELERUI_CONFPADRAONAORESTAURADA,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 else
 {
  //Remove o arquivo atual
  QFile::remove(arq_atual);
  //Copia o arquivo original no lugar do arquivo atual
  QFile::copy(arq_orig, arq_atual);
 }
}
//-----------------------------------------------------------
void ConfBaseWidget::carregarConfiguracao(const QString &id_conf, const vector<QString> &atribs_chave)
{
 try
 {
  //Limpa os parâmetros de configuração atuais
  params_config.clear();

  //Reinicia o parser XML para a leitura do arquivo
  ParserXML::reiniciarParser();

  ParserXML::definirArquivoDTD(AtributosGlobais::DIR_CONFIGURACOES +
                               AtributosGlobais::SEP_DIRETORIO +
                               AtributosGlobais::DIR_DTD_OBJETO +
                               AtributosGlobais::SEP_DIRETORIO +
                               id_conf +
                               AtributosGlobais::EXT_DTD_OBJETO,
                               id_conf);

  ParserXML::carregarArquivoXML(AtributosGlobais::DIR_CONFIGURACOES +
                                AtributosGlobais::SEP_DIRETORIO +
                                id_conf +
                                AtributosGlobais::EXT_CONFIGURACAO);

  if(ParserXML::acessarElemento(ParserXML::ELEMENTO_FILHO))
  {
   do
   {
    /* Certificando que só elementos xml serão lidos do parser,
      qualquer outro tipo de objeto xml será ignorado */
    if(ParserXML::obterTipoElemento()==XML_ELEMENT_NODE)
    {
     this->obterParamsConfiguracao(atribs_chave);

     if(ParserXML::possuiElemento(ParserXML::ELEMENTO_FILHO))
     {
      ParserXML::salvarPosicao();
      ParserXML::acessarElemento(ParserXML::ELEMENTO_FILHO);

      do
      {
       this->obterParamsConfiguracao(atribs_chave);
      }
      while(ParserXML::acessarElemento(ParserXML::ELEMENTO_POSTERIOR));

      ParserXML::restaurarPosicao();
     }
    }
   }
   while(ParserXML::acessarElemento(ParserXML::ELEMENTO_POSTERIOR));
  }
 }
 catch(Excecao &e)
 {
  //Redireciona qualquer exceção capturada
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}
//-----------------------------------------------------------
void ConfBaseWidget::obterParamsConfiguracao(const vector<QString> &atribs_chave)
{
 map<QString, QString> atrib_aux;
 map<QString, QString>::iterator itr, itr_end;
 QString chave;

 //Obtém os atributos do elemento atual
 ParserXML::obterAtributosElemento(atrib_aux);

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
  chave=ParserXML::obterNomeElemento();

 //Atribui os dados obtidos do elemento atual à chave do mapa de parâmetros de configuração
 if(!atrib_aux.empty())
  params_config[chave]=atrib_aux;
}
//***********************************************************
