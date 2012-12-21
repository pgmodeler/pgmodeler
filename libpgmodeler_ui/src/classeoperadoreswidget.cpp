#include "classeoperadoreswidget.h"

ClasseOperadoresWidget::ClasseOperadoresWidget(QWidget *parent): ObjetoBaseWidget(parent, OBJ_OPCLASS)
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

  sel_familiaop=new SeletorObjetoWidget(OBJ_OPFAMILY, false, this);
  sel_operador=new SeletorObjetoWidget(OBJ_OPERATOR, true, this);
  sel_funcao=new SeletorObjetoWidget(OBJ_FUNCTION, true, this);
  tipo_dado=new TipoPgSQLWidget(this);
  tipo_armazenamento=new TipoPgSQLWidget(this, trUtf8("Storage Type"));
  tab_elementos=new TabelaObjetosWidget(TabelaObjetosWidget::TODOS_BOTOES, true, this);

  /* A tabela de elementos é constituída de 4 colunas:
     Nome do objeto, tipo, nº de suporte/estratégia e rechecagem. */
  tab_elementos->definirNumColunas(4);
  tab_elementos->definirRotuloCabecalho(trUtf8("Object"),0);
  tab_elementos->definirIconeCabecalho(QPixmap(":/icones/icones/table.png"),0);

  tab_elementos->definirRotuloCabecalho(trUtf8("Type"),1);
  tab_elementos->definirIconeCabecalho(QPixmap(":/icones/icones/usertype.png"),1);

  tab_elementos->definirRotuloCabecalho(trUtf8("Support/Strategy"),2);
  tab_elementos->definirRotuloCabecalho(trUtf8("Recheck"),3);

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
  configurarLayouFormulario(grid, OBJ_OPCLASS);

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
  IndexingType::getTypes(tipos);
  tipo_index_cmb->addItems(tipos);
 }
 catch(Exception &e)
 {
  //Redireciona o erro
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}

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

void ClasseOperadoresWidget::selecionarTipoElemento(int tipo)
{
 unsigned tipo_sel=static_cast<unsigned>(tipo);

 //Exibe os campos do elemento conforme o tipo selecionado

 //Campos exclusivos para o tipo FUNÇÃO
 funcao_lbl->setVisible(tipo_sel==OperatorClassElement::FUNCTION_ELEM);
 sel_funcao->setVisible(tipo_sel==OperatorClassElement::FUNCTION_ELEM);

 //Campos exclusivos para o tipo OPERADOR
 operador_lbl->setVisible(tipo_sel==OperatorClassElement::OPERATOR_ELEM);
 sel_operador->setVisible(tipo_sel==OperatorClassElement::OPERATOR_ELEM);
 rechecar_chk->setVisible(tipo_sel==OperatorClassElement::OPERATOR_ELEM);

 //Campos exclusivos para o tipo ARMAZENAMENTO
 tipo_armazenamento->setVisible(tipo_sel==OperatorClassElement::STORAGE_ELEM);

 //Campos comuns aos tipos FUNÇÃO e OPERADOR
 num_suporte_lbl->setVisible(tipo_sel!=OperatorClassElement::STORAGE_ELEM);
 num_suporte_sb->setVisible(tipo_sel!=OperatorClassElement::STORAGE_ELEM);
}

void ClasseOperadoresWidget::editarElemento(int idx_linha)
{
 OperatorClassElement elem;

 //Obtém o elemento da linha selecionada na tabela
 elem=tab_elementos->obterDadoLinha(idx_linha).value<OperatorClassElement>();

 //Preenche o formulário com atributos do elemento
 tipo_elem_cmb->setCurrentIndex(elem.getElementType());
 sel_funcao->definirObjeto(elem.getFunction());
 sel_operador->definirObjeto(elem.getOperator());
 rechecar_chk->setChecked(elem.isRecheck());
 num_suporte_sb->setValue(elem.getStrategyNumber());
 tipo_armazenamento->definirAtributos(elem.getStorage(),this->modelo);
}

void ClasseOperadoresWidget::exibirDadosElemento(OperatorClassElement elem, int idx_linha)
{
 unsigned tipo_elem;

 //Obtém o tipo do elemento
 tipo_elem=elem.getElementType();

 /* Para o tipo FUNÇÃO são exibidas na coluna 0 e 1, respectivamente,
    a assinatura da função e o tipo do objeto (função) */
 if(tipo_elem==OperatorClassElement::FUNCTION_ELEM)
 {
  tab_elementos->definirTextoCelula(QString::fromUtf8(elem.getFunction()->getSignature()), idx_linha, 0);
  tab_elementos->definirTextoCelula(QString::fromUtf8(elem.getFunction()->getTypeName()), idx_linha, 1);
 }
 /* Para o tipo OPERADOR são exibidas na coluna 0 e 1, respectivamente,
    a assinatura do operador e o tipo do objeto (operador) */
 else if(tipo_elem==OperatorClassElement::OPERATOR_ELEM)
 {
  tab_elementos->definirTextoCelula(QString::fromUtf8(elem.getOperator()->getSignature()), idx_linha, 0);
  tab_elementos->definirTextoCelula(QString::fromUtf8(elem.getOperator()->getTypeName()), idx_linha, 1);
 }
  /* Para o tipo ARMAZENAMENTO são exibidas na coluna 0 e 1, respectivamente,
    a nome completo do tipo e o tipo do objeto (tipo) */
 else
 {
  tab_elementos->definirTextoCelula(*elem.getStorage(), idx_linha, 0);
  tab_elementos->definirTextoCelula(QString::fromUtf8(BaseObject::getTypeName(OBJ_TYPE)), idx_linha, 1);
 }

 /* Para os tipos FUNÇÃO e OPERADOR é na coluna 2
    o número de suporte/estratégia da função ou operador */
 if(tipo_elem!=OperatorClassElement::STORAGE_ELEM)
  tab_elementos->definirTextoCelula(QString("%1").arg(elem.getStrategyNumber()), idx_linha, 2);
 /* Para o tipo ARMAZENAMENTO este campo não é preenchido pois não aplica
    esta categoria de elemento */
 else
  tab_elementos->definirTextoCelula("-", idx_linha, 2);

 /* Para o tipo OPERADOR na coluna 3 é exibido se o operador
    necessita ser rechecado ou não */
 if(tipo_elem==OperatorClassElement::OPERATOR_ELEM)
 {
  if(elem.isRecheck())
   tab_elementos->definirTextoCelula(trUtf8("Yes"), idx_linha, 3);
  else
   tab_elementos->definirTextoCelula(trUtf8("No"), idx_linha, 3);
 }
 /* Para os tipos FUNÇÃO e ARMAZENAMENTO este campo não é preenchido
    pois não aplica esta categorias de elementos */
 else
  tab_elementos->definirTextoCelula("-", idx_linha, 3);

 //Define o dado da linha como sendo o elemento fornecido no parâmetro.
 tab_elementos->definirDadoLinha(QVariant::fromValue<OperatorClassElement>(elem), idx_linha);
}

void ClasseOperadoresWidget::manipularElemento(int idx_linha)
{
 OperatorClassElement elem;
 unsigned tipo_elem;

 //Obtém o tipo do elemento
 tipo_elem=tipo_elem_cmb->currentIndex();

 try
 {
  /* Caso seja do tipo FUNÇÃO, atribui a função selecionado no widget seletor de função
     e também o valor configurado como suporte no formulário */
  if(tipo_elem==OperatorClassElement::FUNCTION_ELEM)
   elem.setFunction(dynamic_cast<Function *>(sel_funcao->obterObjeto()), num_suporte_sb->value());
  /* Caso seja do tipo OPERADOR, atribui o operador selecionado no widget seletor de operador,
     o valor configurado como suporte no formulário e o estado do checkbox de rechecagem */
  else  if(tipo_elem==OperatorClassElement::OPERATOR_ELEM)
   elem.setOperator(dynamic_cast<Operator *>(sel_operador->obterObjeto()), num_suporte_sb->value(), rechecar_chk->isChecked());
  //Caso seja do tipo ARMAZENAMENTO, atribui o tipo configurado no widget de configuração de tipo
  else
   elem.setStorage(tipo_armazenamento->obterTipoPgSQL());

  //Exibe o elemento configurado na tabela
  exibirDadosElemento(elem, idx_linha);

  //Reinicia o formulário de configuração de elementos
  sel_funcao->removerObjetoSelecionado();
  sel_operador->removerObjetoSelecionado();
  num_suporte_sb->setValue(1);
  rechecar_chk->setChecked(false);
  tab_elementos->limparSelecao();
 }
 catch(Exception &e)
 {
  /* Em caso de erro e caso a linha atual da tabela esteja sem dados (algum campo sem texto)
     indica que a operação de inserção de um elemento é que gerou a exceção e para tanto
     a linha recém adicionada precisa ser removida */
  if(tab_elementos->obterTextoCelula(idx_linha, 0).isEmpty())
   tab_elementos->removerLinha(idx_linha);

  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}

void ClasseOperadoresWidget::definirAtributos(DatabaseModel *modelo, OperationList *lista_op, OperatorClass *classe_op)
{
 PgSQLType tipo;
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
  tipo=classe_op->getDataType();

  //Exibe no formulário a família de operadores usada pela classe
  sel_familiaop->definirObjeto(classe_op->getFamily());

  //Marca no formulário se a classe é considerada padrão
  classepadrao_chk->setChecked(classe_op->isDefault());

  //Marca no combobox o tipo de indexão usada pela classe
  tipo_index_cmb->setCurrentIndex(tipo_index_cmb->findText(~(classe_op->getIndexingType())));

  //Exibe na tabela os elementos configuradas para a classe de operadores
  tab_elementos->blockSignals(true);
  qtd=classe_op->getElementCount();
  for(i=0; i < qtd; i++)
  {
   tab_elementos->adicionarLinha();
   exibirDadosElemento(classe_op->getElement(i), i);
  }
  tab_elementos->blockSignals(false);
  tab_elementos->limparSelecao();
 }

 //Exibe no widget de tipos pgsql, o tipo de dados que a classe opera
 tipo_dado->definirAtributos(tipo, modelo);
}

void ClasseOperadoresWidget::aplicarConfiguracao(void)
{
 try
 {
  OperatorClass *classe_op=NULL;
  unsigned i, qtd;

  iniciarConfiguracao<OperatorClass>();

  //Obtém a referência    classe de objetos que está sendo manipulada
  classe_op=dynamic_cast<OperatorClass *>(this->objeto);

  //Atribui os valores configurados no formulário    classe de operadores
  classe_op->setDefault(classe_op->isDefault());
  classe_op->setFamily(dynamic_cast<OperatorFamily *>(sel_familiaop->obterObjeto()));
  classe_op->setIndexingType(IndexingType(tipo_index_cmb->currentText()));
  classe_op->setDataType(tipo_dado->obterTipoPgSQL());

  /* Remove todos os elementos da classe de operadores e em seguida
     insere aqueles configurados na tabela    classe */
  classe_op->removeElements();
  qtd=tab_elementos->obterNumLinhas();

  for(i=0; i < qtd; i++)
   classe_op->addElement(tab_elementos->obterDadoLinha(i).value<OperatorClassElement>());

  //Aplica e finaliza a configuração da classe de operadores
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

