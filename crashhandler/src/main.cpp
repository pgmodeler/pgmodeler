#include "crashhandler.h"
#include <QApplication>
#include <QTranslator>

int main(int argc, char **argv)
{
 try
 {
  QApplication app(argc,argv);
  QTranslator tradutor;
  QString ling_fallback="pgmodeler.en_US";

  /** issue#23 **/
  /* Caso o pgModeler não encontre um arquivo de tradução da lingua nativa do sistema o qual está
     sendo executado será carregado o arquivo pgmodeler.en_US (lingua fallback) */
  if(!tradutor.load(QString("pgmodeler.") + QLocale::system().name(), GlobalAttributes::LANGUAGES_DIR))
   //Carrega a lingua fallback
   tradutor.load(ling_fallback, GlobalAttributes::LANGUAGES_DIR);

  //Instala o tradutor na aplicação
  app.installTranslator(&tradutor);

  CrashHandler crashhandler;

  //Atribui o formulário alocado �  aplicação
  app.setMainWidget(&crashhandler);

  if(argc > 1)
   crashhandler.carregarRelatorio(argv[1]);

  //Exibe o formulário principal e prossegue com a execução da aplicação
  crashhandler.show();

  //Executa a aplicação
  app.exec();

  return(0);
 }
 //Caso um erro seja capturado durante a inicialização da aplicação
 catch(Exception &e)
 {
  //Retorna o código de erro da última exceção e aborta a aplicação
  return(e.getErrorType());
 }
}
