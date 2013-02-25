#include "dummyplugin.h"
#include "exception.h"
#include "messagebox.h"

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

	configurePluginInfo(getPluginTitle(),
											getPluginVersion(),
											getPluginAuthor(),
											getPluginDescription(),

											GlobalAttributes::PLUGINS_DIR +
											GlobalAttributes::DIR_SEPARATOR +
											QString("dummyplugin") +
											GlobalAttributes::DIR_SEPARATOR + "dummyplugin.png");
}

QString DummyPlugin::getPluginTitle(void)
{
	return(trUtf8("Dummy Plugin"));
}


QString DummyPlugin::getPluginVersion(void)
{
	return("0.1");
}

QString DummyPlugin::getPluginAuthor(void)
{
	return("Raphael A. Silva");
}

QString DummyPlugin::getPluginDescription(void)
{
	return(trUtf8("A dummy plugin only to test the pgModeler plugin structure."));
}

void DummyPlugin::showPluginInfo(void)
{
	plugin_info_frm->show();
}

void DummyPlugin::executePlugin(ModelWidget *)
{
	MessageBox msgbox;
	msgbox.show(trUtf8("Plugin successfully loaded!"),
							trUtf8("Plugin successfully loaded! Check the file PLUGINS.md to know how to create your own plugins."),
							MessageBox::INFO_ICON);
}

Q_EXPORT_PLUGIN2(dummyplugin, DummyPlugin)
