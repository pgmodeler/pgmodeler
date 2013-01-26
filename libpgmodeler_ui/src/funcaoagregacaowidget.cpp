#include "funcaoagregacaowidget.h"

FuncaoAgregacaoWidget::FuncaoAgregacaoWidget(QWidget *parent): BaseObjectWidget(parent, OBJ_AGGREGATE)
{
 try
 {
  QGridLayout *grid=NULL;
  Ui_FuncaoAgregacaoWidget::setupUi(this);
  QSpacerItem *spacer=NULL;
  QFrame *frame=NULL;

  //Cria um destacador de sintaxe no campo de condição inicial da função
  destaque_cond=NULL;
  destaque_cond=new SyntaxHighlighter(cond_inicial_txt, false);
  destaque_cond->loadConfiguration(GlobalAttributes::CONFIGURATIONS_DIR +
                                      GlobalAttributes::DIR_SEPARATOR +
                                      GlobalAttributes::SQL_HIGHLIGHT_CONF +
                                      GlobalAttributes::CONFIGURATION_EXT);

  //Alocando os seletores de objetos (função e operador) que são atriuíos   função de agregação
  sel_func_final=NULL;
  sel_func_transicao=NULL;
  sel_op_ordenacao=NULL;

  sel_func_final=new SeletorObjetoWidget(OBJ_FUNCTION, true, this);
  sel_func_transicao=new SeletorObjetoWidget(OBJ_FUNCTION, true, this);
  sel_op_ordenacao=new SeletorObjetoWidget(OBJ_OPERATOR, true, this);

  //Alocando os widgets de configuração de tipo pgsql
  tipo_entrada=NULL;
  tipo_estado=NULL;

  tipo_entrada=new TipoPgSQLWidget(this, trUtf8("Input Data Type"));
  tipo_estado=new TipoPgSQLWidget(this, trUtf8("State Data Type"));

  //Alocando a tabela de tipos de entrada da função de agregação.
  tab_tipos_entrada=NULL;
  tab_tipos_entrada=new TabelaObjetosWidget(TabelaObjetosWidget::TODOS_BOTOES ^
                                            TabelaObjetosWidget::BTN_EDITAR_ITEM, true, this);
  tab_tipos_entrada->definirNumColunas(1);

  //Adicionando os objetos recém alocados no layout do formulário
  funcaoagregacao_grid->addWidget(sel_func_final,0,1,1,1);
  funcaoagregacao_grid->addWidget(sel_func_transicao,1,1,1,1);
  funcaoagregacao_grid->addWidget(sel_op_ordenacao,2,1,1,1);

  grid=new QGridLayout;
  grid->setContentsMargins(2,2,2,2);
  grid->addWidget(tipo_entrada,0,0);
  grid->addWidget(tab_tipos_entrada,1,0);
  tipos_entrada_estado_twg->widget(0)->setLayout(grid);

  grid=new QGridLayout;
  spacer=new QSpacerItem(20, 1, QSizePolicy::Minimum, QSizePolicy::Expanding);

  grid->setContentsMargins(2,2,2,2);
  grid->addWidget(tipo_estado,0,0);
  grid->addItem(spacer,1,0);
  tipos_entrada_estado_twg->widget(1)->setLayout(grid);

  //Gera o frame de informação sobre a criação de funções agregadas
  frame=generateInformationFrame(trUtf8("An aggregate function that accepts the types <em><strong>typeA</strong></em> and <em><strong>typeB</strong></em> as input types and which type of state is <em><strong>state_type</strong></em>, must obey the following rules: <br/><br/> <strong> &nbsp;&nbsp;&nbsp;• Final Function:</strong> <em>void final_function(<strong>state_type</strong>)</em><br/>  <strong> &nbsp;&nbsp;&nbsp;• Transition Function:</strong> <em><strong>state_type</strong> transition_function(<strong>state_type</strong>, <strong>typeA</strong>, <strong>typeB</strong>)</em>"));
  funcaoagregacao_grid->addWidget(frame, funcaoagregacao_grid->count()+1, 0, 1, 2);
  frame->setParent(this);

  configureFormLayout(funcaoagregacao_grid, OBJ_AGGREGATE);

  parent_form->setMinimumSize(645, 750);

  connect(parent_form->aplicar_ok_btn,SIGNAL(clicked(bool)), this, SLOT(applyConfiguration(void)));
  connect(tab_tipos_entrada, SIGNAL(s_linhaAdicionada(int)), this, SLOT(manipularTipoDado(int)));
  connect(tab_tipos_entrada, SIGNAL(s_linhaAtualizada(int)), this, SLOT(manipularTipoDado(int)));
 }
 catch(Exception &e)
 {
  //Redireciona o erro
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}

void FuncaoAgregacaoWidget::hideEvent(QHideEvent *evento)
{
 sel_func_final->removerObjetoSelecionado();
 sel_func_transicao->removerObjetoSelecionado();
 sel_op_ordenacao->removerObjetoSelecionado();
 tab_tipos_entrada->removerLinhas();
 cond_inicial_txt->clear();
 BaseObjectWidget::hideEvent(evento);
}

void FuncaoAgregacaoWidget::setAttributes(DatabaseModel *modelo, OperationList *lista_op, Aggregate *funcao_ag)
{
 unsigned qtd, i;
 PgSQLType tipo;

 //Preenchendo os campos básicos do formulário com os atributos da função de agregação
 BaseObjectWidget::setAttributes(modelo,lista_op,funcao_ag);

 //Configurado o modelo de banco de dados referênciado pelos widget seletores
 tipo_entrada->definirAtributos(tipo, modelo);
 tipo_estado->definirAtributos(tipo, modelo);
 sel_func_final->definirModelo(modelo);
 sel_func_transicao->definirModelo(modelo);
 sel_op_ordenacao->definirModelo(modelo);

 //Caso a função de agregação esteja alocada (sendo editada)
 if(funcao_ag)
 {
  //Preenche os seletores de função e operador com as funções e operador configurados na função de agregação
  sel_func_final->definirObjeto(funcao_ag->getFunction(Aggregate::FINAL_FUNC));
  sel_func_transicao->definirObjeto(funcao_ag->getFunction(Aggregate::TRANSITION_FUNC));
  sel_op_ordenacao->definirObjeto(funcao_ag->getSortOperator());

  //Exibe a expressão de condição inicial da função no formulário
  cond_inicial_txt->setPlainText(QString::fromUtf8(funcao_ag->getInitialCondition()));

  //Preenche a tabela de tipos de entrada com os tipos configurados na função de agregação
  tab_tipos_entrada->blockSignals(true);
  qtd=funcao_ag->getDataTypeCount();

  for(i=0; i < qtd; i++)
  {
   tab_tipos_entrada->adicionarLinha();
   tipo=funcao_ag->getDataType(i);
   tab_tipos_entrada->definirDadoLinha(QVariant::fromValue<PgSQLType>(tipo), i);
   tab_tipos_entrada->definirTextoCelula(QString::fromUtf8(*tipo),i,0);
  }
  tab_tipos_entrada->blockSignals(false);
  tab_tipos_entrada->limparSelecao();

  //Configura o widget de tipo de estado marcando o tipo de dado configurado na função
  tipo_estado->definirAtributos(funcao_ag->getStateType(), modelo);
 }
}
//---------------------------------------------------------
void FuncaoAgregacaoWidget::manipularTipoDado(int linha)
{
 PgSQLType tipo;

 //Obtém o tipo de dado configurado no widget de tipo de entrada
 tipo=tipo_entrada->obterTipoPgSQL();
 //O tipo obtido é inserido na tabela e a linha correspondente guarda-o como dado interno
 tab_tipos_entrada->definirDadoLinha(QVariant::fromValue<PgSQLType>(tipo), linha);
 tab_tipos_entrada->definirTextoCelula(QString::fromUtf8(*tipo),linha,0);
}
//---------------------------------------------------------
void FuncaoAgregacaoWidget::applyConfiguration(void)
{
 try
 {
  Aggregate *funcao_ag=NULL;
  unsigned qtd, i;

  startConfiguration<Aggregate>();

  //Obtém a referêni   função de agregação que está sendo editada/criada
  funcao_ag=dynamic_cast<Aggregate *>(this->object);

  //Configura os atributos do mesmo com os valores definidos no formulário
  funcao_ag->setInitialCondition(cond_inicial_txt->toPlainText());
  funcao_ag->setStateType(tipo_estado->obterTipoPgSQL());

  /* Remove todos os tipos de dados da função de agregação para inserir os
     que estão na tabela de tipos de entrada */
  funcao_ag->removeDataTypes();
  qtd=tab_tipos_entrada->obterNumLinhas();

  /* Obtém de cada linha da tabela de tipos de entrada o dado interno o qual é
     uma instância da classe TipoPgSQL e que será atribía   função de agregação
     como tipo de dado de entrada */
  for(i=0; i < qtd; i++)
   funcao_ag->addDataType(tab_tipos_entrada->obterDadoLinha(i).value<PgSQLType>());

  //Define as funções e operador obtendo tais objetos dos respectivos seletores
  funcao_ag->setFunction(Aggregate::TRANSITION_FUNC, dynamic_cast<Function *>(sel_func_transicao->obterObjeto()));
  funcao_ag->setFunction(Aggregate::FINAL_FUNC, dynamic_cast<Function *>(sel_func_final->obterObjeto()));
  funcao_ag->setSortOperator(dynamic_cast<Operator *>(sel_op_ordenacao->obterObjeto()));

  //Finaliza a configuração da função de agregação
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

