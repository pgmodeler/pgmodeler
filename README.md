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

pgModeler grown bigger and reached a state that its lonely developer cannot handle all the modification and new feature requests. So if you know C++ and Qt framework programming and wants to contribute with coding let me know! I'll be grateful for any help to the project!

Compiling/Installation
----------------------

For details about installation process from source code visit the [Installation](http://www.pgmodeler.com.br/wiki/doku.php?id=installation) section on Wiki. If you don't want to compile pgModeler there are binaries available for purchase at [official site](http://pgmodeler.com.br/purchase.php).

Known Issues
-----------

* pgModeler does not fully supports the [quoted identifier notation](http://www.postgresql.org/docs/current/static/sql-syntax-lexical.html#SQL-SYNTAX-IDENTIFIERS). When using quoted identifiers only the characters are accepted in the middle of names: a-z A-Z 0-9 _ . @ $ - space
* pgModeler is unusable in sandboxed Mac OS X installations. To workaround this issue you'll need to deactivate sandbox usage to run pgModeler properly. There is no planning to adapt this tool for sandbox feature in Mac OS X. There is no plan to adapt pgModeler to run in sandboxed Mac OS X installations.
* pgModeler can't be compiled in Microsoft Visual Studio due to use of some gcc/clang extensions. There is a work in progress to make the build possible but it is too early to tell if it'll be fully functional and merged to the official codebase.
* Compiling the source using '-Wconversion' (disabled by Qt in its default) generates a lot of warnings. These warnings are harmless in 99% of times but we can't just ignore them but I don't plan to fix them in a near future (mail me for more details! It's a long history...).
* pgModeler sometimes can crash during the export, import or validation process due to some threaded operations that, depending on size and arrange of the model, causes race conditions.
* Due to particularities on executing bundles on MacOSX the file association does not work correctly on this system. So it's not possible to open a model file by clicking it on Finder.

Change Log
----------

The detailed log of changes can be seen on [CHANGELOG.md](https://github.com/pgmodeler/pgmodeler/blob/master/CHANGELOG.md) file.

Older Releases/Code
-------------------

Older releases of pgModeler can be found at [Sourceforge.net](http://sourceforge.net/projects/pgmodeler)
