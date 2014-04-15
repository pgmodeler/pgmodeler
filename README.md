Introduction
------------

pgModeler - PostgreSQL Database Modeler - is an open source data modeling tool designed for PostgreSQL. No more DDL commands written by hand let pgModeler do the job for you! This software reunites the concepts of entity-relationship diagrams and the features that PostgreSQL implements as extensions of SQL standards.

Please, let me know how the pgModeler is working on your system! Help to improve this project, give your feedback about the software or report any bug at [Issues](https://github.com/pgmodeler/pgmodeler/issues) page. Additionaly, follow the pgModeler profile on [Facebook](https://www.facebook.com/pgmodeler) or [Twitter](https://twitter.com/pgmodeler) and be up-to-date with new features, fixes and releases.

Licensing
---------

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation version 3.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See [LICENSE](https://github.com/pgmodeler/pgmodeler/blob/master/LICENSE) for details.

Donate to pgModeler
-------------------

Much effort, coffee, chocolate, time and knowledge has been devoted to this project so that a usable and constantly improved product can be delivered to the community. If you liked pgModeler and think it deserves a contribution please donate any amount (via PayPal) on [project's official site](http://pgmodeler.com.br).

Developers and Reviewers wanted!
--------------------------------

pgModeler grown bigger and reached a state that its lonely developer cannot handle all the modification and new feature requests as well to create a user's manual. So if you know C++ and Qt framework programming and wants to contribute with coding or an user's manual let me know! I'll be grateful for any help to the project!

Compiling/Installation
----------------------

For details about installation process from source code visit the [Installation](http://www.pgmodeler.com.br/wiki/doku.php?id=installation) section on Wiki. If you don't want to compile pgModeler there are binaries available for purchase at [official site](http://pgmodeler.com.br/purchase.php).

Known Issues
-----------

* pgModeler sometimes can crash during the export, import or validation process due to some threaded operations that, depending on size and arrange of the model, causes race conditions.
* Due to particularities on executing bundles on MacOSX the file association does not work correctly on this system. So it's not possible to open a model file by clicking it on Finder.
* To use pgmodeler-cli command on MacOSX you must call it by executing the following command inside the "pgmodeler.app/Contents/MacOS" path: ```./startapp pgmodeler-cli [PARAMS]```

Change Log
----------

v0.7.1
------
<em>Codename: <strong>Brave Mastodon</strong></em><br/>
<em>Release date: April 15, 2014</em><br/>

* [New] Added option to invert panning mode and range selection triggers.
* [New] Added support to use relationship attributes as special primary keys.
* [New] Created a small interface to pgmodeler-cli that enables the user to fix a broken model inside pgModeler GUI.
* [New] Added support to assign a sequence as default value of a column. The sequence will be converted to "nextval('seqname'::regclass) and the validation process will check if the sequence is correctly referenced by the table that owns the column.
* [Change] Improvement on unique name generation for columns and constraints when connecting relatioships.
* [Change] Improvement on copy / paste operations.
* [Change] Minor workaround in order to try to fix the crash due to thread conflict mainly on Windows system.
* [Change] Minor change on project's description text on about dialog.
* [Change] Changed the default behavior of left click on blank areas of canvas. Instead of create a range selection the user will move the viewport (panning mode). To enable range selection user must press SHIFT and click/move to draw the selection rectangle.
* [Change] Major model validation improvement. Now special objects are correctly validated.
* [Fix] Minor fix on custom columns positioning.
* [Fix] Input and output files are now correctly escaped on the model fix form and the process works fine.
* [Fix] Workaround for the slow editing of function's definition. Disabled the automatic syntax highlighting.
* [Fix] Minor fix on reverse engineering process. In some cases the process was aborted due to duplication of relationships caused by an incorreclty name generation for this kind of object.
* [Fix] Minor fix on model objects widget when changing the visible object types.
* [Fix] Fixed the conflict with panning mode and graphical object addition operation.
* [Fix] Fixed a regression introduced by 0.7.1-beta on model fix process.
* [Fix] Minor fix on connection class in order to accept empty passwords as well passwords that contains spaces.
* [Fix] Fixed the column listing on constraint editing form after remove one or more columns.
* [Fix] Fix a crash when canceling the model saving dialog on Windows.
* [Fix] Minor patch on the model fix process.
* [Fix] Fixed wrong default values for relationship added columns.
* [Fix] Fixed a bug related to file paths with ampersand on config files.
* [Fix] Fix the broken sql for inheritance when a child table don't have columns but only constraints.
* [Fix] Fixed a crash when closing a model that contains a view that references columns added by relationships.
* [Fix] Fixed wrong working directory handling on CLI.
* [Fix] Fixed a bug on file loading process that could left behind some objects depending on size and arrange of the loaded model.
* [Fix] Fixed the undesired behavior when moving a table to another schema.
* [Fix] Fixed connection config. Empty passwords are now accepted.
* [Fix] Fixed schema object code generation.
* [Fix] Fixed the usage of PGMODELER_SCHEMAS_DIR environment variable on import process.
* [Fix] Fixed "ALTER ... SET OWNER" DDL for materialized views.
* [Fix] Fixed duplicated semicolon at end of permissions defintion.

The detailed change log can be seen on [CHANGELOG.md](https://github.com/pgmodeler/pgmodeler/blob/master/CHANGELOG.md) file.

Older Releases/Code
-------------------

Older releases of pgModeler can be found at [Sourceforge.net](http://sourceforge.net/projects/pgmodeler)
