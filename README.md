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

v0.6.0
------
<em>Codename: <strong>Daring Mammoth</strong></em><br/>
<em>Release date: September 30, 2013</em>

* [New] Added a validation when removing protected FK relationships.
* [New] Added a progress info (at bottom widgets bar) for temporary model saving.
* [New] User can now restore the last session via File > Restore Session. Sessions will not be restored at startup anymore.
* [New] Added a "zoom" option when exporting to PNG image.
* [Change] Disabled the model loading via command line on MacOSX due to bundle particularities.
* [Change] Remove option "Save session" from general config widget.
* [Change] Improved the way schema's children objects are selected/unselected.
* [Change] Improved the printing operation. Now custom paper size has a separated field to assign it's coordinates.
* [Change] The import errors now are written on a log file when "ignore import errors" is checked.
* [Fix] Fixed an inconsistence when removing a table before the fk relationship linked to it. From now on (to avoid crashes) user must remove the relationship first and then remove the table.
* [Fix] Fixed a minor bug on column's graphical representation that was incorrectly configuring the column descriptor for self-relationship fk's.
* [Fix] Minor fix on model overview widget when showing large models.
* [Fix] Fixed bug on pgmodeler-cli that was generating errors when running it outside the executable's directory.
* [Fix] Fixed the calculation of pages to be printed.
* [Fix] Fixed the type enumeration validation to accept space on the names.
* [Fix] Minor fix for GiS types. Spatial auxiliary type name can null.
* [Fix] Minor pgmodeler-cli typos corrections.
* [Fix] Fixed a bug related to XMLParser and threads that was crashing pgModeler on Windows.
* [Fix] Fixes on DatabaseImportHelper to correctly handle extension created types.
* [Fix] Minor fix on PgSQLType::parseString() when creating datatypes from strings.

The complete change log can be found on [CHANGELOG.md](https://github.com/pgmodeler/pgmodeler/blob/master/CHANGELOG.md) file.

Older Releases/Code
-------------------

The older code and releases of pgModeler can be found at [Sourceforge.net](http://sourceforge.net/projects/pgmodeler)
