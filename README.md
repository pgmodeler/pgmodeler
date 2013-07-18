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

Model File Changes
------------------

Several changes were introduced in pgModeler 0.5.1_r1 making some tags on model files created on 0.5.1 (and below) incompatible with the new version. If you having problems during loading of older model files you can open them on a text editor and try the fixes below:

* Remove ```io-cast``` attribute from ```<cast>``` tags.
* Remove ```recheck``` attribute from ```<element>``` tags.
* Remove values ```greater-op```, ```less-op```, ```sort-op``` or ```sort2-op``` from ```ref-type``` attribute from ```<operator>``` tags.
* Remove ```sysid```attribute from ```<role>``` tags.
* Replace ```<parameter>``` tag by ```<typeattrib>``` on ```<usertype>``` tags.
* Remove ```auto-sufix```, ```src-sufix``` and ```dst-sufix``` from ```<relationship>``` tags.

After that you can try to reload your model. Probably a second problem can occurr: broken references between objects. As the model validation was introduced on pgModeler 0.5.0 the object references are not checked during loading time anymore. Instead the user needs to validate the model every time before saving it.

As a collateral effect loading older models can generate several errors like "Object A is referencing the object B which was not found in the model!" (Code: ERR_REF_OBJ_INEXISTS_MODEL). The fix to this is a little tricky: you have to change the creation order of the object A in relation of B, that means, B must be created before A. So, again, open the model on a text editor and move the XML portion of the B object to a position right before the A XML portion.

MacOSX Notes
------------

* Due to particularities on executing bundles on MacOSX the file association does not work correctly on this system. So it's not possible to open a model file by clicking it on Finder.
* To use pgmodeler-cli command you must call it by executing the following command inside the "pgmodeler.app/Contents/MacOS" path: ```./startapp pgmodeler-cli [PARAMS]```

Change Log
----------

v0.6.0-alpha
------
<em>Codename: <strong>Daring Mammoth</strong></em><br/>
<em>Release date: July 18, 2013</em>

[New] Added the widget to swap objects IDs on model validation form.
[New] Added indexing type "spgist" to IndexingType class.
[New] Model validation and export now works with threads.
[New] Functions now have a "leak proof" attribute.
[New] VARIADIC key attribute added to function parameters.
[New] User now can completely disabled UI stylesheets by calling executable with param "-no-stylesheet".
[New] Added the class Catalog to handle the basis of reverse engineering by reading the pgcatalog/information_schema.
[New] System tablespaces pg_global and pg_default are now automatically created as new models are added.
[Change] Improved the transition between opened models. The problem was solved putting the temporary models saving in a separated thread.
[Change] Minor improvement on model restoration operation. Models that fails to be restored can be kept so the user can try to fix them manually (until pgModeler is closed).
[Change] Minor adjustments on model loading progress.
[Change] Minor chages on model export form. Export to DBMS is the default option.
[Change] Minor improvements on pgmodeler-cli. Added "--simulate" option to export dbms.
[Change] Disabled notice output to console for Connection class. User can re-enable it by calling Connection::setNoticeEnabled() [recompile source needed].
[Change] Minor changes on PgSQL base types. "[NO] LEAKPROOF" removed from FunctionType class.
[Change] Minor improvements on SchemaParser, now its possible to make parser ignore empty attributes.
[Change] libdbconnect renamed to libpgconnector for semantics reasons.
[Change] Minor improvements on CrashHandler. Added buttons to load report and save embedded model when in analysis mode.
[Fix] Minor fix on linuxdeploy.sh script related to grep command execution.
[Fix] Fixed some leaks when destroying objects that are registered as PgSQLType (table, sequence, extension, domain, view, type). Now these type are correctly remove from user type listing.
[Fix] Fixed bad sql code generation when disabling sql of columns/constraints.
[Fix] Fixed dependency retrieving for operator classes.
[Fix] Fixed incorrect reference to "replicate" option on Roles.
[Fix] Fixed the "ignore duplicity" bug for columns when exporting model.
[Fix] Fixed library build order now libpgmodeler is built before libpgconnector.
[Fix] Minor fix on UI stylesheet related tooltips when using Fusion theme.
[Fix] Fixed the assignment of LC_COLLATE, LC_CTYPE and template db to database instance on DatabaseWidget.

The complete change log can be found on [CHANGELOG.md](https://github.com/pgmodeler/pgmodeler/blob/master/CHANGELOG.md) file.

Older Releases/Code
-------------------

The older code and releases of pgModeler can be found at [Sourceforge.net](http://sourceforge.net/projects/pgmodeler)
