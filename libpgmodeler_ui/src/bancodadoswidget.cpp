#include "bancodadoswidget.h"

BancoDadosWidget::BancoDadosWidget(QWidget *parent): BaseObjectWidget(parent, OBJ_DATABASE)
{
 map<QString, vector<QWidget *> > mapa_campos;
 QFrame *frame=NULL;
 QLocale loc;
 QStringList lista_loc, codificacoes;
 unsigned i,i1;

 Ui_BancoDadosWidget::setupUi(this);

 connect(parent_form->aplicar_ok_btn,SIGNAL(clicked(bool)), this, SLOT(applyConfiguration(void)));
 configureFormLayout(bancodados_grid, OBJ_DATABASE);

 //Define os campos exclusivos para cada versão
 mapa_campos[generateVersionsInterval(AFTER_VERSION, SchemaParser::PGSQL_VERSION_83)].push_back(connlim_lbl);
 mapa_campos[generateVersionsInterval(AFTER_VERSION, SchemaParser::PGSQL_VERSION_84)].push_back(lccollate_lbl);
 mapa_campos[generateVersionsInterval(AFTER_VERSION, SchemaParser::PGSQL_VERSION_84)].push_back(lcctype_lbl);

 //Gera o frame de alerta
 frame=generateVersionWarningFrame(mapa_campos);
 bancodados_grid->addWidget(frame, bancodados_grid->count()+1, 0, 1, 0);
 frame->setParent(this);

 //Define as alturas mínimas e máxima do formulário
 parent_form->setMinimumWidth(530);
 parent_form->setMinimumHeight(420);
 parent_form->setMaximumHeight(420);

 //Obtém os nomes das codificações e as insere no combo de codificação
 EncodingType::getTypes(codificacoes);
 encoding_cmb->addItems(codificacoes);

 //Obtém todas as localizações padrão e as armazena em uma lista de QString
 for(i=QLocale::C; i <= QLocale::Chewa; i++)
 {
  for(i1=QLocale::Afghanistan; i1 <= QLocale::Zimbabwe; i1++)
  {
   loc=QLocale(static_cast<QLocale::Language>(i),static_cast<QLocale::Country>(i1));
   lista_loc.append(loc.name());
  }
 }

 //Remove as localizações duplicadas
 lista_loc.removeDuplicates();
 //Ordena as localizações
 lista_loc.sort();

 //Insere as localizações nos combos
 lccollate_cmb->addItem(trUtf8("Default"));
 lcctype_cmb->addItem(trUtf8("Default"));
 lccollate_cmb->addItems(lista_loc);
 lcctype_cmb->addItems(lista_loc);
}

void BancoDadosWidget::setAttributes(DatabaseModel *modelo)
{
 if(modelo)
 {
  int idx;

  //Configura os atributos de limite de conexão, banco modelo e autor
  connlim_sb->setValue(modelo->getConnectionLimit());
  templatedb_edt->setText(QString::fromUtf8(modelo->getTemplateDB()));
  author_edt->setText(QString::fromUtf8(modelo->getAuthor()));

  //Configura o combo de codificação com a codificação atual
  idx=encoding_cmb->findText(~modelo->getEncoding());
  if(idx < 0) idx=0;
  encoding_cmb->setCurrentIndex(idx);

  //Configura as localizações LC_COLLATE E LC_CTYPE de acordo com a conf. atual
  idx=lccollate_cmb->findText(modelo->getLocalization(LC_COLLATE));
  if(idx < 0) idx=0;
  lccollate_cmb->setCurrentIndex(idx);

  idx=lcctype_cmb->findText(modelo->getLocalization(LC_CTYPE));
  if(idx < 0) idx=0;
  lcctype_cmb->setCurrentIndex(idx);

  //Define os atributos do formulários e da janela pai
  BaseObjectWidget::setAttributes(modelo, NULL, modelo);
 }
}

void BancoDadosWidget::applyConfiguration(void)
{
 try
 {
  //Aplica as configurações básicas do objeto
  BaseObjectWidget::applyConfiguration();

  //Define o autor do modelo
  model->setAuthor(author_edt->text().toUtf8());

  /* Define a condificação do modelo de acordo com a selecionada no formulário
     caso a codifição 'Padrão' seja selecionada o modelo usará a codificação padrão
     do SGBD em que for executado o script sql gerado */
  model->setEncoding(EncodingType(encoding_cmb->currentText()));

  /* Define as localização LC_COLLATE e LC_CTYPE do modelo de acordo com a selecionada
     no formulário caso a localização 'Padrão' seja selecionada o modelo usará a localização
     padrão do SGBD em que for executado o script sql gerado */
  if(lccollate_cmb->currentIndex() > 0)
   model->setLocalization(LC_COLLATE, lccollate_cmb->currentText());
  else
   model->setLocalization(LC_COLLATE, "");

  if(lcctype_cmb->currentIndex() > 0)
   model->setLocalization(LC_CTYPE, lcctype_cmb->currentText());
  else
   model->setLocalization(LC_CTYPE, "");

  finishConfiguration();
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}

