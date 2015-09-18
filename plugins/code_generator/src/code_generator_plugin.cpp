#include "code_generator_plugin.h"
#include "code_generator_widget.h"
#include "exception.h"
#include "messagebox.h"
#include "python_daos_generator.h"

CodeGeneratorPlugin::CodeGeneratorPlugin(void)
{
    this->widget = new CodeGeneratorWidget();
    this->generator = new PythonDAOsGenerator();
    ((CodeGeneratorWidget*)this->widget)->setGenerator((BaseCodeGenerator *) this->generator);

    configurePluginInfo(
        getPluginTitle(),
        getPluginVersion(),
        getPluginAuthor(),
        getPluginDescription(),

        GlobalAttributes::PLUGINS_DIR +
        GlobalAttributes::DIR_SEPARATOR +
        QString("code_generator") +
        GlobalAttributes::DIR_SEPARATOR + QString("code_generator.png")
    );
}

CodeGeneratorPlugin::~CodeGeneratorPlugin(void)
{
    delete ((CodeGeneratorWidget*)this->widget);
    this->widget = 0;
}

QString CodeGeneratorPlugin::getPluginTitle(void)
{
    return(trUtf8("CodeGenerator"));
}

QString CodeGeneratorPlugin::getPluginVersion(void)
{
    return(QString("0.1"));
}

QString CodeGeneratorPlugin::getPluginAuthor(void)
{
  return(QString("Riccardo Campisano"));
}

QString CodeGeneratorPlugin::getPluginDescription(void)
{
    return(trUtf8("This plugin automatically gerate code."));
}

void CodeGeneratorPlugin::showPluginInfo(void)
{
    plugin_info_frm->show();
}

void CodeGeneratorPlugin::executePlugin(ModelWidget *model)
{
    if(!model)
        throw Exception(trUtf8("This plugin must be executed with at least one model opened!"),ERR_CUSTOM,__PRETTY_FUNCTION__,__FILE__,__LINE__);

    ((CodeGeneratorWidget*)this->widget)->show(model->getDatabaseModel(), model->getOperationList());
}

QKeySequence CodeGeneratorPlugin::getPluginShortcut(void)
{
  return(QKeySequence(QString("Ctrl+Alt+G")));
}
