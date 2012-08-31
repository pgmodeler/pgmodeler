#include "confgeralwidget.h"
//***********************************************************
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

 params_config[AtributosParsers::CONFIGURACAO][AtributosParsers::TAM_GRADE]="";
 params_config[AtributosParsers::CONFIGURACAO][AtributosParsers::TAM_LISTA_OP]="";
 params_config[AtributosParsers::CONFIGURACAO][AtributosParsers::INTERVALO_SALVAR_AUTO]="";
 params_config[AtributosParsers::CONFIGURACAO][AtributosParsers::TIPO_PAPEL_IMP]="";
 params_config[AtributosParsers::CONFIGURACAO][AtributosParsers::ORIENTACAO_PAPEL]="";
 params_config[AtributosParsers::CONFIGURACAO][AtributosParsers::MARGEM_PAPEL]="";
 params_config[AtributosParsers::CONFIGURACAO][AtributosParsers::SALVAR_SESSAO]="";
 params_config[AtributosParsers::CONFIGURACAO][AtributosParsers::SALVAR_WIDGETS]="";
 params_config[AtributosParsers::CONFIGURACAO][AtributosParsers::ARQUIVO]="";
 params_config[AtributosParsers::CONFIGURACAO][AtributosParsers::WIDGET]="";
}
//-----------------------------------------------------------
void ConfGeralWidget::carregarConfiguracao(void)
{
 QStringList margem;
 vector<QString> atribs_chave;
 unsigned interv;

 //Para a sessão de configuração geral, o 'id' será usado como campo chave
 atribs_chave.push_back(AtributosParsers::ID);

 //Carrega as configurações
 ConfBaseWidget::carregarConfiguracao(AtributosGlobais::CONF_GERAL, atribs_chave);

 //Repassa os valores obtidos no arquivo de configuração para os widgets do formulário
 tam_grade_spb->setValue((params_config[AtributosParsers::CONFIGURACAO][AtributosParsers::TAM_GRADE]).toUInt());
 tam_lista_spb->setValue((params_config[AtributosParsers::CONFIGURACAO][AtributosParsers::TAM_LISTA_OP]).toUInt());

 interv=(params_config[AtributosParsers::CONFIGURACAO][AtributosParsers::INTERVALO_SALVAR_AUTO]).toUInt();
 salvar_mod_chk->setChecked(interv > 0);
 salvar_mod_spb->setValue(interv);
 salvar_mod_spb->setEnabled(salvar_mod_chk->isChecked());

 salvar_wgts_chk->setChecked(params_config[AtributosParsers::CONFIGURACAO][AtributosParsers::SALVAR_WIDGETS]==AtributosParsers::VERDADEIRO);
 salvar_sessao_chk->setChecked(params_config[AtributosParsers::CONFIGURACAO][AtributosParsers::SALVAR_SESSAO]==AtributosParsers::VERDADEIRO);

 papel_cmb->setCurrentIndex((params_config[AtributosParsers::CONFIGURACAO][AtributosParsers::TIPO_PAPEL_IMP]).toUInt());
 retrato_rb->setChecked(params_config[AtributosParsers::CONFIGURACAO][AtributosParsers::ORIENTACAO_PAPEL]==AtributosParsers::RETRATO);
 paisagem_rb->setChecked(params_config[AtributosParsers::CONFIGURACAO][AtributosParsers::ORIENTACAO_PAPEL]==AtributosParsers::PAISAGEM);

 margem=params_config[AtributosParsers::CONFIGURACAO][AtributosParsers::MARGEM_PAPEL].split(",");

 marg_esq->setValue((margem.count() >= 1 ? margem[0].toFloat() : 10));
 marg_topo->setValue((margem.count()>= 2 ? margem[1].toFloat() : 10));
 marg_dir->setValue((margem.count() >= 3 ? margem[2].toFloat() : 10));
 marg_base->setValue((margem.count() >= 4 ? margem[3].toFloat() : 10));

 //Efetiva as configurações do formulário aplicando-as às classes interessadas
 this->aplicarConfiguracao();
}
//-----------------------------------------------------------
void ConfGeralWidget::salvarConfiguracao()
{
 try
 {
  map<QString, map<QString, QString> >::iterator itr, itr_end;
  QString sch_widget, sch_arquivo, dir_raiz;

  dir_raiz=AtributosGlobais::DIR_CONFIGURACOES +
           AtributosGlobais::SEP_DIRETORIO;

  sch_widget=dir_raiz +
             AtributosGlobais::DIR_ESQUEMAS +
             AtributosGlobais::SEP_DIRETORIO +
             AtributosParsers::WIDGET +
             AtributosGlobais::EXT_ESQUEMA;

  sch_arquivo=dir_raiz +
              AtributosGlobais::DIR_ESQUEMAS +
              AtributosGlobais::SEP_DIRETORIO +
              AtributosParsers::ARQUIVO +
              AtributosGlobais::EXT_ESQUEMA;

  //Armazena no mapa de parâmetros de configuração os valores dos widgets no formulário
  params_config[AtributosParsers::CONFIGURACAO][AtributosParsers::TAM_GRADE]=QString("%1").arg(tam_grade_spb->value());
  params_config[AtributosParsers::CONFIGURACAO][AtributosParsers::TAM_LISTA_OP]=QString("%1").arg(tam_lista_spb->value());
  params_config[AtributosParsers::CONFIGURACAO][AtributosParsers::INTERVALO_SALVAR_AUTO]=QString("%1").arg(salvar_mod_chk->isChecked() ? salvar_mod_spb->value() : 0);
  params_config[AtributosParsers::CONFIGURACAO][AtributosParsers::TIPO_PAPEL_IMP]=QString("%1").arg(papel_cmb->currentIndex());
  params_config[AtributosParsers::CONFIGURACAO][AtributosParsers::ORIENTACAO_PAPEL]=(retrato_rb->isChecked() ? AtributosParsers::RETRATO : AtributosParsers::PAISAGEM);

  unidade_cmb->setCurrentIndex(0);
  params_config[AtributosParsers::CONFIGURACAO][AtributosParsers::MARGEM_PAPEL]=QString("%1,%2,%3,%4").arg(marg_esq->value())
                                                                             .arg(marg_topo->value())
                                                                             .arg(marg_dir->value())
                                                                             .arg(marg_base->value());

  params_config[AtributosParsers::CONFIGURACAO][AtributosParsers::SALVAR_SESSAO]=(salvar_sessao_chk->isChecked() ? "1" : "");
  params_config[AtributosParsers::CONFIGURACAO][AtributosParsers::SALVAR_WIDGETS]=(salvar_wgts_chk->isChecked() ? "1" : "");
  params_config[AtributosParsers::CONFIGURACAO][AtributosParsers::ARQUIVO]="";
  params_config[AtributosParsers::CONFIGURACAO][AtributosParsers::WIDGET]="";

  itr=params_config.begin();
  itr_end=params_config.end();

  //Salvando as configurações da sessão e dos widgets
  while(itr!=itr_end)
  {
   if(salvar_sessao_chk->isChecked() &&
     (itr->first).contains(QRegExp(QString("(") +
                           AtributosParsers::ARQUIVO +
                           QString(")([0-9]+)"))))
   {
    params_config[AtributosParsers::CONFIGURACAO][AtributosParsers::ARQUIVO]+=
    ParserEsquema::obterDefinicaoObjeto(sch_arquivo, itr->second);
   }
   else if(salvar_wgts_chk->isChecked() &&
     (itr->first).contains(QRegExp(QString("(") +
                           AtributosParsers::WIDGET +
                           QString(")([0-9]+)"))))
   {
    params_config[AtributosParsers::CONFIGURACAO][AtributosParsers::WIDGET]+=
    ParserEsquema::obterDefinicaoObjeto(sch_widget, itr->second);
   }

   itr++;
  }

  //Salva a configuração em arquivo
  ConfBaseWidget::salvarConfiguracao(AtributosGlobais::CONF_GERAL);
 }
 catch(Excecao &e)
 {
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}
//-----------------------------------------------------------
void ConfGeralWidget::aplicarConfiguracao(void)
{
 //Aplica as configurações de papel à cena
 CenaObjetos::definirConfiguracaoPagina(static_cast<QPrinter::PaperSize>(papel_cmb->itemData(papel_cmb->currentIndex()).toInt()),
                                        (retrato_rb->isChecked() ? QPrinter::Portrait : QPrinter::Landscape),
                                        QRectF(marg_esq->value(), marg_topo->value(),
                                               marg_dir->value(), marg_base->value()));

 //Aplica a configuração de grade à cena
 CenaObjetos::definirGrade(tam_grade_spb->value());

 //Aplica as configurações à lista de operações
 ListaOperacoes::definirTamanhoMaximo(tam_lista_spb->value());
}
//-----------------------------------------------------------
void ConfGeralWidget::restaurarPadroes(void)
{
 try
 {
  //Restaura as configurações padrão e recarrega o arquivo restaurado
  ConfBaseWidget::restaurarPadroes(AtributosGlobais::CONF_GERAL);
  this->carregarConfiguracao();
 }
 catch(Excecao &e)
 {
  throw Excecao(e.obterMensagemErro(),e.obterTipoErro(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
 }
}
//-----------------------------------------------------------
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
//***********************************************************
