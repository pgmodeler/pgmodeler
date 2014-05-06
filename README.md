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

v0.7.2-alpha
------
<em>Codename: <strong>Brave Mastodon</strong></em><br/>
<em>Release date: May 06, 2014</em><br/>

* [New] Added a basic routine to check if there is some new version available on pgModeler site.
* [Change] Custom  indexes for columns and constraints added by relationships are now stored on tables. In previous version the relationship was the responsible for that but this approach was provoking the bug related on issue 449.
* [Change] Remove unused parser attributes and commented old code.
* [Change] Removed attributes and methods from relationship which were responsible to control columns, attributes and constraints indexes.
* [Fix] Fixed a bug that was changing the columns positions whenever a relationship was edited on the model.
* [Fix] Minor fix when generate the string that denotes the assigned constraints to a column.
* [Fix] Minor fix on function editing form when handling the result table columns. The variadic field is disabled on the parameter form.
* [Fix] Fixed a crash when removing a view linked to tables.
* [Fix] Minor fix on view reference code generation.
* [Fix] Minor editing form size adjustments.
* [Fix] Minor compilation fix on Windows system.

The detailed change log can be seen on [CHANGELOG.md](https://github.com/pgmodeler/pgmodeler/blob/master/CHANGELOG.md) file.

Older Releases/Code
-------------------

Older releases of pgModeler can be found at [Sourceforge.net](http://sourceforge.net/projects/pgmodeler)
