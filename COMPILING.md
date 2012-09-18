PostgreSQL Database Modeler - pgModeler
---------------------------------------

This document describes a simple way how to compile and run the pgModeler. Please read it carefully so that no step is forgotten compromising the entire process.

Compiling Requirements
-------------------------

* GCC Development Tools
* Qt 4.7.x library or above (qt.nokia.com)
* LibXML2 library (www.xmlsoft.org)
* libpq library (www.postgresql.org)
* pgModeler source (github.com/pgmodeler/pgmodeler/tags)

Compiling
----------

Before starting the compilation, check in the file 'pgmodeler.pro' if the variables GLOBAL_INCLUDES and GLOBAL_LIBS points to the correct paths relative to your system. Change the value of these variables only in the section for your operating system. 
To compile the pgModeler type (in the root folder of the source code):

     $ qmake pgmodeler.pro
     $ make

If the build is successful the executable 'pgmodeler' will be created in the build/ directory. Copy it to the folder you want and do not forget to also copy the folders schemas/, conf/, lang/ and plugins/ in the same directory.

Environment Variables
---------------------

By default pgModeler accesses the folders schemas/, conf/ and lang/ on the working directory, but you can change these settings. Four own environment variables are used for this purpose. If you want to configure directories cited in deferent paths use the following variables:

* PGMODELER_SCHEMAS_DIR -> stores the full path the directory schemas/
* PGMODELER_CONF_DIR -> stores the full path the directory conf/
* PGMODELER_LANG_DIR -> stores the full path the directory lang/
* PGMODELER_PLUGINS_DIR -> stores the full path the directory plugins/

Additionally, the pgModeler need to know where to find the libraries required for the execution (Qt Library, LibXML and libpq) for which you should enter the full path in the variable LD_LIBRARY_PATH (Unix) or PATH (Windows), if not already configured.

To provide the interface translation pgModeler checks the variable LANG (Unix). To use a specific translation, change the value of this variable. So far, the pgModeler only supports the languages Brazilian Portuguese (pt_BR) and U.S. English (en_US).

Finally, as a form of convenience, the script 'start-pgmodeler.(sh | bat)' was created to assist the initialization of pgModeler so it is not necessary to call the executable directly or change the environment variables on your operating system.

Note: You may need to edit the parameters of the script depending on configuration of your operating system.
