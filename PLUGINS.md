PostgreSQL Database Modeler - pgModeler
---------------------------------------

This document describes a simple way how to create third party plugins on pgModeler. ATTENTION: Any plugin must be compiled using the most current source of pgModeler.

Basic Rules
-----------

To be used as a plugin your class must:

1) Inherit the class "PgModelerPlugin"
2) Use the macro Q_INTERFACES in its declaration.
3) Overloadthe methods PgModelerPlugin::obterRotuloPlugin(void) and PgModelerPlugin::executarPlugin(ModeloWidget *model)

Plugin directory structure
--------------------------

The plugins in pgModeler must be within the "plugins" folder in its own directory and must have the following basic structure:


     [PGMODELER_PLUGINS_DIR]/
                            +- pluginA/
                                      +---(lib)*(pluginA.)(so|dylib|dll) (library)
                                      +---pluginA.png                    (icon)

* Library: is the shared object that represents the plugin. The prefix (lib) and suffix (so|dylib|dll) are plataform dependent.
* Icon: It is a PNG image that represents the plugin on the plugins toolbar.

The dummy plugin
----------------

To assist in developing a plugin the DummyPlugin project was created. This basic project has the necessary coding to create plugins. It is recommended to use this as a reference in creating your own plugins.

Advanced plugins
---------------

To create advanced plugins you need to study the source code of pgModeler (particularly the headers). The code is well documented but in Portuguese. If you want to create a plugin and have questions please contact me via e-mail.

In the near future all the code will be cataloged and online documentation in English will be available to developers (help wanted!).