#include "formprincipal.h"
#include "aplicacao.h"
#include <QTranslator>

int main(int argc, char **argv)
{
 try
 {
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

  QSplashScreen splash(pixmap);

  QFont fnt=splash.font();
  fnt.setFamily("Dejavu Sans");
  fnt.setBold(true);
  fnt.setPointSize(7.5f);
  splash.setFont(fnt);

  //Exibe a versão do pgmodeler na base do splash
  splash.showMessage(QString("       v%1\n").arg(AtributosGlobais::VERSAO_PGMODELER), Qt::AlignBottom | Qt::AlignHCenter, Qt::white);
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
