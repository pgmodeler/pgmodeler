#include "sequenciawidget.h"

SequenciaWidget::SequenciaWidget(QWidget *parent): BaseObjectWidget(parent, OBJ_SEQUENCE)
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
  mapa_campos[generateVersionsInterval(AFTER_VERSION, SchemaParser::PGSQL_VERSION_83)].push_back(possuidora_lbl);

  //Gera o frame de alerta
  frame=generateVersionWarningFrame(mapa_campos);
  sequencia_grid->addWidget(frame, sequencia_grid->count()+1, 0, 1, 0);
  frame->setParent(this);

  configureFormLayout(sequencia_grid, OBJ_SEQUENCE);

  connect(parent_form->aplicar_ok_btn,SIGNAL(clicked(bool)), this, SLOT(applyConfiguration(void)));

  parent_form->resize(540, 410);
  parent_form->setMinimumWidth(540);
  parent_form->setMinimumHeight(430);
  parent_form->setMaximumHeight(430);
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
 BaseObjectWidget::hideEvent(evento);
}

void SequenciaWidget::setAttributes(DatabaseModel *modelo, OperationList *lista_op, Sequence *sequencia)
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
 BaseObjectWidget::setAttributes(modelo,lista_op,sequencia);
}
//---------------------------------------------------------
void SequenciaWidget::applyConfiguration(void)
{
 try
 {
  Sequence *sequencia=NULL;
  startConfiguration<Sequence>();

  //Obtém a referêni   sequência que está sendo editada/criada
  sequencia=dynamic_cast<Sequence *>(this->object);

  sequencia->setCycle(ciclica_chk->isChecked());
  sequencia->setValues(minimo_edt->text(), maximo_edt->text(), incremento_edt->text(),
                            inicio_edt->text(), cache_edt->text());
  sequencia->setOwnerColumn(dynamic_cast<Column *>(sel_coluna->obterObjeto()));

  //Finaliza a configuração
  BaseObjectWidget::applyConfiguration();
  finishConfiguration();
 }
 catch(Exception &e)
 {
  /* Cancela a configuração o objeto removendo a ultima operação adicionada
     referente ao objeto editado/criado e desaloca o objeto
     caso o mesmo seja novo */
  cancelConfiguration();
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}

