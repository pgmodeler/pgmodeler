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

Running pgModeler
-----------------

The pgModeler can be executed directly from de executable but it is necessary change some environment variables, see [COMPILING.md](https://github.com/pgmodeler/pgmodeler/blob/master/COMPILING.md). For convinience there are two scripts used to startup pgModeler.

* Windows users: use the ```start-pgmodeler.bat``` script. By default this script consider that pgModeler is installed on ```c:\pgmodeler``` if you desire to set other location edit the script and change the PGMODELER_* environment variables.

* Linux/Unix users: use the ```start-pgmodeler.sh``` script. By default this script consider that pgModeler is installed on current working directory AKA ```pwd``` if you desire to set other location edit the script and change the PGMODELER_* environment variables.

Important Note
--------------

Since the beta release the model files has suffered changes on its structure. So if you are experiencing the error "Assignment of a not allocated schema to object [NAME]" during the loading process of models generated in version below 0.4.0-beta you need to manually create the schema "public".

To do it, open the .dbm file on your preferred text editor and add the code below right before the "database" tag.

    <schema name="public" rect-visible="true" fill-color="#f0f0f0">
    </schema>

Known bugs
----------

* pgModeler in some situations can crash when undo / redoing operations. This is still in a deep investigation but the causes were not yet discovered.

Change Log
----------

v0.4.0
------

<em>Release date: February 27, 2013 </em>

* [New]: introduce a "New object" submenu when activating the schema context menu (right-click)
* [New]: tables and view are now graphically separated by colored rectangles representing its schemas.
* [New]: compiling pgModeler now works perfectly on Mac OSX system.
* [New]: introduced the 'Quick actions' menu that permits: rename, move to another schema, change onwer and edit permissions.
* [New]: the relationship editing form gained an "advanced" tab which shows the objects generated and/or represents the relatioship itself.
* [New]: the user now can add relationships only creating foreign keys on tables (fk relationships).
* [New]: added a french UI translation (provided by [toorpy](https://github.com/toorpy)).
* [Change]: all relationships type are now grouped together on "Model objects" widget.
* [Change]: chinese UI translation updated (provided by: [Bumanji](https://github.com/Bumanji)).
* [Change]: user now can remove fk relationships directly without needing to remove the related foreign keys.
* [Change]: field semantics adjustments on relationship editing form.
* [Change]: graphical object can be now selected and have the context menu activated only with a single right-click.
* [Change]: minor improvements on plugin base class: PgModelerPlugin.
* [Change]: widget size adjustments to better showing on Mac OSX system.
* [Change]: crashhandler now shows the compiled and running versions of Qt.
* [Change]: french UI translation reviewed and updated (provided by [babs](https://github.com/babs)).
* [Change]: 'Objects of Model' when used as object picker now expand all the nodes by default.
* [Change]: 'Objects of Model' now memorizes the tree state when update an object and / or opening another model.
* [Change]: PostGiS 'geometry' type can have a free assigned SRID value.
* [Change]: editing forms when shown set the focus on the first field, generally, the object name.
* [Change]: 'Objects of Model' widget displays the nodes in alphabetical order.
* [Change]: the printing options for the model were moved to the general configuration form.
* [Change]: relationship validation method now removes fk relationships when the foreign keys that gerenates is no longer exists.
* [Change]: copy/cut/delete commands does not manipulates system objects like schema public and languages C, SQL and plpgsql.
* [Change]: pgModeler startup scripts are now path location free meaning that software can be installed where the user desires.
* [Fix]: corrected a bug related  constraint name on domain XML code generation.
* [Fix]: corrected a bug that was causing crash when click "Apply" on Type editing form with fields not filled.
* [Fix]: corrected the "invalid constraint name" error on domain editing form.
* [Fix]: corrected the empty DEFAULT clause for columns, types and domains.
* [Fix]: corrected a bug related to incorrectly initialized OID attribute when creating tables.
* [Fix]: corrected a bug when creating a view with WHERE statement.
* [Fix]: corrected a bug related to one-to-many relationships semantics.
* [Fix]: corrected some bugs that was causing crash when removing all operations from operation list.
* [Fix]: minor bug fixes related to object selection over the model.
* [Fix]: corrected a bug on load model dialog filter (chinese UI only).
* [Fix]: pgModeler no longer crashes when editing objects style.
* [Fix]: corrected bug that was deleting two sequeces at once.
* [Fix]: pgModeler no longer crashes when removing (disconnecting) relationship that has special primary keys.
* [Fix]: minor fixes on the startup scripts on all platforms.
* [Fix]: corrected an incorrect reference to output stream on Windows system.
* [Fix]: shortcuts and popup menu now works correctly when selection an object on 'Objects of Model' tree.
* [Fix]: the pgsql base types (represented by tables, sequences, user defined types and domains) are now updated correctly when the related schema is renamed.
* [Fix]: corrected some weird SRID value on non spatial types.
* [Fix]: corrected bug on objects table when move rows to last / first.
* [Fix]: typos corrections on some error messages and dialog titles.
* [Fix]: 'referenced columns' combobox on constraint editing form are filled correctly when the dialog is shown in a second time.
* [Fix]: pgModeler no longer crashes when creating many-to-many relationships.
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
