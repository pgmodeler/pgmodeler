#include "funcaoagregacaowidget.h"
//***********************************************************
FuncaoAgregacaoWidget::FuncaoAgregacaoWidget(QWidget *parent): ObjetoBaseWidget(parent, OBJETO_FUNC_AGREGACAO)
{
 try
 {
  QGridLayout *grid=NULL;
  Ui_FuncaoAgregacaoWidget::setupUi(this);
  QSpacerItem *spacer=NULL;
  QFrame *frame=NULL;

  //Cria um destacador de sintaxe no campo de condição inicial da função
  destaque_cond=NULL;
  destaque_cond=new DestaqueSintaxe(cond_inicial_txt, false);
  destaque_cond->carregarConfiguracao(AtributosGlobais::DIR_CONFIGURACOES +
                                      AtributosGlobais::SEP_DIRETORIO +
                                      AtributosGlobais::CONF_DESTAQUE_SQL +
                                      AtributosGlobais::EXT_CONFIGURACAO);

  //Alocando os seletores de objetos (função e operador) que são atribuídos à função de agregação
  sel_func_final=NULL;
  sel_func_transicao=NULL;
  sel_op_ordenacao=NULL;

  sel_func_final=new SeletorObjetoWidget(OBJETO_FUNCAO, true, this);
  sel_func_transicao=new SeletorObjetoWidget(OBJETO_FUNCAO, true, this);
  sel_op_ordenacao=new SeletorObjetoWidget(OBJETO_OPERADOR, true, this);

  //Alocando os widgets de configuração de tipo pgsql
  tipo_entrada=NULL;
  tipo_estado=NULL;

  tipo_entrada=new TipoPgSQLWidget(this, trUtf8("Tipo de Dados de Entrada"));
  tipo_estado=new TipoPgSQLWidget(this, trUtf8("Tipo de Dado de Estado"));

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
  frame=gerarFrameInformacao(trUtf8("Uma função de agregação que aceita os tipos <em><strong>tipoA</strong></em> e <em><strong>tipoB</strong></em>\
                                    como entrada e cujo tipo de estado seja <em><strong>tipo_estado</strong></em>, deve obedecer às seguintes regras:<br/><br/> \
                                    <strong> &nbsp;&nbsp;&nbsp;• Função Final:</strong> <em>void funcao_final(<strong>tipo_estado</strong>)</em><br/>\
  <strong> &nbsp;&nbsp;&nbsp;• Função Transição:</strong> <em><strong>tipo_estado</strong> funcao_transicao(<strong>tipo_estado</strong>, <strong>tipoA</strong>, <strong>tipoB</strong>)</em>"));
  funcaoagregacao_grid->addWidget(frame, funcaoagregacao_grid->count()+1, 0, 1, 2);
  frame->setParent(this);

  configurarLayouFormulario(funcaoagregacao_grid, OBJETO_FUNC_AGREGACAO);

  janela_pai->setMinimumSize(600, 500);

  connect(janela_pai->aplicar_ok_btn,SIGNAL(clicked(bool)), this, SLOT(aplicarConfiguracao(void)));
  connect(tab_tipos_entrada, SIGNAL(s_linhaAdicionada(int)), this, SLOT(manipularTipoDado(int)));
  connect(tab_tipos_entrada, SIGNAL(s_linhaAtualizada(int)), this, SLOT(manipularTipoDado(int)));
 }
 catch(Excecao &e)
 {
  //Redireciona o erro
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}
//----------------------------------------------------------
void FuncaoAgregacaoWidget::hideEvent(QHideEvent *evento)
{
 sel_func_final->removerObjetoSelecionado();
 sel_func_transicao->removerObjetoSelecionado();
 sel_op_ordenacao->removerObjetoSelecionado();
 tab_tipos_entrada->removerLinhas();
 cond_inicial_txt->clear();
 ObjetoBaseWidget::hideEvent(evento);
}
//----------------------------------------------------------
void FuncaoAgregacaoWidget::definirAtributos(ModeloBD *modelo, ListaOperacoes *lista_op, FuncaoAgregacao *funcao_ag)
{
 unsigned qtd, i;
 TipoPgSQL tipo;

 //Preenchendo os campos básicos do formulário com os atributos da função de agregação
 ObjetoBaseWidget::definirAtributos(modelo,lista_op,funcao_ag);

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
  sel_func_final->definirObjeto(funcao_ag->obterFuncao(FuncaoAgregacao::FUNCAO_FINAL));
  sel_func_transicao->definirObjeto(funcao_ag->obterFuncao(FuncaoAgregacao::FUNCAO_TRANSICAO));
  sel_op_ordenacao->definirObjeto(funcao_ag->obterOperadorOrdenacao());

  //Exibe a expressão de condição inicial da função no formulário
  cond_inicial_txt->setPlainText(QString::fromUtf8(funcao_ag->obterCondicaoInicial()));

  //Preenche a tabela de tipos de entrada com os tipos configurados na função de agregação
  tab_tipos_entrada->blockSignals(true);
  qtd=funcao_ag->obterNumTipoDados();

  for(i=0; i < qtd; i++)
  {
   tab_tipos_entrada->adicionarLinha();
   tipo=funcao_ag->obterTipoDado(i);
   tab_tipos_entrada->definirDadoLinha(QVariant::fromValue<TipoPgSQL>(tipo), i);
   tab_tipos_entrada->definirTextoCelula(QString::fromUtf8(*tipo),i,0);
  }
  tab_tipos_entrada->blockSignals(false);
  tab_tipos_entrada->limparSelecao();

  //Configura o widget de tipo de estado marcando o tipo de dado configurado na função
  tipo_estado->definirAtributos(funcao_ag->obterTipoEstado(), modelo);
 }
}
//---------------------------------------------------------
void FuncaoAgregacaoWidget::manipularTipoDado(int linha)
{
 TipoPgSQL tipo;

 //Obtém o tipo de dado configurado no widget de tipo de entrada
 tipo=tipo_entrada->obterTipoPgSQL();
 //O tipo obtido é inserido na tabela e a linha correspondente guarda-o como dado interno
 tab_tipos_entrada->definirDadoLinha(QVariant::fromValue<TipoPgSQL>(tipo), linha);
 tab_tipos_entrada->definirTextoCelula(QString::fromUtf8(*tipo),linha,0);
}
//---------------------------------------------------------
void FuncaoAgregacaoWidget::aplicarConfiguracao(void)
{
 try
 {
  FuncaoAgregacao *funcao_ag=NULL;
  unsigned qtd, i;

  iniciarConfiguracao<FuncaoAgregacao>();

  //Obtém a referência à função de agregação que está sendo editada/criada
  funcao_ag=dynamic_cast<FuncaoAgregacao *>(this->objeto);

  //Configura os atributos do mesmo com os valores definidos no formulário
  funcao_ag->definirCondicaoInicial(cond_inicial_txt->toPlainText());
  funcao_ag->definirTipoEstado(tipo_estado->obterTipoPgSQL());

  /* Remove todos os tipos de dados da função de agregação para inserir os
     que estão na tabela de tipos de entrada */
  funcao_ag->removerTiposDado();
  qtd=tab_tipos_entrada->obterNumLinhas();

  /* Obtém de cada linha da tabela de tipos de entrada o dado interno o qual é
     uma instância da classe TipoPgSQL e que será atribuída à função de agregação
     como tipo de dado de entrada */
  for(i=0; i < qtd; i++)
   funcao_ag->adicionarTipoDado(tab_tipos_entrada->obterDadoLinha(i).value<TipoPgSQL>());

  //Define as funções e operador obtendo tais objetos dos respectivos seletores
  funcao_ag->definirFuncao(FuncaoAgregacao::FUNCAO_TRANSICAO, dynamic_cast<Funcao *>(sel_func_transicao->obterObjeto()));
  funcao_ag->definirFuncao(FuncaoAgregacao::FUNCAO_FINAL, dynamic_cast<Funcao *>(sel_func_final->obterObjeto()));
  funcao_ag->definirOperadorOrdenacao(dynamic_cast<Operador *>(sel_op_ordenacao->obterObjeto()));

  //Finaliza a configuração da função de agregação
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
