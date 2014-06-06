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

v0.7.2-beta
------
<em>Codename: <strong>Brave Mastodon</strong></em><br/>
<em>Release date: June 06, 2014</em><br/>

* [New] Added an option to convert integer type to serial ones on columns which default value is a nextval(seq::regclass) call.
* [Change] Changed the MacOS X deployment script to use Qt 5.3.
* [Change] Minor size adjustment on the new object overlay.
* [Change] The model objects widget now clears the selection when it lost the focus.
* [Change] Minor change on new object overlay behavior. The overlay will be hidden when any tool button on it is pressed or the ESC key is pressed.
* [Change] The partial database import was improved. Now pgModeler is capable to resolve dependencies of foreign key constraints, creating the needed tables automatically. Additionally, domains are automatically imported when referenced by columns.
* [Fix] Fixed compilation process on Windows system.
* [Fix] Fixed the crash handler call inside the main program.
* [Fix] Minor fix on "object's deps. & refs." dialog that was triggering an error when trying to open a table child object from there.
* [Fix] Added a entry on DatabaseModel::getObjectDependecies() to get the sequence that a column depends on.
* [Fix] Fixed a bug column editing dialog that was wrongly removing the sequence assigned to a column.
* [Fix] Fixed sample plugins compilation when using a LIBDIR value other than the default.
* [Fix] Fixed the build number generation at compilation time.
* [Fix] Minor fix on TableTitleView class to correctly strikeout the schema name only when the schema rectangle is not visible.
* [Fix] Minor adjustment on databaseimportform.ui size.
* [Fix] Fixed the custom page size bug due to patches provided by Qt 5.3. A conditional compilation statement was added on portions where the custom page size is used. This way the code can be build on any Qt version but the fix will available only on 5.3.
* [Fix] Minor fix on the Linux deployment script.

The detailed change log can be seen on [CHANGELOG.md](https://github.com/pgmodeler/pgmodeler/blob/master/CHANGELOG.md) file.

Older Releases/Code
-------------------

Older releases of pgModeler can be found at [Sourceforge.net](http://sourceforge.net/projects/pgmodeler)
