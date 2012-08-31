#include "seletorobjetowidget.h"
#include "visaoobjetoswidget.h"
extern VisaoObjetosWidget *selecaoobjetos_wgt;
//***********************************************************
QObject *SeletorObjetoWidget::seletor_atual=NULL;
//-----------------------------------------------------------
SeletorObjetoWidget::SeletorObjetoWidget(TipoObjetoBase tipo_obj_seletor, bool inst_destaque_txt, QWidget *parent): QWidget(parent)
{
 try
 {
  Ui_SeletorObjetoWidget::setupUi(this);

  modelo=NULL;
  objeto=NULL;
  destaque_txt=NULL;

  //Define o tipo de objeto que o seletor aceita
  this->tipo_obj_seletor=tipo_obj_seletor;

  /* Caso a flag de instalação de destacador de código fonte
     esteja setada. */
  if(inst_destaque_txt)
  {
   //Aloca um destacador de código fonte
   destaque_txt=new DestaqueSintaxe(nome_objeto_txt, false);

   //A configuração padrão carregada é a de destaque de código SQL
   destaque_txt->carregarConfiguracao(AtributosGlobais::DIR_CONFIGURACOES +
                                      AtributosGlobais::SEP_DIRETORIO +
                                      AtributosGlobais::CONF_DESTAQUE_SQL +
                                      AtributosGlobais::EXT_CONFIGURACAO);
  }

  connect(sel_objeto_tb, SIGNAL(clicked(bool)), this, SLOT(exibirSelecaoObjetos(void)));
  connect(rem_objeto_tb, SIGNAL(clicked(bool)), this, SLOT(removerObjetoSelecionado(void)));
  connect(selecaoobjetos_wgt, SIGNAL(s_visibilityChanged(ObjetoBase*,bool)), this, SLOT(exibirObjetoSelecionado(ObjetoBase*, bool)));
 }
 catch(Excecao &e)
 {
  //Redireciona o erro
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}
//-----------------------------------------------------------
ObjetoBase *SeletorObjetoWidget::obterObjeto(void)
{
 return(objeto);
}
//-----------------------------------------------------------
void SeletorObjetoWidget::definirObjeto(ObjetoBase *objeto)
{
 TipoObjetoBase tipo_obj;
 QString nome_obj;

 //Caso o objeto esteja alocado
 if(objeto)
 {
  //Obtém o seu tipo
  tipo_obj=objeto->obterTipoObjeto();

  /* Dependendo do tipo do objeto obtém o nome ou assinatura
     do mesmo com o método correto. */
  if(tipo_obj==OBJETO_FUNCAO)
   nome_obj=dynamic_cast<Funcao *>(objeto)->obterAssinatura();
  else if(tipo_obj==OBJETO_OPERADOR)
   nome_obj=dynamic_cast<Operador *>(objeto)->obterAssinatura();
  /* Formatando o nome do objeto caso o mesmo seja um objeto de tabela.
     nesse modo o nome será exibido da seguinte forma:
     esquema.tabela.objeto */
  else if(tipo_obj==OBJETO_COLUNA || tipo_obj==OBJETO_RESTRICAO || tipo_obj==OBJETO_REGRA ||
     tipo_obj==OBJETO_GATILHO ||tipo_obj==OBJETO_INDICE)
  {
   ObjetoBase *tab_pai=dynamic_cast<ObjetoTabela *>(objeto)->obterTabelaPai();
   if(tab_pai)
   {
   /*if(tab_pai->obterEsquema())
      nome_obj+=tab_pai->obterEsquema()->obterNome() + ".";*/
    nome_obj+=tab_pai->obterNome(true) + ".";
   }
   nome_obj+=objeto->obterNome();
  }
  else
  {
   /* if(objeto->obterEsquema())
    nome_obj=objeto->obterEsquema()->obterNome() + ".";
   nome_obj+=objeto->obterNome(); */
   nome_obj=objeto->obterNome(true);
  }
 }

 /* A identificação do objeto só será exibida caso o tipo do mesmo seja compatível
    com o tipo de objeto aceito pelo seletor e caso o objeto esteja alocado (selecionado) */
 if(objeto && tipo_obj==tipo_obj_seletor)
 {
  //Exibe o nome do objeto
  nome_objeto_txt->setPlainText(QString::fromUtf8(nome_obj));
  //Habilita o botão de remoção do objeto no seletor
  rem_objeto_tb->setEnabled(objeto);
  //Armazena a referência ao objeto no seletor
  this->objeto=objeto;

  //Emite o sinal indicando que um objeto foi selecionado
  emit s_objetoSelecionado();
 }
 else
  removerObjetoSelecionado();
}
//-----------------------------------------------------------
void SeletorObjetoWidget::definirModelo(ModeloBD *modelo)
{
 this->modelo=modelo;
}
//-----------------------------------------------------------
void SeletorObjetoWidget::exibirObjetoSelecionado(ObjetoBase *obj_sel, bool)
{
 /* Caso se tente atribuir um objeto ao seletor é necessário verificar
    se o seletor atual (em foco) é o mesmo que chamou o método pois
    isso evita que outros seletores num mesmo formulário recebam o
    objeto escolhido (o que seria incorreto) */
 if(SeletorObjetoWidget::seletor_atual==this && obj_sel)
  definirObjeto(obj_sel);
}
//-----------------------------------------------------------
void SeletorObjetoWidget::removerObjetoSelecionado(void)
{
 //Limpa a referência ao objeto selecionado
 this->objeto=NULL;
 //Limpa o campo de texto que contém o nome do objeto
 nome_objeto_txt->clear();
 //Desabilita o botão de remoção do objeto
 rem_objeto_tb->setEnabled(false);

 //Emite o sinal indicando que um objeto foi selecionado
 emit s_objetoRemovido();
}
//-----------------------------------------------------------
void SeletorObjetoWidget::exibirSelecaoObjetos(void)
{
 /* Antes de exibir o form de seleção de objeto é preciso armazenar
    o seletor que chama este form para que o objeto escolhido pelo
    usuário seja exibido no seletor correto. Para isso sua referência
    é armazenada no atributo estático da classe o qual é checado por
    todas as instâncias desta classe a fim de se evitar atribuições
    incorretas. */
 SeletorObjetoWidget::seletor_atual=this;

 /* Exibe o form de seleção de objetos exibindo apenas os objetos
    do tipo aceito pelo seletor */
 selecaoobjetos_wgt->definirObjetoVisivel(tipo_obj_seletor, true);
 selecaoobjetos_wgt->definirModelo(this->modelo);
 selecaoobjetos_wgt->show();
}
//***********************************************************
