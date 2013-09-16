Introduction
------------

PostgreSQL Database Modeler, or simply, pgModeler is an open source tool for modeling databases that merges the classical concepts of entity-relationship diagrams with specific features that only PostgreSQL implements. The pgModeler translates the models created by the user to SQL code and apply them onto database clusters (Version 9.x).

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

For details about installation process from source code visit the [Installation](http://www.pgmodeler.com.br/wiki/doku.php?id=installation) section on Wiki. If you don't want to compile pgModeler there are binaries available for download at [official site](http://pgmodeler.com.br).

MacOSX Notes
------------

* Due to particularities on executing bundles on MacOSX the file association does not work correctly on this system. So it's not possible to open a model file by clicking it on Finder.
* To use pgmodeler-cli command you must call it by executing the following command inside the "pgmodeler.app/Contents/MacOS" path: ```./startapp pgmodeler-cli [PARAMS]```

Change Log
----------

v0.6.0-beta
------
<em>Codename: <strong>Daring Mammoth</strong></em><br/>
<em>Release date: September 16, 2013</em>

* [New] Added experimental reverse engineering support.
* [New] Added an experimental option --fix-model to pgmodeler-cli to permit the user to fix the structure of an older model (generated in pgModeler < 0.6.0) or a corrupted file.
* [New] Added an option to debug the import process printing any generated code to stdout.
* [New] Added support for bidirectinal FK relationships.
* [New] Added a statement "SET search_path [schemas]" on database model SQL code.
* [New] Added missing PostgreSQL built-in types.
* [New] Configured connections now can be duplicated in order to reuse it's attributes.
* [Change] Minor change on main compilation script. The subproject "tests" are included only when compiling in debug mode.
* [Change] Major change on validation widget. Fixes are now applied using a thread and can be aborted any time user want.
* [Change] Change on model saving process. pgModeler will not deny to save invalidated anymore. Now it will ask the user to validate the model or save an invalidate one anyway.
* [Change] Changed the behavior of the operation list. In the first exception the entire list are emptied.
* [Change] Changed the way foreign keys are generated. They always will be generated at end of database definition to avoid reference breaking.
* [Change] Minor improvements on model code generation and copy operations.
* [Change] Removed deprecated "rtree" indexing type.
* [Fix] Minor fixes on PgSQLType class. User types aren`t removed instead they are deactivated to avoid reference breaking.
* [Fix] Minor fix on selecting the children objects of a schema.
* [Fix] Minor fixes on scene and relationship avoiding crashes when destroying the whole graphical scene.
* [Fix] Fixed bug on deleting self relationships.
* [Fix] Minor fix on model export process. The last line of the SQL code now is correctly extracted and executed.
* [Fix] Minor fix on sequence class to accept owner.
* [Fix] Minor fixes on the splash screen control code.
* [Fix] Fixed a bug that was crashing pgModeler at startup.
* [Fix] Fixed a bug that was causing pgModeler to crash when loading operators which name contained '&' char.
* [Fix] Fixed bug related to sequence values assignment.
* [Fix] Fixed "Operation with not allocated object" error on applying validation fixes.
* [Fix] Fixed relationship label position saving.
* [Fix] Minor fix on main window. pgModeler now is not closed while the validation is running.

The complete change log can be found on [CHANGELOG.md](https://github.com/pgmodeler/pgmodeler/blob/master/CHANGELOG.md) file.

Older Releases/Code
-------------------

The older code and releases of pgModeler can be found at [Sourceforge.net](http://sourceforge.net/projects/pgmodeler)
