#include "papelwidget.h"
#include "visaoobjetoswidget.h"
extern VisaoObjetosWidget *selecaoobjetos_wgt;
//***********************************************************
PapelWidget::PapelWidget(QWidget *parent): ObjetoBaseWidget(parent, OBJETO_PAPEL)
{
 TabelaObjetosWidget *tab_obj=NULL;
 QGridLayout *grid=NULL;
 unsigned i;

 Ui_PapelWidget::setupUi(this);
 configurarLayouFormulario(papel_grid, OBJETO_PAPEL);

 connect(janela_pai->aplicar_ok_btn,SIGNAL(clicked(bool)), this, SLOT(aplicarConfiguracao(void)));
 connect(membros_twg, SIGNAL(currentChanged(int)), this, SLOT(configurarSelecaoPapeis(void)));

 //Alocação e configuração das tabela de papéis membros
 for(i=0; i < 3; i++)
 {
  //Aloca uma tabela e armazena seu endereço no vetor
  tab_obj=new TabelaObjetosWidget(TabelaObjetosWidget::TODOS_BOTOES ^
                                  TabelaObjetosWidget::BTN_ATUALIZAR_ITEM, true, this);
  tab_membros[i]=tab_obj;

  /*
   Cada tabela de membros possui 6 colunas, sendo elas:
    * Nome do papel membro
    * Id do papel membro
    * Validade do papel membro
    * Lista de papéis os quais o papel membro faz parte
    * Membros do papel membro
    * Membros com privilégio de adm. do papel membro
  */
  tab_obj->definirNumColunas(6);

  tab_obj->definirRotuloCabecalho(trUtf8("Papel"),0);
  tab_obj->definirIconeCabecalho(QPixmap(":/icones/icones/role.png"),0);

  tab_obj->definirRotuloCabecalho(trUtf8("SysID"),1);
  tab_obj->definirIconeCabecalho(QPixmap(":/icones/icones/uid.png"),1);

  tab_obj->definirRotuloCabecalho(trUtf8("Validade"),2);
  tab_obj->definirIconeCabecalho(QPixmap(":/icones/icones/validade.png"),2);

  tab_obj->definirRotuloCabecalho(trUtf8("Membro de"),3);
  tab_obj->definirIconeCabecalho(QPixmap(":/icones/icones/role.png"),3);

  tab_obj->definirRotuloCabecalho(trUtf8("Membros"),4);
  tab_obj->definirIconeCabecalho(QPixmap(":/icones/icones/role.png"),4);

  tab_obj->definirRotuloCabecalho(trUtf8("Membros (Admin.)"),5);
  tab_obj->definirIconeCabecalho(QPixmap(":/icones/icones/role.png"),5);

  /* Cria um layout em grade adiciona a tabela alocada e a
     insere na tabwidget o qual comporta as tabelas */
  grid=new QGridLayout;
  grid->addWidget(tab_obj,0,0,1,1);
  grid->setContentsMargins(2,2,2,2);
  membros_twg->widget(i)->setLayout(grid);
 }

 janela_pai->setMinimumSize(500, 530);
}
//----------------------------------------------------------
void PapelWidget::configurarSelecaoPapeis(void)
{
 unsigned i;

 /* Desconecta todos os sinais-slots das tabelas de membros
    isso evita que o método de selecionar papel membro seja
    executado várias vezes por estar conectado às três
    instâncias das tabelas de membros */
 for(i=0; i < 3; i++)
  disconnect(tab_membros[i],0,this,0);

 /* Conecta os sinais enviados pela tabela atual para executar o
    método de seleção papel membro */
 connect(tab_membros[membros_twg->currentIndex()], SIGNAL(s_linhaAdicionada(int)), this, SLOT(selecionarPapelMembro(void)));
 connect(tab_membros[membros_twg->currentIndex()], SIGNAL(s_linhaEditada(int)), this, SLOT(selecionarPapelMembro(void)));
}
//----------------------------------------------------------
void PapelWidget::selecionarPapelMembro(void)
{
 selecaoobjetos_wgt->definirObjetoVisivel(OBJETO_PAPEL, true);
 selecaoobjetos_wgt->definirModelo(this->modelo);
 selecaoobjetos_wgt->show();
}
//----------------------------------------------------------
void PapelWidget::hideEvent(QHideEvent *evento)
{
 unsigned i;

 disconnect(selecaoobjetos_wgt,0,this,0);

 //Remove todas as linhas das tabelas de membros
 for(i=0; i < 3; i++)
  tab_membros[i]->blockSignals(true);
  //disconnect(tab_membros[i],0,this,0);

 for(i=0; i < 3; i++)
 {
  tab_membros[i]->removerLinhas();
  tab_membros[i]->blockSignals(false);
 }

 //Configura o tab widget de membros para exibir a primeira tabela
 membros_twg->setCurrentIndex(0);

 sysid_sb->setValue(sysid_sb->minValue());
 senha_edt->clear();
 limconexao_sb->setValue(limconexao_sb->minValue());
 superusr_chk->setChecked(false);
 herdarperm_chk->setChecked(false);
 criarbd_chk->setChecked(false);
 permitirlogin_chk->setChecked(false);
 criarusr_chk->setChecked(false);
 senhacripto_chk->setChecked(false);

 //Executa o método que trata o evento de esconder da classe superior
 ObjetoBaseWidget::hideEvent(evento);
}
//----------------------------------------------------------
void PapelWidget::definirAtributos(ModeloBD *modelo, ListaOperacoes *lista_op, Papel *papel)
{
 if(papel)
 {
  /* Preenche os widgets do formulário específicos de objetos papel
     com os valores presentes no objeto papel passado */
  sysid_sb->setValue(papel->obterSysid());
  limconexao_sb->setValue(papel->obterLimiteConexao());
  senha_edt->setText(papel->obterSenha());
  validade_dte->setDateTime(QDateTime::fromString(papel->obterValidade(),"yyyy-MM-dd hh:mm"));

  /* Marca as checkboxes de opção do papel de acordo com a configuração
     atual de tais opções no papel passado */
  superusr_chk->setChecked(papel->obterOpcao(Papel::OP_SUPERUSER));
  criarbd_chk->setChecked(papel->obterOpcao(Papel::OP_CREATEDB));
  criarusr_chk->setChecked(papel->obterOpcao(Papel::OP_CREATEROLE));
  senhacripto_chk->setChecked(papel->obterOpcao(Papel::OP_ENCRYPTED));
  herdarperm_chk->setChecked(papel->obterOpcao(Papel::OP_INHERIT));
  permitirlogin_chk->setChecked(papel->obterOpcao(Papel::OP_LOGIN));
 }

 //Define os atributos do formulários e da janela pai
 ObjetoBaseWidget::definirAtributos(modelo, lista_op, papel);

 //Preenche a tabela de membros do papel
 preencherTabelaMembros();

 connect(selecaoobjetos_wgt, SIGNAL(s_visibilityChanged(ObjetoBase*,bool)), this, SLOT(exibirDadosPapelSelecionado(void)));
 configurarSelecaoPapeis();
}
//----------------------------------------------------------
void PapelWidget::exibirDadosPapel(Papel *papel, unsigned idx_tabela, unsigned lin)
{
 if(papel)
 {
  QString str_aux;
  Papel *papel_aux=NULL;
  unsigned qtd, i, id_tipo,
           tipo_papeis[3]={ Papel::PAPEL_REF, Papel::PAPEL_MEMBRO, Papel::PAPEL_ADMIN };

  if(idx_tabela > 3)
   throw Excecao(ERR_PGMODELER_REFOBJIDXINV,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  /* Atribui como dado da linha a referência ao papel membro, para tanto é necessário
     converter a referência para void * pois é a única forma de ponteiro que
     a classe QVariant aceita */
  tab_membros[idx_tabela]->definirDadoLinha(QVariant::fromValue(reinterpret_cast<void *>(papel)), lin);

  //Configura as informações básicas do membro
  tab_membros[idx_tabela]->definirTextoCelula(QString::fromUtf8(papel->obterNome()), lin, 0);
  tab_membros[idx_tabela]->definirTextoCelula(QString("%1").arg(papel->obterSysid()), lin, 1);
  tab_membros[idx_tabela]->definirTextoCelula(papel->obterValidade(), lin, 2);

  /* Varre a lista de membros do papel obtido como membro atual
     concatena todos os nomes obtidos em uma string e a coloca
     na coluna respectiva ao tipo de membro auxiliar */
  for(id_tipo=0; id_tipo < 3; id_tipo++)
  {
   //Obtém a quantidade de tipos de membro atual do papel membro atual
   qtd=papel->obterNumPapeis(tipo_papeis[id_tipo]);

   //Varre a lista atual concatenando os nomes
   for(i=0; i < qtd; i++)
   {
    papel_aux=papel->obterPapel(tipo_papeis[id_tipo], i);
    str_aux+=papel_aux->obterNome();
    if(i < qtd-1) str_aux+=", ";
   }

   //Atribui a string configurada à coluna do tipo de membro do papel membro atual
   tab_membros[idx_tabela]->definirTextoCelula(QString::fromUtf8(str_aux), lin, 3 + id_tipo);
   str_aux.clear();
  }
 }
}
//----------------------------------------------------------
void PapelWidget::preencherTabelaMembros(void)
{
 //Caso exista um objeto a ser editado
 if(this->objeto)
 {
  QString str_aux;
  Papel *papel_aux=NULL, *papel=NULL;
  unsigned qtd, i, id_tipo,
           tipo_papeis[3]={ Papel::PAPEL_REF, Papel::PAPEL_MEMBRO, Papel::PAPEL_ADMIN };

  //Converte a referência do objeto a ser editado em um referência a um papel
  papel=dynamic_cast<Papel *>(this->objeto);

  /* O processo de preenchimento das tabelas se dá da seguinte forma:
      * Os membros do papel em cada tipo (PAPEL_REF, PAPEL_MEMBRO, PAPEL_ADMIN)
        são obtidos e colocados nas suas respectivas tabelas. Para isso a
        varredura é feita usando um for e com o auxílio do vetor 'tipo_papeis'.

      * Adicionalmente algumas informações importantes são obtidas desses membros
        como a data de validade da conta, o sysid, e os membros do papel membro
        que é adicionado na tabela */
  for(id_tipo=0; id_tipo < 3; id_tipo++)
  {
   //Obtém o número de papéis membros do tipo atual
   qtd=papel->obterNumPapeis(tipo_papeis[id_tipo]);
   tab_membros[id_tipo]->blockSignals(true);

   //Varre a lista de tipos atual
   for(i=0; i < qtd; i++)
   {
    //Obtém um papel da lista de membros
    papel_aux=papel->obterPapel(tipo_papeis[id_tipo], i);

    //Adiciona uma linha na tabela respectiva ao tipo de membro
    tab_membros[id_tipo]->adicionarLinha();

    //Exibe o dado do papel na linha atual (i)
    exibirDadosPapel(papel_aux, id_tipo, i);
   }

   tab_membros[id_tipo]->blockSignals(true);
   tab_membros[id_tipo]->limparSelecao();
  }
 }
}
//----------------------------------------------------------
void PapelWidget::exibirDadosPapelSelecionado(void)
{
 unsigned idx_tab;
 int lin, idx_lin=-1;
 ObjetoBase *obj_sel=NULL;

 //Obtém o papel selecionado na janela de seleção de objetos
 obj_sel=selecaoobjetos_wgt->obterObjetoSelecao();

 //Obtém o índice da tabela em foco atualmente
 idx_tab=membros_twg->currentIndex();

 //Obtém a linha selecionada da tabela atual
 lin=tab_membros[idx_tab]->obterLinhaSelecionada();

 /* Caso haja um objeto selecionado tenta obter o índice da linha
    a qual armazene como dado o objeto selecionado isso é feito
    para se validar se o usuário está tentando inserir na mesma
    tabela um objeto por mais de uma vez */
 if(obj_sel)
  idx_lin=tab_membros[idx_tab]->obterIndiceLinha(QVariant::fromValue<void *>(dynamic_cast<void *>(obj_sel)));

 /* Caso o objeto da seleção seja o mesmo do objeto sendo editado
    um erro será disparado pois o objeto não pode referenciar
    a ele mesmo */
 if(obj_sel && obj_sel==this->objeto)
 {
  throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELER_REFREDUNDANTEPAPEIS)
                               .arg(QString::fromUtf8(obj_sel->obterNome()))
                               .arg(QString::fromUtf8(nome_edt->text())),
                 ERR_PGMODELER_REFREDUNDANTEPAPEIS,__PRETTY_FUNCTION__,__FILE__,__LINE__);
 }
 //Se o objeto da seleção não existir na tabela exibe seus dados
 else if(obj_sel && idx_lin < 0)
  exibirDadosPapel(dynamic_cast<Papel *>(obj_sel), idx_tab, lin);
 else
 {
  /* Caso a linha atual da tabela não contenha nenhum dado isso indica
     que a mesmo é uma linha vazia ou seja que foi inclúida recentemente
     e sendo assim esta linha precisa ser removida caso o objeto da seleção
     não seja usado */
  if(!tab_membros[idx_tab]->obterDadoLinha(lin).value<void *>())
   tab_membros[idx_tab]->removerLinha(lin);

  /* Caso o objeto da seleção já exista na tabela dispara uma exceção
     pois o mesmo objeto não pode aparecer mais de uma vez na mesma tabela */
  if(obj_sel && idx_lin >= 0)
  {
   throw Excecao(Excecao::obterMensagemErro(ERR_PGMODELER_INSITEMPAPELDUPLIC)
                               .arg(QString::fromUtf8(obj_sel->obterNome()))
                               .arg(QString::fromUtf8(nome_edt->text())),
                 ERR_PGMODELER_INSITEMPAPELDUPLIC,__PRETTY_FUNCTION__,__FILE__,__LINE__);
  }
 }
}
//----------------------------------------------------------
void PapelWidget::aplicarConfiguracao(void)
{
 Papel *papel=NULL, *papel_aux=NULL;
 unsigned qtd, i, id_tipo,
          tipo_papeis[3]={ Papel::PAPEL_REF, Papel::PAPEL_MEMBRO, Papel::PAPEL_ADMIN };

 try
 {
  //Inicia configuração do papel, alocando-o caso seja um novo objeto
  iniciarConfiguracao<Papel>();

  /* Converte o objeto em edição para o tipo Papel, para se ter acesso
     ao métodos específicos da classe */
  papel=dynamic_cast<Papel *>(this->objeto);

  //Configura o papel com os valores informados no formulário
  papel->definirSysid(sysid_sb->value());
  papel->definirLimiteConexao(limconexao_sb->value());
  papel->definirSenha(senha_edt->text());
  papel->definirValidade(validade_dte->dateTime().toString("yyyy-MM-dd hh:mm"));

  papel->definirOpcao(Papel::OP_SUPERUSER, superusr_chk->isChecked());
  papel->definirOpcao(Papel::OP_CREATEDB, criarbd_chk->isChecked());
  papel->definirOpcao(Papel::OP_CREATEROLE, criarusr_chk->isChecked());
  papel->definirOpcao(Papel::OP_ENCRYPTED, senhacripto_chk->isChecked());
  papel->definirOpcao(Papel::OP_INHERIT, herdarperm_chk->isChecked());
  papel->definirOpcao(Papel::OP_LOGIN, permitirlogin_chk->isChecked());

  /* Varre as tabelas de membros atribuindo os papéis constantes
     nestas tabelas como papeis membros do papel em edição */
  for(id_tipo=0; id_tipo < 3; id_tipo++)
  {
   //Obtém a quantidade de elementos na tabela atual
   qtd=tab_membros[id_tipo]->obterNumLinhas();
   if(qtd > 0) papel->removerPapeis(tipo_papeis[id_tipo]);

   for(i=0; i < qtd; i++)
   {
    //Extrái um objeto da linha atual
    papel_aux=reinterpret_cast<Papel *>(tab_membros[id_tipo]->obterDadoLinha(i).value<void *>());
    //Atribui o mesmo como papel membro do papel atual
    papel->definirPapel(tipo_papeis[id_tipo], papel_aux);
   }
  }

  //Aplica a configuração ao objeto
  ObjetoBaseWidget::aplicarConfiguracao();
  finalizarConfiguracao();
 }
 catch(Excecao &e)
 {
  cancelarConfiguracao();
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}
//***********************************************************
