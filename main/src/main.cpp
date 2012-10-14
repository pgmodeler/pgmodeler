#include "formprincipal.h"
#include "aplicacao.h"
#include <QTranslator>

#ifndef Q_OS_WIN
 #include "execinfo.h"
#endif

void executarCrashHandler(int)
{
 ofstream saida;
 QString lin, cmd;

 /** A função backtrace não é existe até o momento no MingW (Windows) desta forma
     o trecho que gera a stacktrace do programa só é ativado em Linux/Unix **/
 #ifndef Q_OS_WIN
  void *pilha[20];
  size_t tam_pilha, i;
  char **simbolos=NULL;

  //Obtém os simbolos da stack trace (até 20 itens)
  tam_pilha = backtrace(pilha, 20);
  simbolos = backtrace_symbols(pilha, tam_pilha);
  cmd="crashhandler";
 #else
  cmd="crashhandler.exe";
 #endif

 //Cria o arquivo que armazenará a stack trace
 saida.open(AtributosGlobais::DIR_TEMPORARIO +
            AtributosGlobais::SEP_DIRETORIO +
            AtributosGlobais::ARQ_CRASH_HANDLER);

 //Caso o arquivo esteja aberto
 if(saida.is_open())
 {
  lin=QString("** pgModeler [v%1] crashed after receive signal: %2 **\n\n").arg(AtributosGlobais::VERSAO_PGMODELER).arg("SIGSEGV");
  saida.write(lin.toStdString().c_str(), lin.size());

  #ifndef Q_OS_WIN
   //Grava cada linha da stack trace no arquivo
   for(i=0; i < tam_pilha; i++)
   {
    lin=QString(simbolos[i]) + QString("\n");
    saida.write(lin.toStdString().c_str(), lin.size());
   }
   //Desaloca a stack trace
   free(simbolos);
  #else
    lin=QString("** Stack trace temporary unavailable on Windows system **");
    saida.write(lin.toStdString().c_str(), lin.size());
  #endif

  saida.close();
 }

 //Executa o comando crashhandler (que obrigatoriamente deve estar na mesma pasta do pgModeler)
 cmd=QApplication::applicationDirPath() + AtributosGlobais::SEP_DIRETORIO + cmd;
 system(cmd.toStdString().c_str());
 exit(1);
}


int main(int argc, char **argv)
{
 try
 {
  //Captura o sinal de segmentation fault e inicia o crashhandler
  signal(SIGSEGV, executarCrashHandler);

  Aplicacao app(argc,argv);
  QTranslator tradutor;
  QString ling_fallback="pgmodeler.en_US";

  app.addLibraryPath(AtributosGlobais::DIR_PLUGINS);

  /** issue#23 **/
  /* Caso o pgModeler não encontre um arquivo de tradução da lingua nativa do sistema o qual está
     sendo executado será carregado o arquivo pgmodeler.en_US (lingua fallback) */
  if(!tradutor.load(QString("pgmodeler.") + QLocale::system().name(), AtributosGlobais::DIR_LINGUAS))
   //Carrega a lingua fallback
   tradutor.load(ling_fallback, AtributosGlobais::DIR_LINGUAS);

  //Instala o tradutor na aplicação
  app.installTranslator(&tradutor);

  //Carregando uma splash screen
  QPixmap pixmap(":imagens/imagens/pgmodeler_logo.png");
  QPixmap alfa(":imagens/imagens/pgmodeler_logo_alfa.png");
  pixmap.setAlphaChannel(alfa);

  //Desenha o texto da versão atual no canto inferior direito do pixmap
  QFont fnt;
  QPainter p;
  fnt.setFamily("Dejavu Sans");
  fnt.setBold(true);
  fnt.setPointSize(7.5f);
  p.begin(&pixmap);
  p.setFont(fnt);
  p.setPen(QColor(255,255,255));
  p.drawText(QPointF(pixmap.size().width()-45, pixmap.size().width()-17), QString("v%1 ").arg(AtributosGlobais::VERSAO_PGMODELER));
  p.end();

  QSplashScreen splash(pixmap);
  splash.show();
  splash.repaint();

  /* Aloca o formulário principal.
     Durante a sua alocação pode ser disparadas uma série de exceções que a
     aplicação não é capaz de caputar pois o formulário ainda não foi atribuído
     a esta, desta forma, a alocação do formulário e feita dentro de um
     try-catch para possível captura de erros. A aplicação será abortada
     e o erro mostrado no console caso ocorra. */
  FormPrincipal fmain;

  //Atribui o formulário alocado à aplicação
  app.setMainWidget(&fmain);

  //Indicando para a splash screen que ela deve fechar quando a janela principal for exibida
  splash.finish(&fmain);

  //Exibe o formulário principal e prossegue com a execução da aplicação
  fmain.showMaximized();

  //Executa a aplicação
  app.exec();

  return(0);
 }
 //Caso um erro seja capturado durante a inicialização da aplicação
 catch(Excecao &e)
 {
  //Retorna o código de erro da última exceção e aborta a aplicação
  return(e.obterTipoErro());
 }
}
