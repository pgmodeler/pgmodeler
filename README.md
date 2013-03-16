Introduction
------------

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


Developers and Reviewers wanted!
--------------------------------

pgModeler grow bigger and reached a state that its lonely developer cannot handle all the modification and new features requests as well to create a user's manual. So if you know C++ and Qt framework programming and wants to contribute with coding or an user's manual let me know! I'll be grateful for any help to the project!

Compiling/Installation
----------------------

For details about installation process from source code see [COMPILING.md](https://github.com/pgmodeler/pgmodeler/blob/master/COMPILING.md). If you don't want to compile pgModeler there are binaries available for download at [Downloads](https://github.com/pgmodeler/pgmodeler/downloads) section.

Known bugs
----------

* pgModeler in some situations can crash when undo / redoing operations. This is still in a deep investigation but the causes were not yet discovered.

Change Log
----------

v0.4.1
------

<em>Release date: March 16, 2013 </em>

* [New]: introduced the  "Disable SQL code" option for roles/tablespaces.
* [New]: user now can add objects by right-clicking group items on "Model Objects" dockwidget tree.
* [New]: added the abbreviation for time and timesptamp data types both with timezone: timetz and timestamptz.
* [New]: introduced a object highlight action on "model objects" dockwidget.
* [Change]: major changes on SQL code generation/export. Introduced a token to help export process to identify the end of each DDL command.
* [Change]: minor improvements on role editing form.
* [Change]: when generationg XML code empty tags that stores pure texts are now created with a <![CDATA[]]> tag in order to avoid malformed xml code.
* [Change]: index FASTUPDATE and FILLFACTOR params is now activated according the indexing type.
* [Change]: index fill factor now is optional.
* [Change]: chinese, portuguese and french translations update.
* [Fix]: pgModeler no longer crash when in error state (showing an exception) and try to auto save the models.
* [Fix]: minor size adjustments on forms.
* [Fix]: corrected a bug related to one-to-many relationship validation (endless looping) when changing to automatic suffix generation.
* [Fix]: corrected the "apply button disabled" bug on constraint edit form.
* [Fix]: IN/OUT keywords now appears on functions signature.
* [Fix]: corrected translation bypassing on index edit form.
* [Fix]: pgModeler no longer crash when triggering the print action.
* [Fix]: triggers no longer complains about assigning a function without parameters.
* [Fix]: corrected the loading process for indexes.
* [Fix]: corrected some bugs related to GiST and index sorting.
* [Fix]: minor fix on quick rename action when renaming a column with primary key.
* [Fix]: corrected a bug that was causing pgModeler to complain about duplicated elements when loading indexes.
* [Fix]: corrected a bug related to main window title when save a model with a different filename.
* [Fix]: fixed a bug related reload a model file after editing a foreign key.
* [Fix]: corrected a bug related to invalid chars at task progress.

The complete change log can be found on [CHANGELOG.md](https://github.com/pgmodeler/pgmodeler/blob/master/CHANGELOG.md) file.

Older Releases/Code
-------------------

The older code and releases of pgModeler can be found at [Sourceforge.net](http://sourceforge.net/projects/pgmodeler)
