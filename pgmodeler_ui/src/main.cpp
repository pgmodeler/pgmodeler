#include "formprincipal.h"
#include "aplicacao.h"
#include <QTranslator>

int main(int argc, char **argv)
{
 try
 {
  Aplicacao app(argc,argv);
  QString local = QLocale::system().name();
  QTranslator tradutor;

  //Obtém os valores das variáveis de ambiente
  QString dir_conf=getenv("PGMODELER_CONF_DIR"),
          dir_sch=getenv("PGMODELER_SCHEMAS_DIR"),
          dir_lang=getenv("PGMODELER_LANG_DIR");

  /* Caso alguma das variáveis de ambiente esteja setada
     atribui a mesma a configuração global respectiva sobrescrevendo
     a configuração padrão */
  if(!dir_conf.isEmpty())
   AtributosGlobais::DIR_CONFIGURACOES=dir_conf.replace("\\","/");

  if(!dir_sch.isEmpty())
   AtributosGlobais::DIR_RAIZ_ESQUEMAS=dir_sch.replace("\\","/");

  if(!dir_lang.isEmpty())
   AtributosGlobais::DIR_RAIZ_ESQUEMAS=dir_lang.replace("\\","/");

  //Carrega o arquivo de tradução da interface de acordo com o locale do sistema
  tradutor.load(QString("pgmodeler.") + local, AtributosGlobais::DIR_LINGUAS);

  //Instala o tradutor na aplicação
  app.installTranslator(&tradutor);

  /* Aloca o formulário principal.
     Durante a sua alocação pode ser disparadas uma série de exceções que a
     aplicação não é capaz de caputar pois o formulário ainda não foi atribuído
     a esta, desta forma, a alocação do formulário e feita dentro de um
     try-catch para possível captura de erros. A aplicação será abortada
     e o erro mostrado no console caso ocorra. */
  FormPrincipal fmain;//=new FormPrincipal;

  //Atribui o formulário alocado à aplicação
  app.setMainWidget(&fmain);

  //Exibe o formulário principal e prossegue com a execução da aplicação
  fmain.showMaximized();

  //Executa a aplicação
  app.exec();

  //Desloca a janela principal ao término do loop principal
  //delete(fmain);
  return(0);
 }
 //Caso um erro seja capturado durante a inicialização da aplicação
 catch(Excecao &e)
 {
  deque<Excecao> excecoes;
  deque<Excecao>::iterator itr, itr_end;
  unsigned idx=0;

  //Obtém a lista de exceções geradas
  e.obterListaExcecoes(excecoes);
  itr=excecoes.begin();
  itr_end=excecoes.end();

  //Exibe todas as exceções no console
  while(itr!=itr_end)
  {
   cout << "[" << idx << "] " << itr->obterArquivo().toStdString() << " (" << itr->obterLinha().toStdString() << ")" << endl;
   cout << "    " << itr->obterLocal().toStdString() << endl;
   cout << "      [" << Excecao::obterNomeErro(itr->obterTipoErro()).toStdString() << "] ";
   cout << itr->obterMensagemErro().toStdString() << endl << endl;
   itr++; idx++;
  }

  //Retorna o código de erro da última exceção e aborta a aplicação
  return(e.obterTipoErro());
 }
}
