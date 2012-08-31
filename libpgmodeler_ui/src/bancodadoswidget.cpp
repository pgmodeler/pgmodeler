#include "bancodadoswidget.h"
//***********************************************************
BancoDadosWidget::BancoDadosWidget(QWidget *parent): ObjetoBaseWidget(parent, OBJETO_BANCO_DADOS)
{
 map<QString, vector<QWidget *> > mapa_campos;
 QFrame *frame=NULL;
 QLocale loc;
 QStringList lista_loc, codificacoes;
 unsigned i,i1;

 Ui_BancoDadosWidget::setupUi(this);

 connect(janela_pai->aplicar_ok_btn,SIGNAL(clicked(bool)), this, SLOT(aplicarConfiguracao(void)));
 configurarLayouFormulario(bancodados_grid, OBJETO_BANCO_DADOS);

 //Define os campos exclusivos para cada versão
 mapa_campos[gerarIntervaloVersoes(APOS_VERSAO, ParserEsquema::VERSAO_PGSQL_83)].push_back(limconexao_lbl);
 mapa_campos[gerarIntervaloVersoes(APOS_VERSAO, ParserEsquema::VERSAO_PGSQL_84)].push_back(lccollate_lbl);
 mapa_campos[gerarIntervaloVersoes(APOS_VERSAO, ParserEsquema::VERSAO_PGSQL_84)].push_back(lcctype_lbl);

 //Gera o frame de alerta
 frame=gerarFrameAlertaVersao(mapa_campos);
 bancodados_grid->addWidget(frame, bancodados_grid->count()+1, 0, 1, 0);
 frame->setParent(this);

 //Define as alturas mínimas e máxima do formulário
 janela_pai->setMinimumWidth(530);
 janela_pai->setMinimumHeight(420);
 janela_pai->setMaximumHeight(420);

 //Obtém os nomes das codificações e as insere no combo de codificação
 TipoCodificacao::obterTipos(codificacoes);
 codificacao_cmb->addItems(codificacoes);

 //Obtém todas as localizações padrão e as armazena em uma lista de QString
 for(i=QLocale::C; i <= QLocale::Chewa; i++)
 {
  for(i1=QLocale::Afghanistan; i1 <= QLocale::Zimbabwe; i1++)
  {
   loc=QLocale(static_cast<QLocale::Language>(i),static_cast<QLocale::Country>(i1));
   lista_loc.append(loc.name());
  }
 }

 //Remove as localizações duplicadas
 lista_loc.removeDuplicates();
 //Ordena as localizações
 lista_loc.sort();

 //Insere as localizações nos combos
 lccollate_cmb->addItem(trUtf8("Padrão"));
 lcctype_cmb->addItem(trUtf8("Padrão"));
 lccollate_cmb->addItems(lista_loc);
 lcctype_cmb->addItems(lista_loc);
}
//----------------------------------------------------------
void BancoDadosWidget::definirAtributos(ModeloBD *modelo)
{
 if(modelo)
 {
  int idx;

  //Configura os atributos de limite de conexão, banco modelo e autor
  limconexao_sb->setValue(modelo->obterLimiteConexao());
  bdmodelo_edt->setText(QString::fromUtf8(modelo->obterBDModelo()));
  autor_edt->setText(QString::fromUtf8(modelo->obterAutor()));

  //Configura o combo de codificação com a codificação atual
  idx=codificacao_cmb->findText(~modelo->obterCodificacao());
  if(idx < 0) idx=0;
  codificacao_cmb->setCurrentIndex(idx);

  //Configura as localizações LC_COLLATE E LC_CTYPE de acordo com a conf. atual
  idx=lccollate_cmb->findText(modelo->obterLocalizacao(LC_COLLATE));
  if(idx < 0) idx=0;
  lccollate_cmb->setCurrentIndex(idx);

  idx=lcctype_cmb->findText(modelo->obterLocalizacao(LC_CTYPE));
  if(idx < 0) idx=0;
  lcctype_cmb->setCurrentIndex(idx);

  //Define os atributos do formulários e da janela pai
  ObjetoBaseWidget::definirAtributos(modelo, NULL, modelo);
 }
}
//----------------------------------------------------------
void BancoDadosWidget::aplicarConfiguracao(void)
{
 try
 {
  //Aplica as configurações básicas do objeto
  ObjetoBaseWidget::aplicarConfiguracao();

  //Define o autor do modelo
  modelo->definirAutor(autor_edt->text().toUtf8());

  /* Define a condificação do modelo de acordo com a selecionada no formulário
     caso a codifição 'Padrão' seja selecionada o modelo usará a codificação padrão
     do SGBD em que for executado o script sql gerado */
  modelo->definirCodificacao(TipoCodificacao(codificacao_cmb->currentText()));

  /* Define as localização LC_COLLATE e LC_CTYPE do modelo de acordo com a selecionada
     no formulário caso a localização 'Padrão' seja selecionada o modelo usará a localização
     padrão do SGBD em que for executado o script sql gerado */
  if(lccollate_cmb->currentIndex() > 0)
   modelo->definirLocalizacao(LC_COLLATE, lccollate_cmb->currentText());
  else
   modelo->definirLocalizacao(LC_COLLATE, "");

  if(lcctype_cmb->currentIndex() > 0)
   modelo->definirLocalizacao(LC_CTYPE, lcctype_cmb->currentText());
  else
   modelo->definirLocalizacao(LC_CTYPE, "");

  finalizarConfiguracao();
 }
 catch(Excecao &e)
 {
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}
//***********************************************************
