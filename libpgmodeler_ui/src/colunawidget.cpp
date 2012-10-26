#include "colunawidget.h"

ColunaWidget::ColunaWidget(QWidget *parent): ObjetoBaseWidget(parent, OBJETO_COLUNA)
{
 try
 {
  Ui_ColunaWidget::setupUi(this);

  //Cria um destacador de sintaxe no campo de expressão de checagem do domínio
  dest_valor_padrao=NULL;
  dest_valor_padrao=new DestaqueSintaxe(valor_padrao_txt, false);
  dest_valor_padrao->carregarConfiguracao(AtributosGlobais::DIR_CONFIGURACOES +
                                      AtributosGlobais::SEP_DIRETORIO +
                                      AtributosGlobais::CONF_DESTAQUE_SQL +
                                      AtributosGlobais::EXT_CONFIGURACAO);

  tipo_col=NULL;
  tipo_col=new TipoPgSQLWidget(this);
  coluna_grid->addWidget(tipo_col,3,0,1,0);

  configurarLayouFormulario(coluna_grid, OBJETO_COLUNA);
  connect(janela_pai->aplicar_ok_btn,SIGNAL(clicked(bool)), this, SLOT(aplicarConfiguracao(void)));

  janela_pai->setMinimumSize(530, 380);
  janela_pai->setMaximumHeight(380);
 }
 catch(Excecao &e)
 {
  //Redireciona o erro
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}
//----------------------------------------------------------
void ColunaWidget::hideEvent(QHideEvent *evento)
{
 valor_padrao_txt->clear();
 nao_nulo_chk->setChecked(false);
 ObjetoBaseWidget::hideEvent(evento);
}
//----------------------------------------------------------
void ColunaWidget::definirAtributos(ModeloBD *modelo, ObjetoBase *objeto_pai, ListaOperacoes *lista_op, Coluna *coluna)
{
 TipoPgSQL tipo;

 if(!objeto_pai)
  throw Excecao(ERR_PGMODELER_ATROBJNAOALOC,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Define os atributos do formulários e da janela pai
 ObjetoBaseWidget::definirAtributos(modelo, lista_op, coluna, objeto_pai);

 if(coluna)
 {
  tipo=coluna->obterTipo();
  nao_nulo_chk->setChecked(coluna->naoNulo());
  valor_padrao_txt->setPlainText(QString::fromUtf8(coluna->obterValorPadrao()));
 }

 //Marca o tipo do domínio no widget de tipos pgsql
 tipo_col->definirAtributos(tipo, modelo, ConfigTipoUsuario::TIPO_BASE | ConfigTipoUsuario::TIPO_DOMINIO);
}
//----------------------------------------------------------
void ColunaWidget::aplicarConfiguracao(void)
{
 try
 {
  Coluna *coluna=NULL;

  iniciarConfiguracao<Coluna>();

  //Obtém a referêni �  coluna que está sendo criada/editada
  coluna=dynamic_cast<Coluna *>(this->objeto);

  //Atribui os dados do formulári�  coluna
  coluna->definirNaoNulo(nao_nulo_chk->isChecked());
  coluna->definirValorPadrao(valor_padrao_txt->toPlainText());
  coluna->definirTipo(tipo_col->obterTipoPgSQL());

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

