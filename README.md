PostgreSQL Database Modeler - pgModeler
---------------------------------------

PostgreSQL Database Modeler, or simply, pgModeler is an open source tool for modeling databases that merges the classical concepts of entity-relationship diagrams with specific features that only PostgreSQL implements. The pgModeler translates the models created by the user to SQL code and apply them onto database clusters from version 8.0 to 9.1.

Please, let me know how the pgModeler is working on your system! Help to improve this project, give your feedback about the software or report any bug at [Issues](https://github.com/pgmodeler/pgmodeler/issues) page. Additionaly, follow the pgModeler profile on [Facebook](https://www.facebook.com/pgmodeler) or [Twitter](https://twitter.com/pgmodeler) and be up-to-date with new features, fixes and releases.

Licensing
---------

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation version 3.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See [LICENSE](https://github.com/pgmodeler/pgmodeler/blob/master/LICENSE) for details.

Compiling/Installation
----------------------

For details about installation process from source code see [COMPILING.md](https://github.com/pgmodeler/pgmodeler/blob/master/COMPILING.md). If you don't want to compile pgModeler there are binaries available for download at [Downloads](https://github.com/pgmodeler/pgmodeler/downloads) section.

Running pgModeler
-----------------

The pgModeler can be executed directly from de executable but it is necessary change some environment variables, see [COMPILING.md](https://github.com/pgmodeler/pgmodeler/blob/master/COMPILING.md). For convinience there are two scripts used to startup pgModeler.

* Windows users: use the ```start-pgmodeler.bat``` script. By default this script consider that pgModeler is installed on ```c:\pgmodeler``` if you desire to set other location edit the script and change the PGMODELER_* environment variables.

* Linux/Unix users: use the ```start-pgmodeler.sh``` script. By default this script consider that pgModeler is installed on current working directory AKA ```pwd``` if you desire to set other location edit the script and change the PGMODELER_* environment variables.

Known bugs
----------

* Broken compilation process on MacOSX. ([issue#10](https://github.com/pgmodeler/pgmodeler/issues/10))
* PgModeler, when compiled on Qt 4.7.x, crashes after activating the print dialog. [(issue#1)](https://github.com/pgmodeler/pgmodeler/issues/1)

Change Log
----------

[ v0.3.4 ]

* [New]: added chinese UI translation (provided by [gjunming](https://github.com/gjunming)).
* [New]: added basic support for PostGiS 2.0 only data types: box2d, box3d, geometry and geography (suggested by [george-silva](https://github.com/george-silva) on [issue#28](https://github.com/pgmodeler/pgmodeler/issues/28))(EXPERIMENTAL). Note: when using these data types make sure that PostGiS extension is installed on database cluster since pgModeler WILL NOT install it automatically or generate the command to do it!
* [New]: added a model restoration feature to reopen models after unexpected quit (crash).
* [New]: added a crash handler to pgModeler. Now signal SIGSEGV is trapped (in most cases) and the crash handler pops up permiting the user to generate an error report. (EXPERIMENTAL)
* [New]: to facilitate the error reporting exceptions stack now can be showed in text format. Users can post the complete error stack when creating an issue.
* [New]: icon added to pgModeler executable (Windows only)
* [Change]: update on pt_BR translation file.
* [Change]: removed "pgmodeler" prefix from translation files.
* [Change]: added the field "Underline" on textbox editing form.
* [Fix]: corrected the "AlwayOnTop" bug on model overview widget. ([issue#30](https://github.com/pgmodeler/pgmodeler/issues/30))
* [Fix]: little fix on startup scripts. Corrected de PGMODELER_ROOT on both Linux and Windows systems. ([issue#29](https://github.com/pgmodeler/pgmodeler/issues/29))
* [Fix]: corrected the referece to environment variables PGMODELER_*. Now pgModeler search for necessary paths on current directory if some of these variables are not set.
* [Fix]: corrected the validation of UTF-8 names that have 3 bytes length.
* [Fix]: corrected the sources path reference on project (.pro) files. Now lupdate command do not generates empty TS files.
* [Fix]: corrected a bug that was causing crash where user try to edit protected objects.
* [Fix]: corrected the exhibition of UTF-8 messages on ```throw``` statements.

The complete change log can be found on [CHANGELOG.md](https://github.com/pgmodeler/pgmodeler/blob/master/CHANGELOG.md) file.

Older Releases/Code
-------------------

The older code and releases of pgModeler can be found at [Sourceforge.net](http://sourceforge.net/projects/pgmodeler)
