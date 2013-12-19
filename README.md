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
* The sample plugins were deactivated on this system due to some unexpected failures. This will be solved on the next major version 0.7.0.

Change Log
----------

v0.6.2
------
<em>Codename: <strong>Daring Mammoth</strong></em><br/>
<em>Release date: December 20, 2013</em>

* [Change] Update Qt version to 5.2.0 on build scripts (Windows only).
* [Change] Linux binaries are now bundled with all needed Qt libs.
* [Change] Important change on the way that special primary keys are created for generalization/copy relationships. Now there is the need to create the relationship first, close the dialog and open it again in order to generate the columns that will be used on the primary key.
* [Fix] Workaround done on the sql append widget when handle a lot of code avoiding slowdowns on the syntax highlighting.
* [Fix] Fixed the incorrect creation of foreign keys on many-to-many relationships.
* [Fix] Fixed the conversion of self many-to-many relationships.
* [Fix] Fixed a bug that was causing some constraints to be destroyed when the relationship was connected to the table that owned the constraint.
* [Fix] Comments that contains apostrophes now are correctly escaped in order to avoid SQL related errors.
* [Fix] Fixed the incorrect generation of SQL code of check constraint associated to many-to-many relationshps.
* [Fix] Minor fix on crash handler when trying to read an stack trace file that doesn't exists.
* [Fix] Minor typos fixes on CLI menu.
* [Fix] Minor fix on the about form positioning.

The complete change log can be found on [CHANGELOG.md](https://github.com/pgmodeler/pgmodeler/blob/master/CHANGELOG.md) file.

Older Releases/Code
-------------------

The older code and releases of pgModeler can be found at [Sourceforge.net](http://sourceforge.net/projects/pgmodeler)
