#include "sequenciawidget.h"
//***********************************************************
SequenciaWidget::SequenciaWidget(QWidget *parent): ObjetoBaseWidget(parent, OBJETO_SEQUENCIA)
{
 try
 {
  map<QString, vector<QWidget *> > mapa_campos;
  QFrame *frame=NULL;

  Ui_SequenciaWidget::setupUi(this);
  sel_coluna=NULL;
  sel_coluna=new SeletorObjetoWidget(OBJETO_COLUNA, true, this);

  //Adicionando os objetos recém alocados no layout do formulário
  sequencia_grid->addWidget(sel_coluna,3,1,1,3);

  //Define os campos exclusivos para cada versão
  mapa_campos[gerarIntervaloVersoes(APOS_VERSAO, ParserEsquema::VERSAO_PGSQL_83)].push_back(possuidora_lbl);

  //Gera o frame de alerta
  frame=gerarFrameAlertaVersao(mapa_campos);
  sequencia_grid->addWidget(frame, sequencia_grid->count()+1, 0, 1, 0);
  frame->setParent(this);

  configurarLayouFormulario(sequencia_grid, OBJETO_SEQUENCIA);

  connect(janela_pai->aplicar_ok_btn,SIGNAL(clicked(bool)), this, SLOT(aplicarConfiguracao(void)));

  janela_pai->resize(540, 410);
  janela_pai->setMinimumWidth(540);
  janela_pai->setMinimumHeight(430);
  janela_pai->setMaximumHeight(430);
 }
 catch(Excecao &e)
 {
  //Redireciona o erro
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}
//----------------------------------------------------------
void SequenciaWidget::hideEvent(QHideEvent *evento)
{
 sel_coluna->removerObjetoSelecionado();
 ObjetoBaseWidget::hideEvent(evento);
}
//----------------------------------------------------------
void SequenciaWidget::definirAtributos(ModeloBD *modelo, ListaOperacoes *lista_op, Sequencia *sequencia)
{
 sel_coluna->definirModelo(modelo);

 if(sequencia)
 {
  sel_coluna->definirObjeto(sequencia->obterPossuidora());
  ciclica_chk->setChecked(sequencia->sequenciaCiclica());
  inicio_edt->setText(sequencia->obterInicio());
  maximo_edt->setText(sequencia->obterValorMax());
  minimo_edt->setText(sequencia->obterValorMin());
  cache_edt->setText(sequencia->obterCache());
  incremento_edt->setText(sequencia->obterIncremento());
 }
 else
 {
  ciclica_chk->setChecked(false);
  inicio_edt->setText("1");
  maximo_edt->setText(Sequencia::VALOR_MAX_POSITIVO);
  minimo_edt->setText("0");
  cache_edt->setText("1");
  incremento_edt->setText("1");
 }

 //Preenchendo os campos básicos do formulário com os atributos da sequência
 ObjetoBaseWidget::definirAtributos(modelo,lista_op,sequencia);
}
//---------------------------------------------------------
void SequenciaWidget::aplicarConfiguracao(void)
{
 try
 {
  Sequencia *sequencia=NULL;
  iniciarConfiguracao<Sequencia>();

  //Obtém a referência à sequência que está sendo editada/criada
  sequencia=dynamic_cast<Sequencia *>(this->objeto);

  sequencia->definirCiclica(ciclica_chk->isChecked());
  sequencia->definirValores(minimo_edt->text(), maximo_edt->text(), incremento_edt->text(),
                            inicio_edt->text(), cache_edt->text());
  sequencia->definirPossuidora(dynamic_cast<Coluna *>(sel_coluna->obterObjeto()));

  //Finaliza a configuração
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
