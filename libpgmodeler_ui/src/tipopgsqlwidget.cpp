#include "tipopgsqlwidget.h"
//***********************************************************
TipoPgSQLWidget::TipoPgSQLWidget(QWidget *parent, const QString &rotulo) : QWidget(parent)
{
 try
 {
  QStringList tipo_interv;

  setupUi(this);

  if(!rotulo.isEmpty())
   groupBox->setTitle(rotulo);

  this->setWindowTitle(groupBox->title());

  //Aloca um destacador de código fonte para o campo de formato do tipo
  destaque_fmt=NULL;
  destaque_fmt=new DestaqueSintaxe(formato_txt, false);

  //A configuração padrão carregada é a de destaque de código SQL
  destaque_fmt->carregarConfiguracao(AtributosGlobais::DIR_CONFIGURACOES +
                                     AtributosGlobais::SEP_DIRETORIO +
                                     AtributosGlobais::CONF_DESTAQUE_SQL +
                                     AtributosGlobais::EXT_CONFIGURACAO);

  //Configura o combo de tipos de intervalo
  TipoIntervalo::obterTipos(tipo_interv);
  tipo_interv_cmb->addItem("");
  tipo_interv_cmb->addItems(tipo_interv);

  //Conecta os objetos do formulário com o método de atualização do formato do tipo
  connect(tipo_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(atualizarFormatoTipo(void)));
  connect(precisao_sb, SIGNAL(valueChanged(int)), this, SLOT(atualizarFormatoTipo(void)));
  connect(comprimento_sb, SIGNAL(valueChanged(int)), this, SLOT(atualizarFormatoTipo(void)));
  connect(dimensao_sb, SIGNAL(valueChanged(int)), this, SLOT(atualizarFormatoTipo(void)));
  connect(tipo_interv_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(atualizarFormatoTipo(void)));
  connect(timezone_chk, SIGNAL(toggled(bool)), this, SLOT(atualizarFormatoTipo(void)));
 }
 catch(Excecao &e)
 {
  //Redireciona o erro
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}
//-----------------------------------------------------------
void TipoPgSQLWidget::atualizarFormatoTipo(void)
{
 try
 {
  QVariant dado;

  //Obtém o índice relacionado ao tipo de dados selecionado no combo
  dado=tipo_cmb->itemData(tipo_cmb->currentIndex());

  /* Caso o índice seja 0 indica que o tipo é uma tipo built-in do postgresql
     não devendo ser tratado como um tipo definido pelo usuário */
  if(dado.toUInt()==0)
   tipo=tipo_cmb->currentText();
  else
   //Caso o índice seja superior a zero trata-se de um tipo definido pelo usuário
   tipo=dado.toUInt();

  /* O campo de comprimento só deve ser ativado quando o tipo de
    dado é de comprimento variável: varchar, char, varbit, etc */
  comprimento_sb->setEnabled(tipo.tipoCompVariavel());

  //O campo de timezone só deve ser ativado quando se tratar de um tipo relacionado a tempo
  timezone_chk->setVisible(tipo=="timestamp" || tipo=="time");
  timezone_lbl->setVisible(timezone_chk->isVisible());

  //O campo de precisão só é ativado quando o tipo em questão aceita precisão
  precisao_sb->setEnabled(tipo.tipoAceitaPrecisao());

  //O campo de dimensão só é ativado quando o tipo não é void
  dimensao_sb->setEnabled(tipo!="void");

  /* O campo de intervalo só é ativado quando o tipo é 'interval' (o único
     que aceita esse dado) */
  tipo_interv_cmb->setVisible(tipo=="interval");
  tipo_interv_lbl->setVisible(tipo_interv_cmb->isVisible());

  //Configura o tipo com os valores do formulário
  tipo.definirComprimento(comprimento_sb->value());
  tipo.definirPrecisao(precisao_sb->value());
  tipo.definirDimensao(dimensao_sb->value());
  tipo.definirTipoIntervalo(tipo_interv_cmb->currentText());
  tipo.definirComTimezone(timezone_chk->isChecked());

  //Atualiza o formato do tipo exibido no campo
  formato_txt->setPlainText(QString::fromUtf8(*tipo));
 }
 catch(Excecao &e)
 {
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}
//-----------------------------------------------------------
void TipoPgSQLWidget::obterTiposPgSQL(QComboBox *combo, ModeloBD *modelo, bool tipo_usr, bool dominio, bool tipo_oid, bool pseudo)
{
 if(combo)
 {
  QStringList tipos;
  int idx, qtd;

  combo->clear();

  if(tipo_usr)
  {
   //Obtém os tipo definidos pelo usuário e insere ao combo de tipo
   TipoPgSQL::obterTiposUsuario(tipos,modelo,true,false);
   tipos.sort();
   qtd=tipos.size();

   /* Adiciona cada indice referente ao tipo como dado do elemento do combobox
      para se referenciar de forma mais direta o tipo definido pelo usuário */
   for(idx=0; idx < qtd; idx++)
    combo->addItem(QString::fromUtf8(tipos[idx]), QVariant(TipoPgSQL::obterIndiceTipoUsuario(tipos[idx],NULL,modelo)));
  }

  if(dominio)
  {
   //Obtém os domínios definidos pelo usuário e insere ao combo de tipo
   TipoPgSQL::obterTiposUsuario(tipos,modelo,false,true);
   tipos.sort();
   qtd=tipos.size();

   /* Adiciona cada índice referente ao tipo como dado do elemento do combobox
      para se referenciar de forma mais direta o tipo definido pelo usuário */
   for(idx=0; idx < qtd; idx++)
    combo->addItem(QString::fromUtf8(tipos[idx]), QVariant(TipoPgSQL::obterIndiceTipoUsuario(tipos[idx],NULL,modelo)));
  }

  /* Obtendo os demais tipos (builtin) do PostgreSQL. Neste caso, por serem tipos
     de fácil acesso não é necessário armazena os índices dos mesmos como
     acontece com os tipos definidos pelo usuário */
  TipoPgSQL::obterTipos(tipos, tipo_oid, pseudo);
  tipos.sort();
  combo->addItems(tipos);
 }
}
//-----------------------------------------------------------
void TipoPgSQLWidget::definirAtributos(TipoPgSQL tipo, ModeloBD *modelo, bool tipo_usr, bool dominio, bool tipo_oid, bool pseudo)
{
 try
 {
  int idx;

  /* Bloqueia os sinais que são emitidos pelo combo de tipo
     para evitar a atualização desnecessária do formato do tipo
     pois o método de obtenção dos tipos obterTiposPgSQL() limpa
     automaticamente o combo e isso evita que erros sejam disparados */
  tipo_cmb->blockSignals(true);

  //Obtém os tipos PgSQL
  obterTiposPgSQL(tipo_cmb, modelo, tipo_usr, dominio, tipo_oid, pseudo);

  //Reativa os sinais do combo de tipos
  tipo_cmb->blockSignals(false);

  /* Obtém o índice do tipo no combo de tipos de acordo com o tipo de dados
     passado no parâmetro */
  idx=tipo_cmb->findText(~tipo);

  //Seleciona o tipo no combobox
  tipo_cmb->setCurrentIndex(idx);

  /* Configura os campos do formulário com os respectivos valores
     configurados no tipo de dado passado ao método */
  precisao_sb->setValue(tipo.obterPrecisao());
  dimensao_sb->setValue(tipo.obterDimensao());
  comprimento_sb->setValue(tipo.obterComprimento());

  idx=tipo_interv_cmb->findText(~(tipo.obterTipoIntervalo()));
  tipo_interv_cmb->setCurrentIndex(idx);

  timezone_chk->setChecked(tipo.comTimezone());

  /* Atribui o tipo do parâmetro ao tipo de dado configurado
     pelo formulário o qual é retornado ao usuário */
  this->tipo=tipo;

  //Atualiza a exibição do formato do tipo configurado
  atualizarFormatoTipo();
 }
 catch(Excecao &e)
 {
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}
//-----------------------------------------------------------
TipoPgSQL TipoPgSQLWidget::obterTipoPgSQL(void)
{
 return(tipo);
}
//***********************************************************
