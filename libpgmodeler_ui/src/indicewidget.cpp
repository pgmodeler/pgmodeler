#include "indicewidget.h"

IndiceWidget::IndiceWidget(QWidget *parent): ObjetoBaseWidget(parent, OBJ_INDEX)
{
 try
 {
  map<QString, vector<QWidget *> > mapa_campos;
  map<QWidget *, vector<QString> > mapa_valores;
  QFrame *frame=NULL;
  QStringList lista;
  QGridLayout *grid=NULL;

  Ui_IndiceWidget::setupUi(this);

  //Cria um destacador de sintaxe no campo de expressão de checagem
  dest_exp_condicional=new DestaqueSintaxe(exp_condicional_txt, false);
  dest_exp_condicional->carregarConfiguracao(GlobalAttributes::CONFIGURATIONS_DIR +
                                     GlobalAttributes::DIR_SEPARATOR +
                                     GlobalAttributes::SQL_HIGHLIGHT_CONF +
                                     GlobalAttributes::CONFIGURATION_EXT);

  dest_exp_elemento=new DestaqueSintaxe(exp_elemento_txt, false);
  dest_exp_elemento->carregarConfiguracao(GlobalAttributes::CONFIGURATIONS_DIR +
                                     GlobalAttributes::DIR_SEPARATOR +
                                     GlobalAttributes::SQL_HIGHLIGHT_CONF +
                                     GlobalAttributes::CONFIGURATION_EXT);

  tab_elementos=new TabelaObjetosWidget(TabelaObjetosWidget::TODOS_BOTOES, true, this);
  sel_classe_op=new SeletorObjetoWidget(OBJ_OPCLASS, true, this);

  tab_elementos->definirNumColunas(5);
  tab_elementos->definirRotuloCabecalho(trUtf8("Element"), 0);
  tab_elementos->definirIconeCabecalho(QPixmap(":/icones/icones/column.png"),0);
  tab_elementos->definirRotuloCabecalho(trUtf8("Type"), 1);
  tab_elementos->definirIconeCabecalho(QPixmap(":/icones/icones/usertype.png"),1);
  tab_elementos->definirRotuloCabecalho(trUtf8("Operator Class"), 2);
  tab_elementos->definirIconeCabecalho(QPixmap(":/icones/icones/opclass.png"),2);
  tab_elementos->definirRotuloCabecalho(trUtf8("Sorting"), 3);
  tab_elementos->definirRotuloCabecalho(trUtf8("Nulls First"), 4);

  grid=dynamic_cast<QGridLayout *>(elementos_grp->layout());
  grid->addWidget(sel_classe_op, 2,1,1,3);
  grid->addWidget(tab_elementos, 5,0,1,4);

  configurarLayouFormulario(indice_grid, OBJ_INDEX);
  janela_pai->setMinimumSize(600, 640);

  IndexingType::getTypes(lista);
  tipo_index_cmb->addItems(lista);

  //Define os campos exclusivos para cada versão
  mapa_campos[gerarIntervaloVersoes(ATE_VERSAO, SchemaParser::PGSQL_VERSION_81)].push_back(tipo_index_lbl);
  mapa_campos[gerarIntervaloVersoes(APOS_VERSAO, SchemaParser::PGSQL_VERSION_82)].push_back(concorrente_chk);
  mapa_campos[gerarIntervaloVersoes(APOS_VERSAO, SchemaParser::PGSQL_VERSION_83)].push_back(ordenacao_lbl);
  mapa_campos[gerarIntervaloVersoes(APOS_VERSAO, SchemaParser::PGSQL_VERSION_84)].push_back(atual_rapida_chk);
  mapa_valores[tipo_index_lbl].push_back(~IndexingType(IndexingType::rtree));

  //Gera o frame de alerta
  frame=gerarFrameAlertaVersao(mapa_campos, &mapa_valores);
  indice_grid->addWidget(frame, indice_grid->count()+1, 0, 1, 0);
  frame->setParent(this);

  connect(janela_pai->aplicar_ok_btn,SIGNAL(clicked(bool)), this, SLOT(aplicarConfiguracao(void)));
  connect(tab_elementos, SIGNAL(s_linhaAdicionada(int)), this, SLOT(manipularElemento(int)));
  connect(tab_elementos, SIGNAL(s_linhaAtualizada(int)), this, SLOT(manipularElemento(int)));
  connect(tab_elementos, SIGNAL(s_linhaEditada(int)), this, SLOT(editarElemento(int)));
  connect(coluna_rb, SIGNAL(toggled(bool)), this, SLOT(selecionarObjetoElemento(void)));
  connect(expressao_rb, SIGNAL(toggled(bool)), this, SLOT(selecionarObjetoElemento(void)));
 }
 catch(Exception &e)
 {
  //Redireciona o erro
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}

void IndiceWidget::hideEvent(QHideEvent *evento)
{
 ObjetoBaseWidget::hideEvent(evento);

 exp_condicional_txt->clear();
 coluna_cmb->clear();
 concorrente_chk->setChecked(false);
 unico_chk->setChecked(false);
 tipo_index_cmb->setCurrentIndex(0);
 fator_preenc_sb->setValue(fator_preenc_sb->minimum());

 tab_elementos->blockSignals(true);
 tab_elementos->removerLinhas();
 tab_elementos->blockSignals(false);

 sel_classe_op->removerObjetoSelecionado();
 exp_elemento_txt->clear();
 ascendente_rb->setChecked(true);
 coluna_rb->setChecked(true);
}

void IndiceWidget::atualizarComboColunas(void)
{
 Column *coluna=NULL;
 unsigned i, qtd_col=0;

 try
 {
  coluna_cmb->clear();

  /* Varre a lista de colunas da tabela inserindo-as
     no combo box do formulário */
  qtd_col=tabela->getColumnCount();
  for(i=0; i < qtd_col; i++)
  {
   coluna=tabela->getColumn(i);
   coluna_cmb->addItem(QString::fromUtf8(coluna->getName()),
                       QVariant::fromValue<void *>(coluna));
  }
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}

void IndiceWidget::exibirDadosElemento(IndexElement elem, int idx_elem)
{
 //Caso o elemento possua uma coluna
 if(elem.getColumn())
 {
  //Exibe os dados da coluna
  tab_elementos->definirTextoCelula(QString::fromUtf8(elem.getColumn()->getName()), idx_elem, 0);
  tab_elementos->definirTextoCelula(QString::fromUtf8(elem.getColumn()->getTypeName()), idx_elem, 1);
 }
 //Caso possua uma expressão
 else
 {
  //Exibe a expressão
  tab_elementos->definirTextoCelula(QString::fromUtf8(elem.getExpression()), idx_elem, 0);
  tab_elementos->definirTextoCelula(trUtf8("Expressão"), idx_elem, 1);
 }

 /* Exibindo o nome da classe de operadores caso exista.
    se não existir exibe um '-' */
 if(elem.getOperatorClass())
  tab_elementos->definirTextoCelula(QString::fromUtf8(elem.getOperatorClass()->getName(true)), idx_elem, 2);
 else
  tab_elementos->definirTextoCelula("-", idx_elem, 2);

 if(elem.getSortAttribute(IndexElement::ASC_ORDER))
  tab_elementos->definirTextoCelula(ascendente_rb->text(), idx_elem, 3);
 else
  tab_elementos->definirTextoCelula(descendente_rb->text(), idx_elem, 3);

 if(elem.getSortAttribute(IndexElement::NULLS_FIRST))
  tab_elementos->definirTextoCelula(trUtf8("Sim"), idx_elem, 4);
 else
  tab_elementos->definirTextoCelula(trUtf8("Não"), idx_elem, 4);

 /* O elemento é definido como dado da linha para facilitar a configuração
    no método aplicarConfiguracao() */
 tab_elementos->definirDadoLinha(QVariant::fromValue<IndexElement>(elem), idx_elem);
}

void IndiceWidget::manipularElemento(int idx_elem)
{
 /* Caso a radio box de coluna esteja marcado ou se o de expressão esteja
    marcado e a expressão esteja preenchida */
 if(coluna_rb->isChecked() ||
   (expressao_rb->isChecked() && !exp_elemento_txt->toPlainText().isEmpty()))
 {
  IndexElement elem;

  //Configura um elemento com todos os dados configurados no formulário d elementos
  elem.setSortAttribute(IndexElement::NULLS_FIRST, nulos_primeiro_chk->isChecked());
  elem.setSortAttribute(IndexElement::ASC_ORDER, ascendente_rb->isChecked());
  elem.setOperatorClass(dynamic_cast<OperatorClass *>(sel_classe_op->obterObjeto()));

  if(expressao_rb->isChecked())
   elem.setExpression(exp_elemento_txt->toPlainText());
  else
   elem.setColumn(reinterpret_cast<Column *>(coluna_cmb->itemData(coluna_cmb->currentIndex()).value<void *>()));

  //Exibe os dados do elemento na tabela de elementos do índice
  exibirDadosElemento(elem, idx_elem);

  //Limpa o formulário de elementos para posteriores inserções
  exp_elemento_txt->clear();
  ascendente_rb->setChecked(true);
  sel_classe_op->removerObjetoSelecionado();
  nulos_primeiro_chk->setChecked(false);
 }
 else if(tab_elementos->obterTextoCelula(idx_elem,0).isEmpty())
  tab_elementos->removerLinha(idx_elem);
}

void IndiceWidget::editarElemento(int idx_elem)
{
 IndexElement elem;

 //Obtém o elemento na linha especificada
 elem=tab_elementos->obterDadoLinha(idx_elem).value<IndexElement>();

 /* Caso possua coluna marca o radio box de coluna
    e marca no combo box o nome da coluna que o elemento possui */
 if(elem.getColumn())
 {
  coluna_rb->setChecked(true);
  coluna_cmb->setCurrentIndex(coluna_cmb->findText(QString::fromUtf8(elem.getColumn()->getName())));
 }
 /* Caso seja uma expressão marca o radiobox de expressão e a
    insere no campo de expressão */
 else
 {
  expressao_rb->setChecked(true);
  exp_elemento_txt->setPlainText(elem.getExpression());
 }

 //Configura a ordenação do formulário conforme configurado no elemento
 if(elem.getSortAttribute(IndexElement::ASC_ORDER))
  ascendente_rb->setChecked(true);
 else
  descendente_rb->setChecked(true);
 nulos_primeiro_chk->setChecked(elem.getSortAttribute(IndexElement::NULLS_FIRST));

 //Exibe a classe de operadores no formulário
 sel_classe_op->definirObjeto(elem.getOperatorClass());
}

void IndiceWidget::selecionarObjetoElemento(void)
{
 QObject *obj_sender=sender();

 coluna_rb->blockSignals(true);
 expressao_rb->blockSignals(true);

 /* Caso a radiobox de coluna seja o sender
    desmarca o radio de expressão e limpa e
    desabilita o campo de expressão. */
 if(obj_sender==coluna_rb)
 {
  exp_elemento_txt->clear();
  coluna_cmb->setEnabled(true);
  expressao_rb->setChecked(false);
  coluna_rb->setChecked(true);
  exp_elemento_txt->setEnabled(false);
 }
 /* Caso o sender seja o radio de expressão
    ativa o campo de expressão e desmarca o
    radio de coluna */
 else
 {
  coluna_cmb->setEnabled(false);
  coluna_rb->setChecked(false);
  expressao_rb->setChecked(true);
  exp_elemento_txt->setEnabled(true);
 }

 coluna_rb->blockSignals(false);
 expressao_rb->blockSignals(false);
}

void IndiceWidget::definirAtributos(DatabaseModel *modelo, Table *tabela_pai, OperationList *lista_op, Index *indice)
{
 unsigned i, qtd;

 if(!tabela_pai)
  throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

 //Define os atributos do formulários e da janela pai
 ObjetoBaseWidget::definirAtributos(modelo, lista_op, indice, tabela_pai);

 //Define o modelo de banco de dados do seletor de classe de operadores
 sel_classe_op->definirModelo(modelo);

 //Exibe o nome das colunas da tabela no combobox de colunas dos elementos
 atualizarComboColunas();

 //Caso o índice esteja alocado (sendo editado)
 if(indice)
 {
  /* Configura os campos do formulário com os respectivos valores
     configurados no índice */
  tipo_index_cmb->setCurrentIndex(tipo_index_cmb->findText(~indice->getIndexingType()));
  fator_preenc_sb->setValue(indice->getFillFactor());
  concorrente_chk->setChecked(indice->getIndexAttribute(Index::CONCURRENT));
  atual_rapida_chk->setChecked(indice->getIndexAttribute(Index::FAST_UPDATE));
  unico_chk->setChecked(indice->getIndexAttribute(Index::UNIQUE));
  exp_condicional_txt->setPlainText(indice->getConditionalExpression());

  //Exibe os elementos do índice na tabela
  tab_elementos->blockSignals(true);
  qtd=indice->getElementCount();
  for(i=0; i < qtd; i++)
  {
   tab_elementos->adicionarLinha();
   exibirDadosElemento(indice->getElement(i), i);
  }
  tab_elementos->blockSignals(false);
 }
}

void IndiceWidget::aplicarConfiguracao(void)
{
 try
 {
  Index *indice=NULL;
  unsigned i, qtd;
  IndexElement elem;

  iniciarConfiguracao<Index>();

  //Obtém a referência ao índice que está sendo criado/editado
  indice=dynamic_cast<Index *>(this->objeto);

  //Configura no índice os valores preenchidos no formulário
  indice->setIndexAttribute(Index::FAST_UPDATE, atual_rapida_chk->isChecked());
  indice->setIndexAttribute(Index::CONCURRENT, concorrente_chk->isChecked());
  indice->setIndexAttribute(Index::UNIQUE, unico_chk->isChecked());
  indice->setConditionalExpression(exp_condicional_txt->toPlainText());
  indice->setIndexingType(IndexingType(tipo_index_cmb->currentText()));
  indice->setFillFactor(fator_preenc_sb->value());

  //Insere os elementos da tabela no índice
  indice->removeElements();
  qtd=tab_elementos->obterNumLinhas();

  for(i=0; i < qtd; i++)
  {
   //Obtém o elemento na linha da tabela
   elem=tab_elementos->obterDadoLinha(i).value<IndexElement>();

   //Caso o mesmo possua uma coluna
   if(elem.getColumn())
    //Adiciona um elmento como sendo um com coluna
    indice->addElement(elem.getColumn(), elem.getOperatorClass(),
                              elem.getSortAttribute(IndexElement::ASC_ORDER),
                              elem.getSortAttribute(IndexElement::NULLS_FIRST));
   else
    //Adiciona um elmento como sendo um com expressão
    indice->addElement(elem.getExpression(), elem.getOperatorClass(),
                              elem.getSortAttribute(IndexElement::ASC_ORDER),
                              elem.getSortAttribute(IndexElement::NULLS_FIRST));
  }

  //Aplica as configurações básicas
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

