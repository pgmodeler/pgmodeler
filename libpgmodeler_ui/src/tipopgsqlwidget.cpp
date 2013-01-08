#include "tipopgsqlwidget.h"

TipoPgSQLWidget::TipoPgSQLWidget(QWidget *parent, const QString &rotulo) : QWidget(parent)
{
 try
 {
  QStringList tipo_interv, tipo_esp;

  setupUi(this);

  if(!rotulo.isEmpty())
   groupBox->setTitle(rotulo);

  this->setWindowTitle(groupBox->title());

  //Aloca um destacador de código fonte para o campo de formato do tipo
  destaque_fmt=NULL;
  destaque_fmt=new DestaqueSintaxe(formato_txt, false);

  //A configuração padrão carregada é a de destaque de código SQL
  destaque_fmt->carregarConfiguracao(GlobalAttributes::CONFIGURATIONS_DIR +
                                     GlobalAttributes::DIR_SEPARATOR +
                                     GlobalAttributes::SQL_HIGHLIGHT_CONF +
                                     GlobalAttributes::CONFIGURATION_EXT);

  //Configura o combo de tipos de intervalo
  IntervalType::getTypes(tipo_interv);
  tipo_interv_cmb->addItem("");
  tipo_interv_cmb->addItems(tipo_interv);

  //Configura o combo de tipos espaciais
  SpatialType::getTypes(tipo_esp);
  tipo_esp_cmb->addItems(tipo_esp);

  //Conecta os objetos do formulário com o método de atualização do formato do tipo
  connect(tipo_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(atualizarFormatoTipo(void)));
  connect(precisao_sb, SIGNAL(valueChanged(int)), this, SLOT(atualizarFormatoTipo(void)));
  connect(comprimento_sb, SIGNAL(valueChanged(int)), this, SLOT(atualizarFormatoTipo(void)));
  connect(dimensao_sb, SIGNAL(valueChanged(int)), this, SLOT(atualizarFormatoTipo(void)));
  connect(tipo_interv_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(atualizarFormatoTipo(void)));
  connect(timezone_chk, SIGNAL(toggled(bool)), this, SLOT(atualizarFormatoTipo(void)));
  connect(tipo_esp_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(atualizarFormatoTipo(void)));
  connect(var_m_chk, SIGNAL(toggled(bool)), this, SLOT(atualizarFormatoTipo(void)));
  connect(var_z_chk, SIGNAL(toggled(bool)), this, SLOT(atualizarFormatoTipo(void)));
  connect(srid_spb, SIGNAL(valueChanged(int)), this, SLOT(atualizarFormatoTipo(void)));
 }
 catch(Exception &e)
 {
  //Redireciona o erro
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}

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
  comprimento_sb->setEnabled(tipo.hasVariableLength());

  //O campo de timezone só deve ser ativado quando se tratar de um tipo relacionado a tempo
  timezone_chk->setVisible(tipo=="timestamp" || tipo=="time");
  timezone_lbl->setVisible(timezone_chk->isVisible());

  //O campo de precisão só é ativado quando o tipo em questão aceita precisão
  precisao_sb->setEnabled(tipo.acceptsPrecision());

  //O campo de dimensão só é ativado quando o tipo não é void
  dimensao_sb->setEnabled(tipo!="void");

  /* O campo de intervalo só é ativado quando o tipo é 'interval' (o único
     que aceita esse dado) */
  tipo_interv_cmb->setVisible(tipo=="interval");
  tipo_interv_lbl->setVisible(tipo_interv_cmb->isVisible());

  //O campo de tipo espacial só é ativado quando o tipo é 'geometry' ou 'geography' é selecionado
  tipo_esp_cmb->setVisible(tipo=="geometry" || tipo=="geography");
  tipo_esp_lbl->setVisible(tipo_esp_cmb->isVisible());
  variacao_lbl->setVisible(tipo_esp_cmb->isVisible());
  srid_lbl->setVisible(tipo_esp_cmb->isVisible());
  srid_spb->setVisible(tipo_esp_cmb->isVisible());
  var_m_chk->setVisible(tipo_esp_cmb->isVisible());
  var_z_chk->setVisible(tipo_esp_cmb->isVisible());

  if(tipo_esp_cmb->isVisible())
  {
   SpatialType tp_esp;

   if(tipo=="geography") srid_spb->setValue(4326);

   //Configurando o tipo espacial conforme o formulário
   tp_esp=SpatialType(tipo_esp_cmb->currentText(), srid_spb->value());

   if(var_z_chk->isChecked() && var_m_chk->isChecked())
    tp_esp.setVariation(SpatialType::var_zm);
   else if(var_m_chk->isChecked())
    tp_esp.setVariation(SpatialType::var_m);
   else if(var_z_chk->isChecked())
    tp_esp.setVariation(SpatialType::var_z);

   tipo.setSpatialType(tp_esp);
  }

  //Configura o tipo com os valores do formulário
  tipo.setLength(comprimento_sb->value());
  tipo.setPrecision(precisao_sb->value());
  tipo.setDimension(dimensao_sb->value());
  tipo.setIntervalType(tipo_interv_cmb->currentText());
  tipo.setWithTimezone(timezone_chk->isChecked());

  //Atualiza o formato do tipo exibido no campo
  formato_txt->setPlainText(QString::fromUtf8(*tipo));
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}

void TipoPgSQLWidget::obterTiposPgSQL(QComboBox *combo, DatabaseModel *modelo, unsigned conf_tipo_usr, bool tipo_oid, bool pseudo)
{
 if(combo)
 {
  QStringList tipos;
  int idx, qtd;

  combo->clear();

  //Obtém os tipo definidos pelo usuário e insere ao combo de tipo
  PgSQLType::getUserTypes(tipos,modelo, conf_tipo_usr);
  tipos.sort();
  qtd=tipos.size();

  /* Adiciona cada indice referente ao tipo como dado do elemento do combobox
      para se referenciar de forma mais direta o tipo definido pelo usuário */
  for(idx=0; idx < qtd; idx++)
   combo->addItem(QString::fromUtf8(tipos[idx]), QVariant(PgSQLType::getUserTypeIndex(tipos[idx],NULL,modelo)));

  /* Obtendo os demais tipos (builtin) do PostgreSQL. Neste caso, por serem tipos
     de fácil acesso não é necessário armazena os índices dos mesmos como
     acontece com os tipos definidos pelo usuário */
  PgSQLType::getTypes(tipos, tipo_oid, pseudo);
  tipos.sort();
  combo->addItems(tipos);
 }
}

void TipoPgSQLWidget::definirAtributos(PgSQLType tipo, DatabaseModel *modelo,  unsigned conf_tipo_usr, bool tipo_oid, bool pseudo)
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
  obterTiposPgSQL(tipo_cmb, modelo, conf_tipo_usr, tipo_oid, pseudo);

  //Reativa os sinais do combo de tipos
  tipo_cmb->blockSignals(false);

  /* Obtém o índice do tipo no combo de tipos de acordo com o tipo de dados
     passado no parâmetro */
  idx=tipo_cmb->findText(QString::fromUtf8(~tipo));

  //Seleciona o tipo no combobox
  tipo_cmb->setCurrentIndex(idx);

  /* Configura os campos do formulário com os respectivos valores
     configurados no tipo de dado passado ao método */
  precisao_sb->setValue(tipo.getPrecision());
  dimensao_sb->setValue(tipo.getDimension());
  comprimento_sb->setValue(tipo.getLength());

  idx=tipo_interv_cmb->findText(~(tipo.getIntervalType()));
  tipo_interv_cmb->setCurrentIndex(idx);

  timezone_chk->setChecked(tipo.isWithTimezone());

  /* Atribui o tipo do parâmetro ao tipo de dado configurado
     pelo formulário o qual é retornado ao usuário */
  this->tipo=tipo;

  //Atualiza a exibição do formato do tipo configurado
  atualizarFormatoTipo();
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}

PgSQLType TipoPgSQLWidget::obterTipoPgSQL(void)
{
 return(tipo);
}

