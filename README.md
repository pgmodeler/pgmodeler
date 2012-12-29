PostgreSQL Database Modeler - pgModeler
---------------------------------------

PostgreSQL Database Modeler, or simply, pgModeler is an open source tool for modeling databases that merges the classical concepts of entity-relationship diagrams with specific features that only PostgreSQL implements. The pgModeler translates the models created by the user to SQL code and apply them onto database clusters from version 8.0 to 9.1.

Please, let me know how the pgModeler is working on your system! Help to improve this project, give your feedback about the software or report any bug at [Issues](https://github.com/pgmodeler/pgmodeler/issues) page. Additionaly, follow the pgModeler profile on [Facebook](https://www.facebook.com/pgmodeler) or [Twitter](https://twitter.com/pgmodeler) and be up-to-date with new features, fixes and releases.

Licensing
---------

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation version 3.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See [LICENSE](https://github.com/pgmodeler/pgmodeler/blob/master/LICENSE) for details.

Donate to pgModeler
-------------------

Making a software is no easy task! With pgModeler was no different. Much effort, coffee, chocolate, time and knowledge has been devoted to the project so that a usable and constantly improved product is delivered to the community free of charge.

Despite all many feature contribution all open source project needs sometimes a financial incentive, this way the developer realizes that his product is even more valuable.

If you like the project and think it deserves a contribution please donate any amount (via PayPal) on [project's official site](http://pgmodeler.com.br). The pgModeler's author will be immensely grateful for your support and will continue to improve the tool until its maximum pontential.

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

Change Log
----------

[ v0.4.0-alpha2 ]

Attention: The v0.4.0 is in alpha stage so the change log can be modified at any moment when new bug fixes / features are implemented. The stable 0.4.0 has no release date yet because all the code is being refactored meaning that the new version will be completely decoupled from previous versions.

* [New]: the relationship editing form gained an "advanced" tab which shows the objects generated and/or represents the relatioship itself.
* [New]: the user now can add relationships only creating foreign keys on tables (fk relationships).
* [New]: added a french UI translation (provided by [toorpy](https://github.com/toorpy)).
* [Change]: the printing options for the model were moved to the general configuration form.
* [Change]: relationship validation method now removes fk relationships when the foreign keys that gerenates is no longer exists.
* [Change]: copy/cut/delete commands does not manipulates system objects like schema public and languages C, SQL and plpgsql.
* [Fix]: pgModeler no longer crashes when the user activates the print dialog.
* [Fix]: corrected bug that was removing fk relationships when pasting objects.
* [Fix]: corrected SQL syntax error of 'timestamp with time zone'.
* [Fix]: corrected constraint type showing on editing form.
* [Fix]: corrected bug on cyrillic typed enums and check constraints expressions.
* [Fix]: corrected bug on enumeration type editing form.
* [Fix]: corrected bug on 'truncate' table privilege code generation.
* [Fix]: corrected column default value code generation.
* [Fix]: dummyplugin build process corrected on Windows.
* [Fix]: corrected bug on column comment code generation.
* [Fix]: corrected bug that was deleting two tables at once.

The complete change log can be found on [CHANGELOG.md](https://github.com/pgmodeler/pgmodeler/blob/master/CHANGELOG.md) file.

Older Releases/Code
-------------------

The older code and releases of pgModeler can be found at [Sourceforge.net](http://sourceforge.net/projects/pgmodeler)
