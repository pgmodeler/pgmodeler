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

  //Desenha o texto da versão atual no canto inferior direito do pixmap
  QFont fnt;
  QPainter p;
  fnt.setFamily("Dejavu Sans");
  fnt.setBold(true);
  fnt.setPointSize(7.5f);
  p.begin(&pixmap);
  p.setFont(fnt);
  p.setPen(QColor(255,255,255));
  p.drawText(QPointF(pixmap.size().width()-45, pixmap.size().width()-17), QString("v%1").arg(AtributosGlobais::VERSAO_PGMODELER));
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
