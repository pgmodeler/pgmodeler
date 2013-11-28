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

v0.6.2-beta
------
<em>Codename: <strong>Daring Mammoth</strong></em><br/>
<em>Release date: November 29, 2013</em>

* [New] Added an option to drop database before a export process.
* [New] Disabling SQL code now disables the code of all referrer and child objects (experimental!).
* [New] Added support for columns to reference "table types".
* [Change] Object names are trimmed on editing forms to avoid unnecessary error triggering.
* [Change] Minor improvement on crash handler form.
* [Change] Minor change on SQL validation message.
* [Change] Minor improvement on operation list.
* [Fix] Fixed the import of foreign key constraints from PostgreSQL 9.3.
* [Fix] Fixed the creation of fk relationships when the involved tables has too long names. pgModeler no more complains about "already existent object".
* [Fix] Minor fix when showing self fk relationships.
* [Fix] Minor fix on pgmodeler-cli: working directory is now set correctly.
* [Fix] Minor fix when retrieving advanced (generated) objects from relationships.
* [Fix] Fixed a bug that was not properly removing table objects when the user was canceling the table's editing.

The complete change log can be found on [CHANGELOG.md](https://github.com/pgmodeler/pgmodeler/blob/master/CHANGELOG.md) file.

Older Releases/Code
-------------------

The older code and releases of pgModeler can be found at [Sourceforge.net](http://sourceforge.net/projects/pgmodeler)
