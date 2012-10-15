#include "crashhandler.h"
const char CrashHandler::CHR_DELIMITADOR=static_cast<char>(3);
//***********************************************************
CrashHandler::CrashHandler(QWidget *parent, Qt::WindowFlags f) : QDialog(parent, f)
{
 ifstream entrada;
 QString buf;
 char lin[1024];

 setupUi(this);
 connect(cancelar_btn, SIGNAL(clicked(void)), this, SLOT(close(void)));
 connect(criar_btn, SIGNAL(clicked(void)), this, SLOT(gerarRelatorio(void)));
 connect(acoes_txt, SIGNAL(textChanged(void)), this, SLOT(habilitarGeracao(void)));

 //Abre o arquivo o qual armazena a stack trace do ultimo travamento
 entrada.open(AtributosGlobais::DIR_TEMPORARIO +
              AtributosGlobais::SEP_DIRETORIO +
              AtributosGlobais::ARQ_STACKTRACE);

 //Le cada linha do arquivo e concatena ao buffer
 while(entrada.is_open() && !entrada.eof())
 {
  entrada.getline(lin, sizeof(lin), '\n');
  buf += QString("%1\n").arg(lin);
 }

 entrada.close();

 //Remove o arquivo stack trace
 QDir arq_stk;
 arq_stk.remove(AtributosGlobais::DIR_TEMPORARIO +
                AtributosGlobais::SEP_DIRETORIO +
                AtributosGlobais::ARQ_STACKTRACE);

 //Exibe o buffer no widget de stack trace
 stack_txt->setPlainText(buf);

 //Cria um destacador de sintaxe para o modelo
 dest_modelo_txt=new DestaqueSintaxe(modelo_txt, false);
 dest_modelo_txt->carregarConfiguracao(AtributosGlobais::DIR_CONFIGURACOES +
                                       AtributosGlobais::SEP_DIRETORIO +
                                       AtributosGlobais::CONF_DESTAQUE_XML +
                                       AtributosGlobais::EXT_CONFIGURACAO);

 QDir dir_tmp=QDir(AtributosGlobais::DIR_TEMPORARIO, "*.dbm", QDir::Name, QDir::Files | QDir::NoDotAndDotDot);
 dir_tmp.setSorting(QDir::Time);
 QStringList lista=dir_tmp.entryList();

 if(!lista.isEmpty())
 {
  //Abre o modelo temporário modificado pela última vez
  entrada.open(AtributosGlobais::DIR_TEMPORARIO +
               AtributosGlobais::SEP_DIRETORIO + lista[0]);

  //Le cada linha do arquivo e concatena ao buffer
  buf.clear();
  while(entrada.is_open() && !entrada.eof())
  {
   entrada.getline(lin, sizeof(lin), '\n');
   buf += QString("%1\n").arg(lin);
  }

  entrada.close();
  modelo_txt->setPlainText(QString::fromUtf8(buf));
 }
}
//-----------------------------------------------------------
void CrashHandler::habilitarGeracao(void)
{
 criar_btn->setEnabled(!acoes_txt->text().isEmpty());
}
//-----------------------------------------------------------
void CrashHandler::carregarRelatorio(const QString &arquivo)
{
 ifstream entrada;
 QFileInfo fi;
 char *buf=NULL;
 CaixaMensagem caixa;

 //Abre o arquivo .crash
 fi.setFile(arquivo);
 entrada.open(arquivo);

 titulo_lbl->setText(trUtf8("pgModeler crash file analysis"));
 criar_btn->setVisible(false);
 msg_lbl->clear();

 //Caso o arquivo  não foi aberto com sucesso, exibe um erro
 if(!entrada.is_open())
  caixa.show(trUtf8("Error"), Excecao::obterMensagemErro(ERR_PARSERS_ARQDIRNAOCARREGADO).arg(arquivo), CaixaMensagem::ICONE_ERRO);
 else
 {
  QByteArray buf_descomp;
  QString buf_aux, str_aux;
  int i, idx;
  QTextEdit *txt_widgets[]={ acoes_txt, modelo_txt , stack_txt};

  //Exibe informações do arquivo
  msg_lbl->setText(trUtf8("File: %1\nSize: %2 bytes\n\n").arg(arquivo).arg(fi.size()));

  //Cria um buffer no tamanho total do arquivo
  buf=new char[fi.size()];
  //Le todos os bytes do arquivo
  entrada.read(buf, fi.size());
  entrada.close();

  //Descompacta o buffer lido
  buf_descomp.append(buf, fi.size());
  buf_descomp=qUncompress(buf_descomp);

  //Desaloca o buffer original
  delete[](buf);
  buf=NULL;

  //Obtém o buffer descompactado
  buf_aux=QString(buf_descomp.data());
  i=idx=0;

  //Varre todo o buffer separando jogando cada parte do buffer em sua respectiva seção
  while(i < buf_aux.size() && idx <= 2)
  {
   if(buf_aux.at(i).ascii()!=CHR_DELIMITADOR)
    str_aux.append(buf_aux.at(i));
   else
   {
    txt_widgets[idx++]->setPlainText(QString::fromUtf8(str_aux));
    str_aux.clear();
   }
   i++;
  }
 }
}
//-----------------------------------------------------------
void CrashHandler::gerarRelatorio(void)
{
 CaixaMensagem caixa;
 QByteArray buf, buf_comp;
 ofstream saida;

 //Configura o caminho para o arquivo .crash gerado
 QString arq_crash=(AtributosGlobais::DIR_TEMPORARIO +
                    AtributosGlobais::SEP_DIRETORIO +
                    AtributosGlobais::ARQ_CRASH_HANDLER).arg(QDateTime::currentDateTime().toString("_yyyyMMdd_hhmm"));

 //Abre o arquivo para gravação
 saida.open(arq_crash);

 //Caso não possa ser aberto, exibe um erro
 if(!saida.is_open())
  caixa.show(trUtf8("Error"), Excecao::obterMensagemErro(ERR_PGMODELER_ARQNAOGRAVADO).arg(arq_crash), CaixaMensagem::ICONE_ERRO);
 else
 {
  //Insere a descrição das ações no buffer
  buf.append(acoes_txt->toPlainText().toUtf8());
  buf.append(CHR_DELIMITADOR);

  //Anexa o modelo caso o usuário tenha selecionado
  if(anexar_mod_chk->isChecked())
   buf.append(modelo_txt->toPlainText().toUtf8());
  buf.append(CHR_DELIMITADOR);

  //Anexa a stack trace
  buf.append(stack_txt->toPlainText().toUtf8());
  buf.append(CHR_DELIMITADOR);

  //Compacta o buffer (usando a zlib) numa taxa de compressão 8
  buf_comp=qCompress(buf, 8);

  //Salva o buffer compactado
  saida.write(buf_comp.data(), buf_comp.size());
  saida.close();

  //Exibe a imagem de sucesso e fecha o crash handler
  caixa.show(trUtf8("Information"), trUtf8("Crash report successfuly generated! Please send the file '%1' to %2 in order be debugged. Thank you for the collaboration!").arg(arq_crash).arg("rkhaotix@gmail.com"), CaixaMensagem::ICONE_INFO);
  this->close();
 }
}
//***********************************************************
