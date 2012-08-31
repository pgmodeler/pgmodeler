#include "classeoperadoreswidget.h"
//***********************************************************
ClasseOperadoresWidget::ClasseOperadoresWidget(QWidget *parent): ObjetoBaseWidget(parent, OBJETO_CLASSE_OPER)
{
 try
 {
  QStringList tipos;
  QGridLayout *grid=NULL;

  Ui_ClasseOperadoresWidget::setupUi(this);

  sel_familiaop=NULL;
  sel_operador=NULL;
  sel_funcao=NULL;
  tipo_dado=NULL;
  tipo_armazenamento=NULL;
  tab_elementos=NULL;

  sel_familiaop=new SeletorObjetoWidget(OBJETO_FAMILIA_OPER, false, this);
  sel_operador=new SeletorObjetoWidget(OBJETO_OPERADOR, true, this);
  sel_funcao=new SeletorObjetoWidget(OBJETO_FUNCAO, true, this);
  tipo_dado=new TipoPgSQLWidget(this);
  tipo_armazenamento=new TipoPgSQLWidget(this, trUtf8("Tipo de Armazenamento"));
  tab_elementos=new TabelaObjetosWidget(TabelaObjetosWidget::TODOS_BOTOES, true, this);

  /* A tabela de elementos é constituída de 4 colunas:
     Nome do objeto, tipo, nº de suporte/estratégia e rechecagem. */
  tab_elementos->definirNumColunas(4);
  tab_elementos->definirRotuloCabecalho(trUtf8("Objeto"),0);
  tab_elementos->definirIconeCabecalho(QPixmap(":/icones/icones/table.png"),0);

  tab_elementos->definirRotuloCabecalho(trUtf8("Tipo"),1);
  tab_elementos->definirIconeCabecalho(QPixmap(":/icones/icones/usertype.png"),1);

  tab_elementos->definirRotuloCabecalho(trUtf8("Suporte/Estratégia"),2);
  tab_elementos->definirRotuloCabecalho(trUtf8("Rechecar"),3);

  //Configurando a disposição dos campos no layout
  grid=new QGridLayout;
  grid->setContentsMargins(0,0,0,0);
  grid->addWidget(classepadrao_lbl,0,2,1,1);
  grid->addWidget(classepadrao_chk,0,3,1,1);
  grid->addWidget(tipo_index_lbl,0,0,1,1);
  grid->addWidget(tipo_index_cmb,0,1,1,1);
  grid->addWidget(familia_lbl,2,0,1,1);
  grid->addWidget(sel_familiaop,2,1,1,4);
  grid->addWidget(tipo_dado,4,0,1,5);
  grid->addWidget(elementos_grp,5,0,1,5);
  this->setLayout(grid);
  configurarLayouFormulario(grid, OBJETO_CLASSE_OPER);

  grid=dynamic_cast<QGridLayout *>(elementos_grp->layout());
  grid->addWidget(sel_funcao, 1,1,1,4);
  grid->addWidget(sel_operador, 2,1,1,4);
  grid->addWidget(tipo_armazenamento, 5,0,1,5);
  grid->addWidget(tab_elementos, 6,0,1,4);

  connect(janela_pai->aplicar_ok_btn,SIGNAL(clicked(bool)), this, SLOT(aplicarConfiguracao(void)));
  connect(tipo_elem_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(selecionarTipoElemento(int)));

  //Conectando os sinais emitidos pela tabela de elementos ao slots de manipulação de elementos
  connect(tab_elementos, SIGNAL(s_linhaAdicionada(int)), this, SLOT(manipularElemento(int)));
  connect(tab_elementos, SIGNAL(s_linhaAtualizada(int)), this, SLOT(manipularElemento(int)));
  connect(tab_elementos, SIGNAL(s_linhaEditada(int)), this, SLOT(editarElemento(int)));

  janela_pai->setMinimumSize(540, 590);
  selecionarTipoElemento(0);

  //Configura o combobox do formulário listando todos os tipos de indexação do PostgreSQL
  TipoIndexacao::obterTipos(tipos);
  tipo_index_cmb->addItems(tipos);
 }
 catch(Excecao &e)
 {
  //Redireciona o erro
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}
//----------------------------------------------------------
void ClasseOperadoresWidget::hideEvent(QHideEvent *evento)
{
 //Retorna os widgets aos valores padrão
 sel_funcao->removerObjetoSelecionado();
 sel_operador->removerObjetoSelecionado();
 num_suporte_sb->setValue(1);
 rechecar_chk->setChecked(false);
 tab_elementos->removerLinhas();
 selecionarTipoElemento(0);

 //Executa o método que trata o evento de esconder da classe superior
 ObjetoBaseWidget::hideEvent(evento);
}
//----------------------------------------------------------
void ClasseOperadoresWidget::selecionarTipoElemento(int tipo)
{
 unsigned tipo_sel=static_cast<unsigned>(tipo);

 //Exibe os campos do elemento conforme o tipo selecionado

 //Campos exclusivos para o tipo FUNÇÃO
 funcao_lbl->setVisible(tipo_sel==ElemClasseOperadores::ELEM_FUNCAO);
 sel_funcao->setVisible(tipo_sel==ElemClasseOperadores::ELEM_FUNCAO);

 //Campos exclusivos para o tipo OPERADOR
 operador_lbl->setVisible(tipo_sel==ElemClasseOperadores::ELEM_OPERADOR);
 sel_operador->setVisible(tipo_sel==ElemClasseOperadores::ELEM_OPERADOR);
 rechecar_chk->setVisible(tipo_sel==ElemClasseOperadores::ELEM_OPERADOR);

 //Campos exclusivos para o tipo ARMAZENAMENTO
 tipo_armazenamento->setVisible(tipo_sel==ElemClasseOperadores::ELEM_ARMAZENAMENTO);

 //Campos comuns aos tipos FUNÇÃO e OPERADOR
 num_suporte_lbl->setVisible(tipo_sel!=ElemClasseOperadores::ELEM_ARMAZENAMENTO);
 num_suporte_sb->setVisible(tipo_sel!=ElemClasseOperadores::ELEM_ARMAZENAMENTO);
}
//----------------------------------------------------------
void ClasseOperadoresWidget::editarElemento(int idx_linha)
{
 ElemClasseOperadores elem;

 //Obtém o elemento da linha selecionada na tabela
 elem=tab_elementos->obterDadoLinha(idx_linha).value<ElemClasseOperadores>();

 //Preenche o formulário com atributos do elemento
 tipo_elem_cmb->setCurrentIndex(elem.obterTipoElemento());
 sel_funcao->definirObjeto(elem.obterFuncao());
 sel_operador->definirObjeto(elem.obterOperador());
 rechecar_chk->setChecked(elem.rechecarElemento());
 num_suporte_sb->setValue(elem.obterNumEstrategia());
 tipo_armazenamento->definirAtributos(elem.obterTipoArmazenamento(),this->modelo);
}
//----------------------------------------------------------
void ClasseOperadoresWidget::exibirDadosElemento(ElemClasseOperadores elem, int idx_linha)
{
 unsigned tipo_elem;

 //Obtém o tipo do elemento
 tipo_elem=elem.obterTipoElemento();

 /* Para o tipo FUNÇÃO são exibidas na coluna 0 e 1, respectivamente,
    a assinatura da função e o tipo do objeto (função) */
 if(tipo_elem==ElemClasseOperadores::ELEM_FUNCAO)
 {
  tab_elementos->definirTextoCelula(QString::fromUtf8(elem.obterFuncao()->obterAssinatura()), idx_linha, 0);
  tab_elementos->definirTextoCelula(QString::fromUtf8(elem.obterFuncao()->obterNomeTipoObjeto()), idx_linha, 1);
 }
 /* Para o tipo OPERADOR são exibidas na coluna 0 e 1, respectivamente,
    a assinatura do operador e o tipo do objeto (operador) */
 else if(tipo_elem==ElemClasseOperadores::ELEM_OPERADOR)
 {
  tab_elementos->definirTextoCelula(QString::fromUtf8(elem.obterOperador()->obterAssinatura()), idx_linha, 0);
  tab_elementos->definirTextoCelula(QString::fromUtf8(elem.obterOperador()->obterNomeTipoObjeto()), idx_linha, 1);
 }
  /* Para o tipo ARMAZENAMENTO são exibidas na coluna 0 e 1, respectivamente,
    a nome completo do tipo e o tipo do objeto (tipo) */
 else
 {
  tab_elementos->definirTextoCelula(*elem.obterTipoArmazenamento(), idx_linha, 0);
  tab_elementos->definirTextoCelula(QString::fromUtf8(ObjetoBase::obterNomeTipoObjeto(OBJETO_TIPO)), idx_linha, 1);
 }

 /* Para os tipos FUNÇÃO e OPERADOR é na coluna 2
    o número de suporte/estratégia da função ou operador */
 if(tipo_elem!=ElemClasseOperadores::ELEM_ARMAZENAMENTO)
  tab_elementos->definirTextoCelula(QString("%1").arg(elem.obterNumEstrategia()), idx_linha, 2);
 /* Para o tipo ARMAZENAMENTO este campo não é preenchido pois não aplica
    esta categoria de elemento */
 else
  tab_elementos->definirTextoCelula("-", idx_linha, 2);

 /* Para o tipo OPERADOR na coluna 3 é exibido se o operador
    necessita ser rechecado ou não */
 if(tipo_elem==ElemClasseOperadores::ELEM_OPERADOR)
 {
  if(elem.rechecarElemento())
   tab_elementos->definirTextoCelula(trUtf8("Sim"), idx_linha, 3);
  else
   tab_elementos->definirTextoCelula(trUtf8("Não"), idx_linha, 3);
 }
 /* Para os tipos FUNÇÃO e ARMAZENAMENTO este campo não é preenchido
    pois não aplica esta categorias de elementos */
 else
  tab_elementos->definirTextoCelula("-", idx_linha, 3);

 //Define o dado da linha como sendo o elemento fornecido no parâmetro.
 tab_elementos->definirDadoLinha(QVariant::fromValue<ElemClasseOperadores>(elem), idx_linha);
}
//----------------------------------------------------------
void ClasseOperadoresWidget::manipularElemento(int idx_linha)
{
 ElemClasseOperadores elem;
 unsigned tipo_elem;

 //Obtém o tipo do elemento
 tipo_elem=tipo_elem_cmb->currentIndex();

 try
 {
  /* Caso seja do tipo FUNÇÃO, atribui a função selecionado no widget seletor de função
     e também o valor configurado como suporte no formulário */
  if(tipo_elem==ElemClasseOperadores::ELEM_FUNCAO)
   elem.definirFuncao(dynamic_cast<Funcao *>(sel_funcao->obterObjeto()), num_suporte_sb->value());
  /* Caso seja do tipo OPERADOR, atribui o operador selecionado no widget seletor de operador,
     o valor configurado como suporte no formulário e o estado do checkbox de rechecagem */
  else  if(tipo_elem==ElemClasseOperadores::ELEM_OPERADOR)
   elem.definirOperador(dynamic_cast<Operador *>(sel_operador->obterObjeto()), num_suporte_sb->value(), rechecar_chk->isChecked());
  //Caso seja do tipo ARMAZENAMENTO, atribui o tipo configurado no widget de configuração de tipo
  else
   elem.definirArmazenamento(tipo_armazenamento->obterTipoPgSQL());

  //Exibe o elemento configurado na tabela
  exibirDadosElemento(elem, idx_linha);

  //Reinicia o formulário de configuração de elementos
  sel_funcao->removerObjetoSelecionado();
  sel_operador->removerObjetoSelecionado();
  num_suporte_sb->setValue(1);
  rechecar_chk->setChecked(false);
  tab_elementos->limparSelecao();
 }
 catch(Excecao &e)
 {
  /* Em caso de erro e caso a linha atual da tabela esteja sem dados (algum campo sem texto)
     indica que a operação de inserção de um elemento é que gerou a exceção e para tanto
     a linha recém adicionada precisa ser removida */
  if(tab_elementos->obterTextoCelula(idx_linha, 0).isEmpty())
   tab_elementos->removerLinha(idx_linha);

  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}
//----------------------------------------------------------
void ClasseOperadoresWidget::definirAtributos(ModeloBD *modelo, ListaOperacoes *lista_op, ClasseOperadores *classe_op)
{
 TipoPgSQL tipo;
 unsigned i, qtd;

 //Define os atributos do formulários e da janela pai
 ObjetoBaseWidget::definirAtributos(modelo, lista_op, classe_op);

 //Configura o modelo de objetos de referência dos widgets
 sel_familiaop->definirModelo(modelo);
 sel_funcao->definirModelo(modelo);
 sel_operador->definirModelo(modelo);
 tipo_armazenamento->definirAtributos(tipo, modelo);

 //Caso a classe de operadores passada esteja alcada
 if(classe_op)
 {
  //Obtém o tipo de dado que a classe opera
  tipo=classe_op->obterTipoDado();

  //Exibe no formulário a família de operadores usada pela classe
  sel_familiaop->definirObjeto(classe_op->obterFamilia());

  //Marca no formulário se a classe é considerada padrão
  classepadrao_chk->setChecked(classe_op->classePadrao());

  //Marca no combobox o tipo de indexão usada pela classe
  tipo_index_cmb->setCurrentIndex(tipo_index_cmb->findText(~(classe_op->obterTipoIndexacao())));

  //Exibe na tabela os elementos configuradas para a classe de operadores
  tab_elementos->blockSignals(true);
  qtd=classe_op->obterNumElemClasseOperadores();
  for(i=0; i < qtd; i++)
  {
   tab_elementos->adicionarLinha();
   exibirDadosElemento(classe_op->obterElementoClasse(i), i);
  }
  tab_elementos->blockSignals(false);
  tab_elementos->limparSelecao();
 }

 //Exibe no widget de tipos pgsql, o tipo de dados que a classe opera
 tipo_dado->definirAtributos(tipo, modelo);
}
//----------------------------------------------------------
void ClasseOperadoresWidget::aplicarConfiguracao(void)
{
 try
 {
  ClasseOperadores *classe_op=NULL;
  unsigned i, qtd;

  iniciarConfiguracao<ClasseOperadores>();

  //Obtém a referência à classe de objetos que está sendo manipulada
  classe_op=dynamic_cast<ClasseOperadores *>(this->objeto);

  //Atribui os valores configurados no formulário à classe de operadores
  classe_op->definirPadrao(classe_op->classePadrao());
  classe_op->definirFamilia(dynamic_cast<FamiliaOperadores *>(sel_familiaop->obterObjeto()));
  classe_op->definirTipoIndexacao(TipoIndexacao(tipo_index_cmb->currentText()));
  classe_op->definirTipoDado(tipo_dado->obterTipoPgSQL());

  /* Remove todos os elementos da classe de operadores e em seguida
     insere aqueles configurados na tabela à classe */
  classe_op->removerElementosClasse();
  qtd=tab_elementos->obterNumLinhas();

  for(i=0; i < qtd; i++)
   classe_op->adicionarElementoClasse(tab_elementos->obterDadoLinha(i).value<ElemClasseOperadores>());

  //Aplica e finaliza a configuração da classe de operadores
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
