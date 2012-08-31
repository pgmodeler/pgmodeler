#include "dominiowidget.h"
//***********************************************************
DominioWidget::DominioWidget(QWidget *parent): ObjetoBaseWidget(parent, OBJETO_DOMINIO)
{
 try
 {
  Ui_DominioWidget::setupUi(this);

  //Cria um destacador de sintaxe no campo de expressão de checagem do domínio
  destaque_expr=NULL;
  destaque_expr=new DestaqueSintaxe(expr_checagem_txt, false);
  destaque_expr->carregarConfiguracao(AtributosGlobais::DIR_CONFIGURACOES +
                                     AtributosGlobais::SEP_DIRETORIO +
                                     AtributosGlobais::CONF_DESTAQUE_SQL +
                                     AtributosGlobais::EXT_CONFIGURACAO);

  tipo_dominio=NULL;
  tipo_dominio=new TipoPgSQLWidget(this);
  dominio_grid->addWidget(tipo_dominio,4,0,1,2);

  configurarLayouFormulario(dominio_grid, OBJETO_DOMINIO);
  connect(janela_pai->aplicar_ok_btn,SIGNAL(clicked(bool)), this, SLOT(aplicarConfiguracao(void)));

  janela_pai->setMinimumSize(530, 450);
  janela_pai->setMaximumHeight(450);
 }
 catch(Excecao &e)
 {
  //Redireciona o erro
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}
//----------------------------------------------------------
void DominioWidget::hideEvent(QHideEvent *evento)
{
 valor_padrao_edt->clear();
 expr_checagem_txt->clear();
 nome_rest_edt->clear();
 nao_nulo_chk->setChecked(false);
 ObjetoBaseWidget::hideEvent(evento);
}
//----------------------------------------------------------
void DominioWidget::definirAtributos(ModeloBD *modelo, ListaOperacoes *lista_op, Dominio *dominio)
{
 TipoPgSQL tipo;

 //Define os atributos do formulários e da janela pai
 ObjetoBaseWidget::definirAtributos(modelo, lista_op, dominio);

 //Caso o domínio passado esteja alocado
 if(dominio)
 {
  //Preenche o formulário com os atributos do domínio
  tipo=dominio->obterTipo();
  valor_padrao_edt->setText(QString::fromUtf8(dominio->obterValorPadrao()));
  expr_checagem_txt->setPlainText(QString::fromUtf8(dominio->obterExpressao()));
  nome_rest_edt->setText(QString::fromUtf8(dominio->obterNomeRestricao()));
  nao_nulo_chk->setChecked(dominio->naoNulo());
 }

 //Marca o tipo do domínio no widget de tipos pgsql
 tipo_dominio->definirAtributos(tipo, modelo);
}
//----------------------------------------------------------
void DominioWidget::aplicarConfiguracao(void)
{
 try
 {
  Dominio *dominio=NULL;
  iniciarConfiguracao<Dominio>();

  //Obtém a referência ao domínio que está sendo editado/criado
  dominio=dynamic_cast<Dominio *>(this->objeto);

  //Configura os atributos do mesmo com os valores definidos no formulário
  dominio->definirTipo(tipo_dominio->obterTipoPgSQL());
  dominio->definirValorPadrao(valor_padrao_edt->text());
  dominio->definirExpressao(expr_checagem_txt->toPlainText());
  dominio->definirNomeRestricao(nome_rest_edt->text());
  dominio->definirNaoNulo(nao_nulo_chk->isChecked());

  ObjetoBaseWidget::aplicarConfiguracao();
  finalizarConfiguracao();
 }
 catch(Excecao &e)
 {
  /* Cancela a configuração o objeto removendo a ultima operação adicionada
     referente ao objeto editado/criado e desaloca o objeto
     caso o mesmo seja novo */
  cancelarConfiguracao();
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}
//***********************************************************
