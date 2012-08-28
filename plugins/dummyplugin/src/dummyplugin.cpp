#include "dummyplugin.h"
//***********************************************************
DummyPlugin::DummyPlugin(void)
{

}
//-----------------------------------------------------------
void DummyPlugin::executarPlugin(ModeloWidget *modelo)
{
 cout << "plugin carregado!" << endl;
}
//***********************************************************
Q_EXPORT_PLUGIN2(dummyplugin, DummyPlugin)
