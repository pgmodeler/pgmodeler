#include "crashhandler.h"
#include <QApplication>
#include <QTranslator>

int main(int argc, char **argv)
{
	try
	{
		QApplication app(argc,argv);
		QTranslator translator;

		//Loads the ui translation for crashhandler
		translator.load(QLocale::system().name(), GlobalAttributes::LANGUAGES_DIR);
		app.installTranslator(&translator);

		CrashHandler crashhandler;

		if(argc > 1)
			crashhandler.loadReport(argv[1]);

		crashhandler.show();
		app.exec();

		return(0);
	}
	catch(Exception &e)
	{
		return(e.getErrorType());
	}
}
