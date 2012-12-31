#include "dummyplugin.h"
#include "exception.h"
#include "caixamensagem.h"

DummyPlugin::DummyPlugin(void)
{
 QTranslator *translator=new QTranslator;

 translator->load(QString("dummyplugin.") + QLocale::system().name(),
                GlobalAttributes::PLUGINS_DIR +
                GlobalAttributes::DIR_SEPARATOR +
                QString("dummyplugin") +
                GlobalAttributes::DIR_SEPARATOR +
                QString("lang"));

 QCoreApplication::installTranslator(translator);
}

QString DummyPlugin::getPluginLabel(void)
{
 return(trUtf8("Dummy Plugin"));
}

void DummyPlugin::executePlugin(ModeloWidget *)
{
 CaixaMensagem msgbox;
 msgbox.show(trUtf8("Plugin successfully loaded!"),
                trUtf8("Plugin successfully loaded! Check the file PLUGINS.md to know how to create your own plugins."),
                CaixaMensagem::ICONE_INFO);
}

Q_EXPORT_PLUGIN2(dummyplugin, DummyPlugin)
