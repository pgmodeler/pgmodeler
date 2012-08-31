#include "confconexoeswidget.h"
#include "caixamensagem.h"

extern CaixaMensagem *caixa_msg;
//***********************************************************
ConfConexoesWidget::ConfConexoesWidget(QWidget * parent) : QWidget(parent)
{
 setupUi(this);

 connect(modo_ssl_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(habilitarCertificados(void)));

 connect(nova_tb, SIGNAL(clicked(bool)), this, SLOT(novaConexao(void)));
 connect(cancelar_tb, SIGNAL(clicked(bool)), this, SLOT(novaConexao(void)));

 connect(testar_tb, SIGNAL(clicked(bool)), this, SLOT(testarConexao(void)));
 connect(adicionar_tb, SIGNAL(clicked(bool)), this, SLOT(manipularConexao(void)));
 connect(atualizar_tb, SIGNAL(clicked(bool)), this, SLOT(manipularConexao(void)));
 connect(editar_tb, SIGNAL(clicked(bool)), this, SLOT(editarConexao(void)));
 connect(excluir_tb, SIGNAL(clicked(bool)), this, SLOT(removerConexao(void)));

 connect(alias_edt, SIGNAL(textChanged(QString)), this, SLOT(habilitarTesteConexao(void)));
 connect(host_edt, SIGNAL(textChanged(QString)), this, SLOT(habilitarTesteConexao(void)));
 connect(usuario_edt, SIGNAL(textChanged(QString)), this, SLOT(habilitarTesteConexao(void)));
 connect(senha_edt, SIGNAL(textChanged(QString)), this, SLOT(habilitarTesteConexao(void)));
 connect(bd_conexao_edt, SIGNAL(textChanged(QString)), this, SLOT(habilitarTesteConexao(void)));

 atualizar_tb->setVisible(false);
 cancelar_tb->setVisible(false);
}
//------------------------------------------------------------
ConfConexoesWidget::~ConfConexoesWidget(void)
{
 /* Remove todas as conexões carregadas ao destruir o formulário,
    (desalocando os descritores de conexão criados) */
 while(conexoes_cmb->count() > 0)
  this->removerConexao();
}
//------------------------------------------------------------
void ConfConexoesWidget::carregarConfiguracao(void)
{
 vector<QString> atribs_chave;
 map<QString, map<QString, QString> >::iterator itr, itr_end;
 ConexaoBD *conexao=NULL;

 //Expressão regular usada para validar o endereço digitado para o servidor
 QRegExp ip_regexp("[0-9]+\\.[0-9]+\\.[0-9]+\\.[0-9]+");

 //Para a sessão de configuração de conexões, o 'alias' será usado como campo chave
 atribs_chave.push_back(AtributosParsers::ALIAS);

 //Carrega as configurações
 ConfBaseWidget::carregarConfiguracao(AtributosGlobais::CONF_CONEXOES, atribs_chave);

 itr=params_config.begin();
 itr_end=params_config.end();

 while(itr!=itr_end)
 {
  //Aloca uma nova conexão
  conexao=new ConexaoBD;

  /* Caso o host da conexão seja um IP, atribui o valor ao parâmetro,
     ConexaoBD::PARAM_IP_SERVIDOR da conexão. Caso contrário atribui ao
     ConexaoBD::PARAM_FQDN_SERVIDOR. A libpq trata separadamente IP e FQDN do servidor
     por isso é necessária esta distinção */
  if(ip_regexp.exactMatch(itr->second[ConexaoBD::PARAM_FQDN_SERVIDOR]))
   conexao->definirParamConexao(ConexaoBD::PARAM_IP_SERVIDOR, itr->second[ConexaoBD::PARAM_FQDN_SERVIDOR]);
  else
   conexao->definirParamConexao(ConexaoBD::PARAM_FQDN_SERVIDOR, itr->second[ConexaoBD::PARAM_FQDN_SERVIDOR]);

  //Atribuindo os demais valores à conexão
  conexao->definirParamConexao(ConexaoBD::PARAM_PORTA, itr->second[ConexaoBD::PARAM_PORTA]);
  conexao->definirParamConexao(ConexaoBD::PARAM_USUARIO, itr->second[ConexaoBD::PARAM_USUARIO]);
  conexao->definirParamConexao(ConexaoBD::PARAM_SENHA,itr->second[ConexaoBD::PARAM_SENHA]);
  conexao->definirParamConexao(ConexaoBD::PARAM_NOME_BD, itr->second[ConexaoBD::PARAM_NOME_BD]);
  conexao->definirParamConexao(ConexaoBD::PARAM_TEMPO_CONEXAO, itr->second[ConexaoBD::PARAM_TEMPO_CONEXAO]);
  conexao->definirParamConexao(ConexaoBD::PARAM_MODO_SSL, itr->second[ConexaoBD::PARAM_MODO_SSL]);
  conexao->definirParamConexao(ConexaoBD::PARAM_CERT_RAIZ_SSL, itr->second[ConexaoBD::PARAM_CERT_RAIZ_SSL]);
  conexao->definirParamConexao(ConexaoBD::PARAM_CERT_SSL, itr->second[ConexaoBD::PARAM_CERT_SSL]);
  conexao->definirParamConexao(ConexaoBD::PARAM_CHAVE_SSL, itr->second[ConexaoBD::PARAM_CHAVE_SSL]);
  conexao->definirParamConexao(ConexaoBD::PARAM_CRL_SSL, itr->second[ConexaoBD::PARAM_CRL_SSL]);
  conexao->definirParamConexao(ConexaoBD::PARAM_LIB_GSSAPI, itr->second[ConexaoBD::PARAM_LIB_GSSAPI]);
  conexao->definirParamConexao(ConexaoBD::PARAM_SERVIDOR_KERBEROS, itr->second[ConexaoBD::PARAM_SERVIDOR_KERBEROS]);
  conexao->definirParamConexao(ConexaoBD::PARAM_OPCOES, itr->second[ConexaoBD::PARAM_OPCOES]);

  //Adiciona a conexão ao combo de conexões
  conexoes_cmb->addItem(QString::fromUtf8(itr->second[AtributosParsers::ALIAS]),
                        QVariant::fromValue<void *>(reinterpret_cast<void *>(conexao)));

  itr++;
 }

 /* Habilita os botões de edição e exclusão caso, após o carregamento,
    hajam conexões inseridas no combo */
 editar_tb->setEnabled(conexoes_cmb->count() > 0);
 excluir_tb->setEnabled(conexoes_cmb->count() > 0);
}
//------------------------------------------------------------
void ConfConexoesWidget::habilitarCertificados(void)
{
 /* Habilita os campos relacionados aos certificados SSL quando o modo
    SSL escolhido é diferente de 'disable ' */
 cert_cliente_lbl->setEnabled(modo_ssl_cmb->currentIndex()!=0);
 cert_cliente_edt->setEnabled(modo_ssl_cmb->currentIndex()!=0);
 cert_raiz_lbl->setEnabled(modo_ssl_cmb->currentIndex()!=0);
 cert_raiz_edt->setEnabled(modo_ssl_cmb->currentIndex()!=0);
 crl_edt->setEnabled(modo_ssl_cmb->currentIndex()!=0);
 crl_lbl->setEnabled(modo_ssl_cmb->currentIndex()!=0);
 chave_cli_lbl->setEnabled(modo_ssl_cmb->currentIndex()!=0);
 chave_cli_edt->setEnabled(modo_ssl_cmb->currentIndex()!=0);
}
//------------------------------------------------------------
void ConfConexoesWidget::habilitarTesteConexao(void)
{
 /* O teste de conexão só é habilitado quando informações básicas
    como alias, host, usuario, senha e db de conexão estão especificados */
 testar_tb->setEnabled(!alias_edt->text().isEmpty() &&
                       !host_edt->text().isEmpty() &&
                       !usuario_edt->text().isEmpty() &&
                       !senha_edt->text().isEmpty() &&
                       !bd_conexao_edt->text().isEmpty());
 adicionar_tb->setEnabled(testar_tb->isEnabled());
 atualizar_tb->setEnabled(testar_tb->isEnabled());
}
//------------------------------------------------------------
void ConfConexoesWidget::novaConexao(void)
{
 /* Limpa todo o formulário e reverte os valores de alguns
    campos para seus valores padrão */
 bd_conexao_edt->clear();
 alias_edt->clear();
 usuario_edt->clear();
 host_edt->clear();
 porta_sbp->setValue(5432);
 senha_edt->clear();
 opcoes_edt->clear();

 modo_ssl_cmb->setCurrentIndex(0);
 cert_cliente_edt->setText("~/.postgresql/postgresql.crt");
 cert_raiz_edt->setText("~/.postgresql/root.crt");
 crl_edt->setText("~/.postgresql/root.crl");
 chave_cli_edt->setText("~/.postgresql/postgresql.key");

 aut_gssapi_chk->setChecked(false);
 serv_kerberus_edt->clear();

 timeout_sbp->setValue(2);
 adicionar_tb->setVisible(true);
 atualizar_tb->setVisible(false);
 conexoes_cmb->setEnabled(true);

 nova_tb->setVisible(true);
 cancelar_tb->setVisible(false);

 editar_tb->setEnabled(conexoes_cmb->count() > 0);
 excluir_tb->setEnabled(conexoes_cmb->count() > 0);
}
//------------------------------------------------------------
void ConfConexoesWidget::manipularConexao(void)
{
 ConexaoBD *conexao=NULL;
 QString alias;
 unsigned i=1;

 try
 {
  /* Verificando duplicidade nos alias das conexões antes de manipular.
     Caso alguma seja detectada, a conexão terá um sufixo '([0-9]+)'
     adicionado */
  alias=alias_edt->text();
  while(conexoes_cmb->findText(alias)>=0)
   alias=QString("%1(%2)").arg(alias_edt->text()).arg(i++);

  if(!atualizar_tb->isVisible())
  {
   conexao=new ConexaoBD;
   this->configurarConexao(conexao);
   conexoes_cmb->addItem(alias, QVariant::fromValue<void *>(reinterpret_cast<void *>(conexao)));
  }
  else
  {
   conexao=reinterpret_cast<ConexaoBD *>(conexoes_cmb->itemData(conexoes_cmb->currentIndex()).value<void *>());
   this->configurarConexao(conexao);
   conexoes_cmb->setItemText(conexoes_cmb->currentIndex(), alias);
  }

  this->novaConexao();
  editar_tb->setEnabled(conexoes_cmb->count() > 0);
  excluir_tb->setEnabled(conexoes_cmb->count() > 0);
 }
 catch(Excecao &e)
 {
  if(adicionar_tb->isVisible())
   delete(conexao);

  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}
//------------------------------------------------------------
void ConfConexoesWidget::removerConexao(void)
{
 //Caso haja uma conexão selecionada no combo
 if(conexoes_cmb->currentIndex() >= 0)
 {
  ConexaoBD *conexao=NULL;

  //Obtém a conexão do item do combobox
  conexao=reinterpret_cast<ConexaoBD *>(conexoes_cmb->itemData(conexoes_cmb->currentIndex()).value<void *>());

  //Remove o item do combo
  conexoes_cmb->removeItem(conexoes_cmb->currentIndex());

  //Desaloca o descritor de conexão
  delete(conexao);

  //Reinicia o formulário
  this->novaConexao();
 }
}
//------------------------------------------------------------
void ConfConexoesWidget::editarConexao(void)
{
 //Caso hajam itens no combo de conexões
 if(conexoes_cmb->count() > 0)
 {
  ConexaoBD *conexao=NULL;

  //Obtém a conexão selecionada
  conexao=reinterpret_cast<ConexaoBD *>(conexoes_cmb->itemData(conexoes_cmb->currentIndex()).value<void *>());

  //Preenche os campos do formulário com os valores dos atributos da conexão
  alias_edt->setText(conexoes_cmb->currentText());

  if(!conexao->obterParamConexao(ConexaoBD::PARAM_FQDN_SERVIDOR).isEmpty())
   host_edt->setText(conexao->obterParamConexao(ConexaoBD::PARAM_FQDN_SERVIDOR));
  else
   host_edt->setText(conexao->obterParamConexao(ConexaoBD::PARAM_IP_SERVIDOR));

  bd_conexao_edt->setText(conexao->obterParamConexao(ConexaoBD::PARAM_NOME_BD));
  usuario_edt->setText(conexao->obterParamConexao(ConexaoBD::PARAM_USUARIO));
  senha_edt->setText(conexao->obterParamConexao(ConexaoBD::PARAM_SENHA));
  porta_sbp->setValue(conexao->obterParamConexao(ConexaoBD::PARAM_PORTA).toInt());
  timeout_sbp->setValue(conexao->obterParamConexao(ConexaoBD::PARAM_TEMPO_CONEXAO).toInt());

  serv_kerberus_edt->setText(conexao->obterParamConexao(ConexaoBD::PARAM_SERVIDOR_KERBEROS));
  aut_gssapi_chk->setChecked(conexao->obterParamConexao(ConexaoBD::PARAM_LIB_GSSAPI)=="gssapi");
  opcoes_edt->setText(conexao->obterParamConexao(ConexaoBD::PARAM_OPCOES));

  //Seta o modo de conexão SSL
  if(conexao->obterParamConexao(ConexaoBD::PARAM_MODO_SSL)==ConexaoBD::SSL_DESATIVADO)
   modo_ssl_cmb->setCurrentIndex(0);
  else if(conexao->obterParamConexao(ConexaoBD::PARAM_MODO_SSL)==ConexaoBD::SSL_PERMITIR)
   modo_ssl_cmb->setCurrentIndex(1);
  else if(conexao->obterParamConexao(ConexaoBD::PARAM_MODO_SSL)==ConexaoBD::SSL_REQUERER)
   modo_ssl_cmb->setCurrentIndex(2);
  else if(conexao->obterParamConexao(ConexaoBD::PARAM_MODO_SSL)==ConexaoBD::SSL_VERIF_AUT_CERT)
   modo_ssl_cmb->setCurrentIndex(3);
  else
   modo_ssl_cmb->setCurrentIndex(4);

  //Caso haja um modo SSL escolhido, preenche os campos de certificados digitais
  if(modo_ssl_cmb->currentIndex() > 0)
  {
   cert_cliente_edt->setText(conexao->obterParamConexao(ConexaoBD::PARAM_CERT_SSL));
   cert_raiz_edt->setText(conexao->obterParamConexao(ConexaoBD::PARAM_CERT_RAIZ_SSL));
   chave_cli_edt->setText(conexao->obterParamConexao(ConexaoBD::PARAM_CHAVE_SSL));
   crl_edt->setText(conexao->obterParamConexao(ConexaoBD::PARAM_CRL_SSL));
  }

  atualizar_tb->setVisible(true);
  adicionar_tb->setVisible(false);
  conexoes_cmb->setEnabled(false);

  nova_tb->setVisible(false);
  cancelar_tb->setVisible(true);
  editar_tb->setEnabled(false);
 }
}
//------------------------------------------------------------
void ConfConexoesWidget::configurarConexao(ConexaoBD *conexao)
{
 if(conexao)
 {
  QRegExp ip_regexp("[0-9]+\\.[0-9]+\\.[0-9]+\\.[0-9]+");

  /* Caso o nome do host especificado no formulário seja um IP (combinando com a expressão regular)
     atribui o valor ao campo respectivo na conexão. Caso contrário atribui ao campo de
     nome de host (fqdn) */
  if(ip_regexp.exactMatch(host_edt->text()))
   conexao->definirParamConexao(ConexaoBD::PARAM_IP_SERVIDOR, host_edt->text());
  else
   conexao->definirParamConexao(ConexaoBD::PARAM_FQDN_SERVIDOR, host_edt->text());

  //Atribuindo os valores básicos de conexão
  conexao->definirParamConexao(ConexaoBD::PARAM_PORTA, QString("%1").arg(porta_sbp->value()));
  conexao->definirParamConexao(ConexaoBD::PARAM_USUARIO, usuario_edt->text());
  conexao->definirParamConexao(ConexaoBD::PARAM_SENHA, senha_edt->text());
  conexao->definirParamConexao(ConexaoBD::PARAM_NOME_BD, bd_conexao_edt->text());
  conexao->definirParamConexao(ConexaoBD::PARAM_TEMPO_CONEXAO, QString("%1").arg(timeout_sbp->value()));

  //Configurando o modo SSL da conexão
  switch(modo_ssl_cmb->currentIndex())
  {
   case 1:
    conexao->definirParamConexao(ConexaoBD::PARAM_MODO_SSL, ConexaoBD::SSL_PERMITIR);
   break;
   case 2:
    conexao->definirParamConexao(ConexaoBD::PARAM_MODO_SSL, ConexaoBD::SSL_REQUERER);
   break;
   case 3:
    conexao->definirParamConexao(ConexaoBD::PARAM_MODO_SSL, ConexaoBD::SSL_VERIF_AUT_CERT);
   break;
   case 4:
    conexao->definirParamConexao(ConexaoBD::PARAM_MODO_SSL, ConexaoBD::SSL_VERIF_COMPLETA);
   break;
   default:
   case 0:
    conexao->definirParamConexao(ConexaoBD::PARAM_MODO_SSL, ConexaoBD::SSL_DESATIVADO);
   break;
  }

  //Caso haja um modo SSL selecionado, atribui os certificados usados para conexão
  if(modo_ssl_cmb->currentIndex()!=0)
  {
   conexao->definirParamConexao(ConexaoBD::PARAM_CERT_RAIZ_SSL, cert_raiz_edt->text());
   conexao->definirParamConexao(ConexaoBD::PARAM_CERT_SSL, cert_cliente_edt->text());
   conexao->definirParamConexao(ConexaoBD::PARAM_CHAVE_SSL, chave_cli_edt->text());
   conexao->definirParamConexao(ConexaoBD::PARAM_CRL_SSL, crl_edt->text());
  }

  //Especificando autenticação GSSAPI caso esteja marcada no formulário
  if(aut_gssapi_chk->isChecked())
   conexao->definirParamConexao(ConexaoBD::PARAM_LIB_GSSAPI, "gssapi");

  //Especificando o endereço do servidor Kerberus quando especificado
  if(!serv_kerberus_edt->text().isEmpty())
   conexao->definirParamConexao(ConexaoBD::PARAM_SERVIDOR_KERBEROS, serv_kerberus_edt->text());

  //Atribuindo opções adicionas de conexão
  if(!opcoes_edt->text().isEmpty())
   conexao->definirParamConexao(ConexaoBD::PARAM_OPCOES, opcoes_edt->text());
 }
}
//------------------------------------------------------------
void ConfConexoesWidget::testarConexao(void)
{
 ConexaoBD conexao;

 try
 {
  //Configura uma conexão com os valores do formulário
  this->configurarConexao(&conexao);

  /* Tenta a conexão. Se neste ponto nenhuma exceção for disparada
     considera-se que a conexão foi realizada com sucesso */
  conexao.conectar();

  //Exibe a mensagem de sucesso
  caixa_msg->show(trUtf8("Sucesso"), trUtf8("Conexão efetuada com sucesso!"), CaixaMensagem::ICONE_INFO);
 }
 catch(Excecao &e)
 {
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}
//-----------------------------------------------------------
void ConfConexoesWidget::restaurarPadroes(void)
{
 try
 {
  //Restaura as configurações padrão e recarrega o arquivo restaurado
  ConfBaseWidget::restaurarPadroes(AtributosGlobais::CONF_CONEXOES);

  //Remove as conexões atuais
  while(conexoes_cmb->count() > 0)
   this->removerConexao();

  //Recarrega a configuração de conexões
  this->carregarConfiguracao();
 }
 catch(Excecao &e)
 {
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}
//-----------------------------------------------------------
void ConfConexoesWidget::salvarConfiguracao(void)
{
 try
 {
  int i, qtd;
  ConexaoBD *conexao=NULL;
  map<QString, QString> atribs;

  params_config[AtributosGlobais::CONF_CONEXOES].clear();
  qtd=conexoes_cmb->count();

  /* Workaround: Quando não existem conexões, para se gravar um arquivo vazio, é necessário
     preencher o atributo 'params_config[AtributosGlobais::CONF_CONEXOES][AtributosParsers::CONEXOES]'
     espaços */
  if(qtd==0)
   params_config[AtributosGlobais::CONF_CONEXOES][AtributosParsers::CONEXOES]="  ";
  else
  {
   /* Quando se tem conexões no combo, os atributos de cada uma são obtidos e é
      gerado um esquema com base nestes valores */
   for(i=0; i < qtd; i++)
   {
    //Obtém uma conexão e seus atributos
    conexao=reinterpret_cast<ConexaoBD *>(conexoes_cmb->itemData(i).value<void *>());
    atribs=conexao->obterParamsConexao();

    /* Caso na conexão não esteja especificado o host significa que o endereço usado é o IP,
       sendo assim, o IP é atribuido ao parâmetro FQDN, desta forma ao gerar o esquema
       da conexão, o parser conseguirá identificar o parâmetro 'host' */
    if(atribs.count(ConexaoBD::PARAM_FQDN_SERVIDOR)==0)
     atribs[ConexaoBD::PARAM_FQDN_SERVIDOR]=atribs[ConexaoBD::PARAM_IP_SERVIDOR];

    /* Armazena também nos atributos o alias da conexão pois este precisa ser gravado
       no arquivo de configuração */
    atribs[AtributosParsers::ALIAS]=conexoes_cmb->itemText(i);

    /* Ativa o modo de ignorar atributos desconhecidos do parser, pois, alguns
       atributos da conexão nem sempre são especificados (opcionais) e isso
       geraria muitos erros se o modo citado não estivesse ativado */
    ParserEsquema::ignorarAtributosDesc(true);

    //Gera o esquema da conexão e contatena à demais geradas
    params_config[AtributosGlobais::CONF_CONEXOES][AtributosParsers::CONEXOES]+=
    ParserEsquema::obterDefinicaoObjeto(AtributosGlobais::DIR_CONFIGURACOES +
                                        AtributosGlobais::SEP_DIRETORIO +
                                        AtributosGlobais::DIR_ESQUEMAS +
                                        AtributosGlobais::SEP_DIRETORIO +
                                        AtributosGlobais::CONF_CONEXOES +
                                        AtributosGlobais::EXT_ESQUEMA,
                                        atribs);
    ParserEsquema::ignorarAtributosDesc(false);
   }
  }

  //Gera o arquivo de configuração completo
  ConfBaseWidget::salvarConfiguracao(AtributosGlobais::CONF_CONEXOES);
 }
 catch(Excecao &e)
 {
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}
//-----------------------------------------------------------
void ConfConexoesWidget::obterConexoes(map<QString, ConexaoBD *> &conexoes)
{
 int i, qtd;

 conexoes.clear();
 qtd=conexoes_cmb->count();

 for(i=0; i < qtd; i++)
  conexoes[conexoes_cmb->itemText(i)]=reinterpret_cast<ConexaoBD *>(conexoes_cmb->itemData(i).value<void *>());
}
//***********************************************************

