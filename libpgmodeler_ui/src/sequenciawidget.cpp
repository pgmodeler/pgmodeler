#include "sequenciawidget.h"

SequenciaWidget::SequenciaWidget(QWidget *parent): ObjetoBaseWidget(parent, OBJ_SEQUENCE)
{
 try
 {
  map<QString, vector<QWidget *> > mapa_campos;
  QFrame *frame=NULL;

  Ui_SequenciaWidget::setupUi(this);
  sel_coluna=NULL;
  sel_coluna=new SeletorObjetoWidget(OBJ_COLUMN, true, this);

  //Adicionando os objetos recém alocados no layout do formulário
  sequencia_grid->addWidget(sel_coluna,3,1,1,3);

  //Define os campos exclusivos para cada versão
  mapa_campos[gerarIntervaloVersoes(APOS_VERSAO, SchemaParser::PGSQL_VERSION_83)].push_back(possuidora_lbl);

  //Gera o frame de alerta
  frame=gerarFrameAlertaVersao(mapa_campos);
  sequencia_grid->addWidget(frame, sequencia_grid->count()+1, 0, 1, 0);
  frame->setParent(this);

  configurarLayouFormulario(sequencia_grid, OBJ_SEQUENCE);

  connect(janela_pai->aplicar_ok_btn,SIGNAL(clicked(bool)), this, SLOT(aplicarConfiguracao(void)));

  janela_pai->resize(540, 410);
  janela_pai->setMinimumWidth(540);
  janela_pai->setMinimumHeight(430);
  janela_pai->setMaximumHeight(430);
 }
 catch(Exception &e)
 {
  //Redireciona o erro
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}

void SequenciaWidget::hideEvent(QHideEvent *evento)
{
 sel_coluna->removerObjetoSelecionado();
 ObjetoBaseWidget::hideEvent(evento);
}

void SequenciaWidget::definirAtributos(DatabaseModel *modelo, OperationList *lista_op, Sequence *sequencia)
{
 sel_coluna->definirModelo(modelo);

 if(sequencia)
 {
  sel_coluna->definirObjeto(sequencia->getOwnerColumn());
  ciclica_chk->setChecked(sequencia->isCycle());
  inicio_edt->setText(sequencia->getStart());
  maximo_edt->setText(sequencia->getMaxValue());
  minimo_edt->setText(sequencia->getMinValue());
  cache_edt->setText(sequencia->getCache());
  incremento_edt->setText(sequencia->getIncrement());
 }
 else
 {
  ciclica_chk->setChecked(false);
  inicio_edt->setText("1");
  maximo_edt->setText(Sequence::MAX_POSITIVE_VALUE);
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
  Sequence *sequencia=NULL;
  iniciarConfiguracao<Sequence>();

  //Obtém a referêni   sequência que está sendo editada/criada
  sequencia=dynamic_cast<Sequence *>(this->objeto);

  sequencia->setCycle(ciclica_chk->isChecked());
  sequencia->setValues(minimo_edt->text(), maximo_edt->text(), incremento_edt->text(),
                            inicio_edt->text(), cache_edt->text());
  sequencia->setOwnerColumn(dynamic_cast<Column *>(sel_coluna->obterObjeto()));

  //Finaliza a configuração
  ObjetoBaseWidget::aplicarConfiguracao();
  finalizarConfiguracao();
 }
 catch(Exception &e)
 {
  /* Cancela a configuração o objeto removendo a ultima operação adicionada
     referente ao objeto editado/criado e desaloca o objeto
     caso o mesmo seja novo */
  cancelarConfiguracao();
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}

