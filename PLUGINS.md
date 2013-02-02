Plug-ins
--------

This document describes a simple way to create third party plugins on pgModeler. ATTENTION: Any plugin must be compiled using the most recent pgModeler source and libs.

Basic Rules
-----------

To be used as a plugin your class must:

* Inherit the class "PgModelerPlugin"
* Use the macro Q_INTERFACES in its declaration and the macro Q_EXPORT_PLUGIN2 on its implementation.
* Overload all pure virtual methods from PgModelerPlugin

Plugin directory structure
--------------------------

The plugins in pgModeler must be within the "plugins" folder in its own directory and must have the following basic structure:


     [PGMODELER_PLUGINS_DIR]/
			    |
			    + - pluginA/
				       + - (lib)*(pluginA.)(so|dylib|dll) (library)
				       |
				       + - pluginA.png (icon)

* Library: it is the shared object that represents the plugin. The prefix (lib) and suffix (so|dylib|dll) are plataform dependent.
* Icon: it is a PNG image that represents the plugin on the plugins toolbar.

The dummy plugin
----------------

To assist in developing a plugin the DummyPlugin project was created. This basic project has the necessary coding to create plugins. It is recommended to use this as a reference in creating your own plugins.

Advanced plugins
---------------

To create advanced plugins for particular tasks you need to check the pgModeler API documentation. The documentation is well structured but the UI library is still in Portuguese language but the code refactoring is in progress. If you want to create a plugin and have questions please contact me via e-mail.
