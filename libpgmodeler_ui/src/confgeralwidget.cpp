#include "confgeralwidget.h"

ConfGeralWidget::ConfGeralWidget(QWidget * parent) : QWidget(parent)
{
 QPrinter::PaperSize cod_tipo_papel[]={QPrinter::A0, QPrinter::A1, QPrinter::A2, QPrinter::A3, QPrinter::A4, QPrinter::A5,
                       QPrinter::A6, QPrinter::A7, QPrinter::A8, QPrinter::A9, QPrinter::B0, QPrinter::B1,
                       QPrinter::B10, QPrinter::B2, QPrinter::B3, QPrinter::B4, QPrinter::B5, QPrinter::B6,
                       QPrinter::B7, QPrinter::B8, QPrinter::B9, QPrinter::C5E, QPrinter::Comm10E, QPrinter::DLE,
                       QPrinter::Executive, QPrinter::Folio, QPrinter::Ledger, QPrinter::Legal, QPrinter::Letter,
                       QPrinter::Tabloid, QPrinter::Custom };

 Ui_ConfGeralWidget::setupUi(this);

 //Atribui os tipos de papel aos elementos do combo de tipo de papel
 for(int i=0; i < 31; i++)
  papel_cmb->setItemData(i, QVariant(cod_tipo_papel[i]));

 connect(unidade_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(converterUnidadeMargem(void)));
 connect(salvar_mod_chk, SIGNAL(toggled(bool)), salvar_mod_spb, SLOT(setEnabled(bool)));

 config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::GRID_SIZE]="";
 config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::OP_LIST_SIZE]="";
 config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::AUTOSAVE_INTERVAL]="";
 config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::PAPER_TYPE]="";
 config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::PAPER_ORIENTATION]="";
 config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::PAPER_MARGIN]="";
 config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::SAVE_SESSION]="";
 config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::SAVE_WIDGETS]="";
 config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::_FILE_]="";
 config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::WIDGET]="";
 config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::PRINT_PG_NUM]="";
 config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::PRINT_GRID]="";
}

void ConfGeralWidget::loadConfiguration(void)
{
 QStringList margem;
 vector<QString> atribs_chave;
 unsigned interv;

 //Para a sessão de configuração geral, o 'id' será usado como campo chave
 atribs_chave.push_back(ParsersAttributes::ID);

 //Carrega as configurações
 BaseConfigWidget::loadConfiguration(GlobalAttributes::GENERAL_CONF, atribs_chave);

 //Repassa os valores obtidos no arquivo de configuração para os widgets do formulário
 tam_grade_spb->setValue((config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::GRID_SIZE]).toUInt());
 tam_lista_spb->setValue((config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::OP_LIST_SIZE]).toUInt());

 interv=(config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::AUTOSAVE_INTERVAL]).toUInt();
 salvar_mod_chk->setChecked(interv > 0);
 salvar_mod_spb->setValue(interv);
 salvar_mod_spb->setEnabled(salvar_mod_chk->isChecked());

 salvar_wgts_chk->setChecked(config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::SAVE_WIDGETS]==ParsersAttributes::_TRUE_);
 salvar_sessao_chk->setChecked(config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::SAVE_SESSION]==ParsersAttributes::_TRUE_);

 print_grid_chk->setChecked(config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::PRINT_GRID]==ParsersAttributes::_TRUE_);
 print_pg_num_chk->setChecked(config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::PRINT_PG_NUM]==ParsersAttributes::_TRUE_);

 papel_cmb->setCurrentIndex((config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::PAPER_TYPE]).toUInt());
 retrato_rb->setChecked(config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::PAPER_ORIENTATION]==ParsersAttributes::PORTRAIT);
 paisagem_rb->setChecked(config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::PAPER_ORIENTATION]==ParsersAttributes::LANDSCAPE);

 margem=config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::PAPER_MARGIN].split(",");

 marg_esq->setValue((margem.count() >= 1 ? margem[0].toFloat() : 10));
 marg_topo->setValue((margem.count()>= 2 ? margem[1].toFloat() : 10));
 marg_dir->setValue((margem.count() >= 3 ? margem[2].toFloat() : 10));
 marg_base->setValue((margem.count() >= 4 ? margem[3].toFloat() : 10));

 //Efetiva as configurações do formulário aplicano-s  s classes interessadas
 this->applyConfiguration();
}

void ConfGeralWidget::saveConfiguration()
{
 try
 {
  map<QString, map<QString, QString> >::iterator itr, itr_end;
  QString sch_widget, sch_arquivo, dir_raiz;

  dir_raiz=GlobalAttributes::CONFIGURATIONS_DIR +
           GlobalAttributes::DIR_SEPARATOR;

  sch_widget=dir_raiz +
             GlobalAttributes::SCHEMAS_DIR +
             GlobalAttributes::DIR_SEPARATOR +
             ParsersAttributes::WIDGET +
             GlobalAttributes::SCHEMA_EXT;

  sch_arquivo=dir_raiz +
              GlobalAttributes::SCHEMAS_DIR +
              GlobalAttributes::DIR_SEPARATOR +
              ParsersAttributes::_FILE_ +
              GlobalAttributes::SCHEMA_EXT;

  //Armazena no mapa de parâmetros de configuração os valores dos widgets no formulário
  config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::GRID_SIZE]=QString("%1").arg(tam_grade_spb->value());
  config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::OP_LIST_SIZE]=QString("%1").arg(tam_lista_spb->value());
  config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::AUTOSAVE_INTERVAL]=QString("%1").arg(salvar_mod_chk->isChecked() ? salvar_mod_spb->value() : 0);
  config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::PAPER_TYPE]=QString("%1").arg(papel_cmb->currentIndex());
  config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::PAPER_ORIENTATION]=(retrato_rb->isChecked() ? ParsersAttributes::PORTRAIT : ParsersAttributes::LANDSCAPE);

  unidade_cmb->setCurrentIndex(0);
  config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::PAPER_MARGIN]=QString("%1,%2,%3,%4").arg(marg_esq->value())
                                                                             .arg(marg_topo->value())
                                                                             .arg(marg_dir->value())
                                                                             .arg(marg_base->value());

  config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::SAVE_SESSION]=(salvar_sessao_chk->isChecked() ? "1" : "");
  config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::SAVE_WIDGETS]=(salvar_wgts_chk->isChecked() ? "1" : "");
  config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::PRINT_PG_NUM]=(print_pg_num_chk->isChecked() ? "1" : "");
  config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::PRINT_GRID]=(print_grid_chk->isChecked() ? "1" : "");

  config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::_FILE_]="";
  config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::WIDGET]="";

  itr=config_params.begin();
  itr_end=config_params.end();

  //Salvando as configurações da sessão e dos widgets
  while(itr!=itr_end)
  {
   if(salvar_sessao_chk->isChecked() &&
     (itr->first).contains(QRegExp(QString("(") +
                           ParsersAttributes::_FILE_ +
                           QString(")([0-9]+)"))))
   {
    config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::_FILE_]+=
    SchemaParser::getCodeDefinition(sch_arquivo, itr->second);
   }
   else if(salvar_wgts_chk->isChecked() &&
     (itr->first).contains(QRegExp(QString("(") +
                           ParsersAttributes::WIDGET +
                           QString(")([0-9]+)"))))
   {
    config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::WIDGET]+=
    SchemaParser::getCodeDefinition(sch_widget, itr->second);
   }

   itr++;
  }

  //Salva a configuração em arquivo
  BaseConfigWidget::saveConfiguration(GlobalAttributes::GENERAL_CONF);
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}

void ConfGeralWidget::applyConfiguration(void)
{
 //Aplica as configurações de pae   cena
 ObjectsScene::setPageConfiguration(static_cast<QPrinter::PaperSize>(papel_cmb->itemData(papel_cmb->currentIndex()).toInt()),
                                        (retrato_rb->isChecked() ? QPrinter::Portrait : QPrinter::Landscape),
                                        QRectF(marg_esq->value(), marg_topo->value(),
                                               marg_dir->value(), marg_base->value()));

 //Aplica a configuração de grd   cena
 ObjectsScene::setGridSize(tam_grade_spb->value());

 //Aplica as configuraçe   lista de operações
 OperationList::setMaximumSize(tam_lista_spb->value());
}

void ConfGeralWidget::restoreDefaults(void)
{
 try
 {
  //Restaura as configurações padrão e recarrega o arquivo restaurado
  BaseConfigWidget::restoreDefaults(GlobalAttributes::GENERAL_CONF);
  this->loadConfiguration();
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}

void ConfGeralWidget::converterUnidadeMargem(void)
{
 static int unidade_ant=0;
 float fator_conv[]={1.0f, 2.83f, 0.04f, 0.1f},
       esq, dir, topo, base;

 esq=marg_esq->value() / fator_conv[unidade_ant];
 dir=marg_dir->value() / fator_conv[unidade_ant];
 base=marg_base->value() / fator_conv[unidade_ant];
 topo=marg_topo->value() / fator_conv[unidade_ant];

 marg_esq->setValue(esq * fator_conv[unidade_cmb->currentIndex()]);
 marg_dir->setValue(dir * fator_conv[unidade_cmb->currentIndex()]);
 marg_base->setValue(base * fator_conv[unidade_cmb->currentIndex()]);
 marg_topo->setValue(topo * fator_conv[unidade_cmb->currentIndex()]);

 unidade_ant=unidade_cmb->currentIndex();
}

