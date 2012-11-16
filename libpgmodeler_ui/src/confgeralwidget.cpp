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

 params_config[ParsersAttributes::CONFIGURATION][ParsersAttributes::GRID_SIZE]="";
 params_config[ParsersAttributes::CONFIGURATION][ParsersAttributes::OP_LIST_SIZE]="";
 params_config[ParsersAttributes::CONFIGURATION][ParsersAttributes::AUTOSAVE_INTERVAL]="";
 params_config[ParsersAttributes::CONFIGURATION][ParsersAttributes::PAPER_TYPE]="";
 params_config[ParsersAttributes::CONFIGURATION][ParsersAttributes::PAPER_ORIENTATION]="";
 params_config[ParsersAttributes::CONFIGURATION][ParsersAttributes::PAPER_MARGIN]="";
 params_config[ParsersAttributes::CONFIGURATION][ParsersAttributes::SAVE_SESSION]="";
 params_config[ParsersAttributes::CONFIGURATION][ParsersAttributes::SAVE_WIDGETS]="";
 params_config[ParsersAttributes::CONFIGURATION][ParsersAttributes::_FILE_]="";
 params_config[ParsersAttributes::CONFIGURATION][ParsersAttributes::WIDGET]="";
}

void ConfGeralWidget::carregarConfiguracao(void)
{
 QStringList margem;
 vector<QString> atribs_chave;
 unsigned interv;

 //Para a sessão de configuração geral, o 'id' será usado como campo chave
 atribs_chave.push_back(ParsersAttributes::ID);

 //Carrega as configurações
 ConfBaseWidget::carregarConfiguracao(GlobalAttributes::GENERAL_CONF, atribs_chave);

 //Repassa os valores obtidos no arquivo de configuração para os widgets do formulário
 tam_grade_spb->setValue((params_config[ParsersAttributes::CONFIGURATION][ParsersAttributes::GRID_SIZE]).toUInt());
 tam_lista_spb->setValue((params_config[ParsersAttributes::CONFIGURATION][ParsersAttributes::OP_LIST_SIZE]).toUInt());

 interv=(params_config[ParsersAttributes::CONFIGURATION][ParsersAttributes::AUTOSAVE_INTERVAL]).toUInt();
 salvar_mod_chk->setChecked(interv > 0);
 salvar_mod_spb->setValue(interv);
 salvar_mod_spb->setEnabled(salvar_mod_chk->isChecked());

 salvar_wgts_chk->setChecked(params_config[ParsersAttributes::CONFIGURATION][ParsersAttributes::SAVE_WIDGETS]==ParsersAttributes::_TRUE_);
 salvar_sessao_chk->setChecked(params_config[ParsersAttributes::CONFIGURATION][ParsersAttributes::SAVE_SESSION]==ParsersAttributes::_TRUE_);

 papel_cmb->setCurrentIndex((params_config[ParsersAttributes::CONFIGURATION][ParsersAttributes::PAPER_TYPE]).toUInt());
 retrato_rb->setChecked(params_config[ParsersAttributes::CONFIGURATION][ParsersAttributes::PAPER_ORIENTATION]==ParsersAttributes::PORTRAIT);
 paisagem_rb->setChecked(params_config[ParsersAttributes::CONFIGURATION][ParsersAttributes::PAPER_ORIENTATION]==ParsersAttributes::LANDSCAPE);

 margem=params_config[ParsersAttributes::CONFIGURATION][ParsersAttributes::PAPER_MARGIN].split(",");

 marg_esq->setValue((margem.count() >= 1 ? margem[0].toFloat() : 10));
 marg_topo->setValue((margem.count()>= 2 ? margem[1].toFloat() : 10));
 marg_dir->setValue((margem.count() >= 3 ? margem[2].toFloat() : 10));
 marg_base->setValue((margem.count() >= 4 ? margem[3].toFloat() : 10));

 //Efetiva as configurações do formulário aplicano-s  s classes interessadas
 this->aplicarConfiguracao();
}

void ConfGeralWidget::salvarConfiguracao()
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
  params_config[ParsersAttributes::CONFIGURATION][ParsersAttributes::GRID_SIZE]=QString("%1").arg(tam_grade_spb->value());
  params_config[ParsersAttributes::CONFIGURATION][ParsersAttributes::OP_LIST_SIZE]=QString("%1").arg(tam_lista_spb->value());
  params_config[ParsersAttributes::CONFIGURATION][ParsersAttributes::AUTOSAVE_INTERVAL]=QString("%1").arg(salvar_mod_chk->isChecked() ? salvar_mod_spb->value() : 0);
  params_config[ParsersAttributes::CONFIGURATION][ParsersAttributes::PAPER_TYPE]=QString("%1").arg(papel_cmb->currentIndex());
  params_config[ParsersAttributes::CONFIGURATION][ParsersAttributes::PAPER_ORIENTATION]=(retrato_rb->isChecked() ? ParsersAttributes::PORTRAIT : ParsersAttributes::LANDSCAPE);

  unidade_cmb->setCurrentIndex(0);
  params_config[ParsersAttributes::CONFIGURATION][ParsersAttributes::PAPER_MARGIN]=QString("%1,%2,%3,%4").arg(marg_esq->value())
                                                                             .arg(marg_topo->value())
                                                                             .arg(marg_dir->value())
                                                                             .arg(marg_base->value());

  params_config[ParsersAttributes::CONFIGURATION][ParsersAttributes::SAVE_SESSION]=(salvar_sessao_chk->isChecked() ? "1" : "");
  params_config[ParsersAttributes::CONFIGURATION][ParsersAttributes::SAVE_WIDGETS]=(salvar_wgts_chk->isChecked() ? "1" : "");
  params_config[ParsersAttributes::CONFIGURATION][ParsersAttributes::_FILE_]="";
  params_config[ParsersAttributes::CONFIGURATION][ParsersAttributes::WIDGET]="";

  itr=params_config.begin();
  itr_end=params_config.end();

  //Salvando as configurações da sessão e dos widgets
  while(itr!=itr_end)
  {
   if(salvar_sessao_chk->isChecked() &&
     (itr->first).contains(QRegExp(QString("(") +
                           ParsersAttributes::_FILE_ +
                           QString(")([0-9]+)"))))
   {
    params_config[ParsersAttributes::CONFIGURATION][ParsersAttributes::_FILE_]+=
    SchemaParser::getObjectDefinition(sch_arquivo, itr->second);
   }
   else if(salvar_wgts_chk->isChecked() &&
     (itr->first).contains(QRegExp(QString("(") +
                           ParsersAttributes::WIDGET +
                           QString(")([0-9]+)"))))
   {
    params_config[ParsersAttributes::CONFIGURATION][ParsersAttributes::WIDGET]+=
    SchemaParser::getObjectDefinition(sch_widget, itr->second);
   }

   itr++;
  }

  //Salva a configuração em arquivo
  ConfBaseWidget::salvarConfiguracao(GlobalAttributes::GENERAL_CONF);
 }
 catch(Exception &e)
 {
  throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}

void ConfGeralWidget::aplicarConfiguracao(void)
{
 //Aplica as configurações de pae   cena
 CenaObjetos::definirConfiguracaoPagina(static_cast<QPrinter::PaperSize>(papel_cmb->itemData(papel_cmb->currentIndex()).toInt()),
                                        (retrato_rb->isChecked() ? QPrinter::Portrait : QPrinter::Landscape),
                                        QRectF(marg_esq->value(), marg_topo->value(),
                                               marg_dir->value(), marg_base->value()));

 //Aplica a configuração de grd   cena
 CenaObjetos::definirGrade(tam_grade_spb->value());

 //Aplica as configuraçe   lista de operações
 OperationList::setMaximumSize(tam_lista_spb->value());
}

void ConfGeralWidget::restaurarPadroes(void)
{
 try
 {
  //Restaura as configurações padrão e recarrega o arquivo restaurado
  ConfBaseWidget::restaurarPadroes(GlobalAttributes::GENERAL_CONF);
  this->carregarConfiguracao();
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

