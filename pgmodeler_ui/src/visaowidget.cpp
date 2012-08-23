#include "visaowidget.h"
//***********************************************************
VisaoWidget::VisaoWidget(QWidget *parent): ObjetoBaseWidget(parent, OBJETO_VISAO)
{
 try
 {
  Ui_VisaoWidget::setupUi(this);
  //QGridLayout *grid=NULL;

  //Cria um destacador de sintaxe no campo de expressão e código fonte
  destaque_expr=NULL;
  destaque_expr=new DestaqueSintaxe(expressao_txt, false);
  destaque_expr->carregarConfiguracao(AtributosGlobais::DIR_CONFIGURACOES +
                                     AtributosGlobais::SEP_DIRETORIO +
                                     AtributosGlobais::CONF_DESTAQUE_SQL +
                                     AtributosGlobais::EXT_CONFIGURACAO);

  destaque_codigo=NULL;
  destaque_codigo=new DestaqueSintaxe(codigo_txt, false);
  destaque_codigo->carregarConfiguracao(AtributosGlobais::DIR_CONFIGURACOES +
                                     AtributosGlobais::SEP_DIRETORIO +
                                     AtributosGlobais::CONF_DESTAQUE_SQL +
                                     AtributosGlobais::EXT_CONFIGURACAO);

  //Alocando os seletores de objetos (tabela e coluna) que são atribuídos às referências da visão
  sel_tabela=NULL;
  sel_tabela=new SeletorObjetoWidget(OBJETO_TABELA, true, this);
  sel_coluna=NULL;
  sel_coluna=new SeletorObjetoWidget(OBJETO_COLUNA, true, this);

  //Alocando a tabela que armazena todas as referências da visão
  tab_referencias=new TabelaObjetosWidget(TabelaObjetosWidget::TODOS_BOTOES, true, this);
  tab_referencias->definirNumColunas(4);
  tab_referencias->definirRotuloCabecalho(trUtf8("Col./Expr."),0);
  tab_referencias->definirRotuloCabecalho(trUtf8("Alias"),1);
  tab_referencias->definirRotuloCabecalho(trUtf8("Alias Col."),2);
  tab_referencias->definirRotuloCabecalho(trUtf8("SF FW AW"),3);

  //Gera o frame de informação sobre a referência a todas as colunas da tabela
  frame_info=gerarFrameInformacao(trUtf8("Para se referenciar todas as colunas de uma tabela (*) basta não preencher\
                                     o campo <strong>Coluna</strong>, isso é equivalente a se \
                                     escrever <em><strong>[esquema].[tabela].*</strong></em>"));

  //grid=dynamic_cast<QGridLayout *>(referencias_gb->layout());
  referencias_grid->addWidget(sel_tabela, 2,1,1,2);
  referencias_grid->addWidget(sel_coluna, 3,1,1,2);
  referencias_grid->addWidget(frame_info, 6, 0, 1, 0);
  referencias_grid->addWidget(tab_referencias, 7,0,2,0);

  configurarLayouFormulario(visao_grid, OBJETO_VISAO);

  connect(janela_pai->aplicar_ok_btn,SIGNAL(clicked(bool)), this, SLOT(aplicarConfiguracao(void)));
  connect(tipo_ref_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(selecionarTipoReferencia(void)));

  connect(sel_coluna, SIGNAL(s_objetoSelecionado(void)), this, SLOT(exibirNomeObjeto(void)));
  connect(sel_coluna, SIGNAL(s_objetoRemovido(void)), this, SLOT(exibirNomeObjeto(void)));
  connect(sel_tabela, SIGNAL(s_objetoSelecionado(void)), this, SLOT(exibirNomeObjeto(void)));

  connect(tab_referencias, SIGNAL(s_linhaAdicionada(int)), this, SLOT(manipularReferencia(int)));
  connect(tab_referencias, SIGNAL(s_linhaAtualizada(int)), this, SLOT(manipularReferencia(int)));
  connect(tab_referencias, SIGNAL(s_linhaEditada(int)), this, SLOT(editarReferencia(int)));

  connect(tab_referencias, SIGNAL(s_linhasMovidas(int,int)), this, SLOT(atualizarPrevisaoCodigo(void)));
  connect(tab_referencias, SIGNAL(s_linhasRemovidas(void)), this, SLOT(atualizarPrevisaoCodigo(void)));
  connect(tab_referencias, SIGNAL(s_linhaRemovida(int)), this, SLOT(atualizarPrevisaoCodigo(void)));

  janela_pai->setMinimumSize(650, 630);
  selecionarTipoReferencia();
 }
 catch(Excecao &e)
 {
  //Redireciona o erro
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}
//----------------------------------------------------------
void VisaoWidget::hideEvent(QHideEvent *evento)
{
 tab_referencias->removerLinhas();
 ObjetoBaseWidget::hideEvent(evento);
}
//----------------------------------------------------------
void VisaoWidget::limparFormReferencia(void)
{
 sel_coluna->removerObjetoSelecionado();
 sel_tabela->removerObjetoSelecionado();
 alias_col_edt->clear();
 alias_exp_edt->clear();
 alias_tab_edt->clear();
 expressao_txt->clear();
 select_from_chk->setChecked(false);
 from_where_chk->setChecked(false);
 apos_where_chk->setChecked(false);
}
//----------------------------------------------------------
void VisaoWidget::selecionarTipoReferencia(void)
{
 /* Marca na variável 'ref_obj' se o índice do combo de tipo de referência
    se trata de uma referência a uma coluna */
 bool ref_obj=(tipo_ref_cmb->currentIndex()==static_cast<int>(Referencia::REFER_COLUNA));

 //Exibe todos os campos do formulário referentes à referência de coluna
 tabela_lbl->setVisible(ref_obj);
 coluna_lbl->setVisible(ref_obj);
 sel_tabela->setVisible(ref_obj);
 sel_coluna->setVisible(ref_obj);
 alias_col_lbl->setVisible(ref_obj);
 alias_col_edt->setVisible(ref_obj);
 alias_tab_edt->setVisible(ref_obj);
 alias_tab_lbl->setVisible(ref_obj);
 frame_info->setVisible(ref_obj);

 /* Esconde todos os objetos que não são relacionados a refência a coluna
    e sim a expressão */
 expressao_lbl->setVisible(!ref_obj);
 expressao_txt->setVisible(!ref_obj);
 alias_exp_edt->setVisible(!ref_obj);
 alias_exp_lbl->setVisible(!ref_obj);
}
//----------------------------------------------------------
void VisaoWidget::manipularReferencia(int idx_ref)
{
 try
 {
  Referencia ref;

  /* Se o combo de tipo de referência estiver selecionado como referência a uma coluna
     cria uma referência como tal */
  if(static_cast<unsigned>(tipo_ref_cmb->currentIndex())==Referencia::REFER_COLUNA)
  {
   /* Chama o método de construtor de referência informando os parâmetros necessários
      para relacioná-la a uma coluna de tabela */
   ref=Referencia(dynamic_cast<Tabela *>(sel_tabela->obterObjeto()),
                  dynamic_cast<Coluna *>(sel_coluna->obterObjeto()),
                  alias_tab_edt->text(), alias_col_edt->text());
  }
  /* Se o combo de tipo de referência estiver selecionado como referência a uma expressão
     cria uma referência como tal */
  else
  {
   //Chama o método de construção de uma referência a uma expressão
   ref=Referencia(expressao_txt->toPlainText(), alias_tab_edt->text());
  }

  /* Obrigatoriamente, a referência deve possuir um aplicação SQL
     (ver método adicionarReferencia() da classe Visao). No formulário
     caso o usuário não marqui nenhuma aplição SQL, será disparado
     um erro e a criação da referência será abortada */
  if(!select_from_chk->isChecked() &&
     !from_where_chk->isChecked() &&
     !apos_where_chk->isChecked())
   throw Excecao(ERR_PGMODELERUI_TIPOSQLINDEFREFVISAO,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  //Exibe os dados da referência recém-criada na tabela de referências
  exibirDadosReferencia(ref, select_from_chk->isChecked(), from_where_chk->isChecked(),
                        apos_where_chk->isChecked(), idx_ref);

  //Limpa o formulário e a seleção de linha na tabela
  limparFormReferencia();
  tab_referencias->limparSelecao();
 }
 catch(Excecao &e)
 {
  /* Caso o método esteja no meio de uma inserção de nova referência,
     e um erro seja disparado, a nova linha da tabela precisa será
     removida pois não será inserida nenhuma referência */
  if(tab_referencias->obterTextoCelula(idx_ref, 0).isEmpty())
   //Remove a linha da tabela
   tab_referencias->removerLinha(idx_ref);
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}
//----------------------------------------------------------
void VisaoWidget::editarReferencia(int idx_ref)
{
 Referencia ref;
 QString str_aux;

 //Obtém a referência da linha especificada por 'idx_ref'
 ref=tab_referencias->obterDadoLinha(idx_ref).value<Referencia>();
 //Seleciona o tipo de referência no formulário de acordo com o tipo da referência obtida
 tipo_ref_cmb->setCurrentIndex(ref.obterTipoReferencia());

 //Caso seja uma referência a uma coluna
 if(ref.obterTipoReferencia()==Referencia::REFER_COLUNA)
 {
  /* Verifica se a referência está ligada a uma coluna específica
     ou a todas as colunas de uma tabela (* --> quando ref.obterColuna() == NULL) */
  if(ref.obterColuna())
   /* Define o objeto no seletor de coluna e automaticamente o seletor
      de tabelas é definido com o objeto pai da coluna */
   sel_coluna->definirObjeto(ref.obterColuna());
  else
   /* Caso a refência esteja ligada a todas a colunas da tabela (*)
      configura o seletor de tabelas com  a tabela usada na referência */
   sel_tabela->definirObjeto(ref.obterTabela());

  //Configura os campos de alias de tabela e coluna com os valores da referência
  alias_col_edt->setText(QString::fromUtf8(ref.obterAliasColuna()));
  alias_tab_edt->setText(QString::fromUtf8(ref.obterAlias()));
 }
 //Caso seja uma referência a uma expressão
 else
 {
  //Configura a caixa de texto de expressão com a expressão usada na referência
  expressao_txt->setPlainText(QString::fromUtf8(ref.obterExpressao()));
  //Configura o campo de alias de expressão com o valor presente na referência
  alias_exp_edt->setText(QString::fromUtf8(ref.obterAlias()));
 }

 /* Configura uma string de acordo com as aplicações SQL da referência.
    Quando uma dada aplicação está marcada a mesma será exibida como '1'
    caso contrário como '0'. O formato possível para esta strig será:

    [SELECT-FROM] [FROM-WHERE] [APÓS-WHERE]
        0|1            0|1         0|1      */
 str_aux=tab_referencias->obterTextoCelula(idx_ref,3);
 select_from_chk->setChecked(str_aux[0]=='1');
 from_where_chk->setChecked(str_aux[1]=='1');
 apos_where_chk->setChecked(str_aux[2]=='1');
}
//----------------------------------------------------------
void VisaoWidget::exibirNomeObjeto(void)
{
 Coluna *col=NULL;
 QObject *obj_sender=sender();

 /* Caso o objeto sender seja o seletor de tabela, isso indica
    que o usuário quer referenciar todas as colunas da tabela */
 if(obj_sender==sel_tabela)
 {
  /* Para isso, bloqueia os sinais do seletor de coluna pois qualquer
     alteração sem bloqueio de sinal pode causar a chamada indefinida
     a este método pois ambos os seletores estão conectados a este */
  sel_coluna->blockSignals(true);
  /* Define como NULL o objeto no seletor de coluna indicado
     que nenhuma coluna específica deve ser referenciada (*) */
  sel_coluna->removerObjetoSelecionado();
  //Reativa os sinais do seletor de coluna
  sel_coluna->blockSignals(false);
 }
 /* Caso o objeto sender seja o seletor de tabela, isso indica
    que o usuário quer referenciar uma coluna específica da tabela */
 else
 {
  //Obtém a coluna do seletor
  col=dynamic_cast<Coluna *>(sel_coluna->obterObjeto());

  /* Bloqueia os sinais do seletor de tabela pois qualquer
     alteração sem bloqueio de sinal pode causar a chamada indefinida
     a este método pois ambos os seletores estão conectados a este */
  sel_tabela->blockSignals(true);

  /* Caso a coluna esteja alocada, o seletor de tabela recebe automaticamente
     o nome da tabela pai desta coluna */
  if(col)
   sel_tabela->definirObjeto(col->obterTabelaPai());
  else
   sel_tabela->removerObjetoSelecionado();

  //Reativa os sinais do seletor de tabela
  sel_tabela->blockSignals(false);
 }
}
//----------------------------------------------------------
void VisaoWidget::exibirDadosReferencia(Referencia refer, bool selec_from, bool from_where, bool apos_where, unsigned idx_lin)
{
 Tabela *tab=NULL;
 Coluna *col=NULL;
 QString str_aux;

 //Caso a referência seja a uma coluna
 if(refer.obterTipoReferencia()==Referencia::REFER_COLUNA)
 {
  //Obtém a tabela e coluna referenciadas
  tab=refer.obterTabela();
  col=refer.obterColuna();

  /* Caso a tabela esteja alocada e a coluna não, indica que a referência
     será para todas as colunas (*), para isso exibe uma string
     no formatdo: [NOME_ESQUEMA].[NOME_TABELA].*  */
  if(tab && !col)
   tab_referencias->definirTextoCelula(QString::fromUtf8(tab->obterNome(true) + QString(".*")),idx_lin,0);
  /* Caso a tabela e coluna estejam alocadas indica que a referência
     será para a coluna em questão para isso exibe uma string
     no formatdo: [NOME_ESQUEMA].[NOME_TABELA].[NOME_COLUNA]  */
  else
   tab_referencias->definirTextoCelula(QString::fromUtf8(tab->obterNome(true) + QString(".") + col->obterNome(true)),idx_lin,0);

  //Exibe o alias da tabela e a exibe na segunda coluna da linha
  tab_referencias->definirTextoCelula(QString::fromUtf8(refer.obterAlias()),idx_lin,1);

  /* Caso a coluna esteja alocada, exibe o alias da mesma na terceira coluna da linha
     caso contrário exibe um '-' */
  if(col)
   tab_referencias->definirTextoCelula(QString::fromUtf8(refer.obterAliasColuna()),idx_lin,2);
  else
   tab_referencias->definirTextoCelula(QString("-"),idx_lin,2);
 }
 //Caso seja uma referência a uma expressão
 else
 {
  //Exibe a expressão na primeira coluna da linha
  tab_referencias->definirTextoCelula(QString::fromUtf8(refer.obterExpressao()),idx_lin,0);
  //Exibe o alias da expressão na segunda coluna da linha
  tab_referencias->definirTextoCelula(QString::fromUtf8(refer.obterAlias()),idx_lin,1);
  /* Exibe um '-' na terceira coluna que armazena o alias da coluna por este campo
     não se aplicar a uma expressão */
  tab_referencias->definirTextoCelula(QString("-"),idx_lin,2);
 }

 //Configura a string de aplicação SQL e exibe na quarta coluna
 str_aux+=(selec_from ? "1" : "0");
 str_aux+=(from_where ? "1" : "0");
 str_aux+=(apos_where ? "1" : "0");
 tab_referencias->definirTextoCelula(str_aux,idx_lin,3);

 //Define a referência obtida como dado da linha
 tab_referencias->definirDadoLinha(QVariant::fromValue<Referencia>(refer), idx_lin);

 /* Atualiza a previsão de código para exibir a nova referência
    no código SQL da visão */
 atualizarPrevisaoCodigo();
}
//----------------------------------------------------------
void VisaoWidget::atualizarPrevisaoCodigo(void)
{
 Referencia refer;
 QString str_aux;
 unsigned i, qtd, i1, tipo_exp[3]={Referencia::SQL_REFER_SELECT,
                                   Referencia::SQL_REFER_FROM,
                                   Referencia::SQL_REFER_WHERE};
 try
 {
  /* Remove todas as referências da visão auxiliar para inserção daquelas
    presente na tabela */
  visao_aux.removerReferencias();

  //Configura o nome da visão com o que está no formulário
  visao_aux.definirNome(nome_edt->text());

  //Configura o esquema da visão com o que está no formulário
  visao_aux.definirEsquema(sel_esquema->obterObjeto());

  /* Insere as referências da tabela na visão auxiliar
     porém estas são inseridas conforme a string de
     aplicação sql */
  qtd=tab_referencias->obterNumLinhas();
  for(i=0; i < qtd; i++)
  {
   //Obtém a referência da tabela
   refer=tab_referencias->obterDadoLinha(i).value<Referencia>();
   //Obtém a string de aplicação
   str_aux=tab_referencias->obterTextoCelula(i,3);

   //Varre a string de aplicação.
   for(i1=0; i1 < 3; i1++)
   {
    /* Caso a string na posição atual (SELECT-FROM|FROM-WHERE|Após WHERE]
       esteja marcada com um 1, a referência será inserida no final da
       lista de referências cuja aplicação seja a atual */
    if(str_aux[i1]=='1')
      visao_aux.adicionarReferencia(refer, tipo_exp[i1]);
   }
  }
  //Exibe o código fonte da visão auxliar, para refletir a configuração atual da mesma
  codigo_txt->setPlainText(QString::fromUtf8(visao_aux.obterDefinicaoObjeto(ParserEsquema::DEFINICAO_SQL)));
 }
 catch(Excecao &e)
 {
  /* Caso algum erro seja disparado durante a configuração da visão auxiliar
     exibe uma mensagem ao usuário no próprio campo de código fonte */
  codigo_txt->setPlainText(trUtf8("-- Impossível gerar código. Verifique se todos os atributos foram preenchidos corretamente! --"));
 }
}
//----------------------------------------------------------
void VisaoWidget::definirAtributos(ModeloBD *modelo, ListaOperacoes *lista_op, Visao *visao, float px, float py)
{
 unsigned i, qtd;
 bool sel_from, from_where, apos_where;
 Referencia refer;

 //Preenchendo os campos básicos do formulário com os atributos da visão
 ObjetoBaseWidget::definirAtributos(modelo,lista_op, visao, NULL, px, py);

 //Configurado o modelo de banco de dados referênciado pelos widget seletores
 sel_coluna->definirModelo(modelo);
 sel_tabela->definirModelo(modelo);

 //Caso a visão esteja alocada (sendo editada)
 if(visao)
 {
  //Obtém o número de referências da visão
  qtd=visao->obterNumReferencias();

  /* Bloqueia os sinais da tabela de referências para inserção de vários
     itens sem disparo de sinais */
  tab_referencias->blockSignals(true);
  for(i=0; i < qtd; i++)
  {
   tab_referencias->adicionarLinha();

   //Obtém a referência da visão
   refer=visao->obterReferencia(i);

   //Verifica qual a aplicação SQL da referência na visão
   sel_from=(visao->obterIndiceReferencia(refer,Referencia::SQL_REFER_SELECT) >= 0);
   from_where=(visao->obterIndiceReferencia(refer,Referencia::SQL_REFER_FROM) >= 0);
   apos_where=(visao->obterIndiceReferencia(refer,Referencia::SQL_REFER_WHERE)>= 0);

   //Exibe a referência na tabela
   exibirDadosReferencia(refer, sel_from, from_where, apos_where, i);
  }
  //Desbloqueia os sinais da tabela
  tab_referencias->blockSignals(false);
  //Limpa a seleção da tabela
  tab_referencias->limparSelecao();
 }
}
//---------------------------------------------------------
void VisaoWidget::aplicarConfiguracao(void)
{
 try
 {
  Visao *visao=NULL;

  iniciarConfiguracao<Visao>();

  //Obtém a referência à visao que está sendo editada/criada
  visao=dynamic_cast<Visao *>(this->objeto);

  //Faz a cópia da visão auxiliar para a visão que está sendo editada
  (*visao)=visao_aux;

  //Restaura a posição original da visão
  //visao->definirPosicaoObjeto(QPointF(this->px_objeto, this->py_objeto));

  //Finaliza a configuração da função de agregação
  ObjetoBaseWidget::aplicarConfiguracao();

  this->modelo->atualizarRelTabelaVisao(visao);
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
