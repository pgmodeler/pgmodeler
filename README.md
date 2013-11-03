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

v0.6.1
------
<em>Codename: <strong>Daring Mammoth</strong></em><br/>
<em>Release date: November 3, 2013</em>

* [New] PostgreSQL version 9.3 activated on code base. Now import and export operations works with this new version.
* [Change] Changed the way inheritance is created. Now the INHERIT command is appended in the table's definition.
* [Change] Update on model validation. Generalization and copy relationships have the participant tables' id's validated in order to check reference breaking.
* [Change] Version info upgraded on MacOSX app bundle configuration file (Info.plist).
* [Change] Minor change on "pgmodeler.vars". Included environment variables for custom Qt installation.
* [Fix] Fixed a bug related to INSTEAD OF/ON UPDATE triggers on views.
* [Fix] Fixed a bug related to incorrectly error raised when setting a owner table in the same schema as the sequece.
* [Fix] Fixed a bug related to importing sequences which name has uppercase characters.
* [Fix] Fixed misspelled "Connetion" word on configuration form.
* [Fix] Typos correction on model validation message box.
* [Fix] Fixed incorrect objects removal after cancel the edition.
* [Fix] Minor fix on disconnection of generalization relationships.
* [Fix] Minor fix on updating table's graphical representation when importing primary keys.
* [Fix] Minor change when displaying the columns' types on table/relationship editing form.
* [Fix] Fixed the compilation process on MacOSX 10.9 (Mavericks).
* [Fix] Minor change on macdeploy.sh to use Qt5.2-beta by default.

The complete change log can be found on [CHANGELOG.md](https://github.com/pgmodeler/pgmodeler/blob/master/CHANGELOG.md) file.

Older Releases/Code
-------------------

The older code and releases of pgModeler can be found at [Sourceforge.net](http://sourceforge.net/projects/pgmodeler)
