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

v0.7.0-alpha
------
<em>Codename: <strong>Brave Mastodon</strong></em><br/>
<em>Release date: January 24, 2014</em><br/>

* [New] Introduced the SQL tool that permits the execution of arbitrary SQL commands direclty on a server.
* [New] Added methods getType, getTypeId to BaseType and getSQLTypeName to PgSQLType as an alternative to call operators ~, ! and *.
* [New] Added a commented DROP command at start of each object definition (CREATE or ALTER TABLE ADD)
* [New] Added a "Code Preview" tab on permissions dialog.
* [New] Enabled SQL code visualization for FK relationships.
* [New] Added a build number on about dialog. This number is the compilation date in format yyyymmdd.
* [New] Added support for materialized and recursive views (PostgreSQL 9.3 feature).
* [New] Added pgModeler version information on generated sql scripts as well .dbm files for debugging purpose.
* [New] Added support to custom delete/update actions for relationship generated foreign keys.
* [New] Added support to move the canvas by positioning the mouse over corners.
* [New] Added a configuration parameter to control font style for any source code highlight field.
* [New] Added additional PostGiS types: geomval, addbandarg, rastbandarg, raster, reclassarg, unionarg, TopoGeometry, getfaceedges_returntype, validatetopology_returntype.
* [Change] Minor change when generate .stacktrace file for crash handler to include pgModeler build number.
* [Change] Minor adjustments on DatabaseImportForm's import execution progress.
* [Change] Minor enhancements on operation list when removing last operations.
* [Change] Minor enhancements on table and relationship dialogs on error control flow.
* [Change] Changed Z-value for relationship labels in order to avoid that name labels don't overlaps the cardinality labels.
* [Change] Removed the translation installing from within plugin loading method at PluginsConfigWidget.
* [Change] The Application class constructor now loads at once all translation files available for the current language including language file for plugins.
* [Change] Minor changes on deploy scripts on all platforms. The parameter '-with-build-num' was introduced in order to generate a package with build number.
* [Change] Relationship dialog enhanced. Now participant tables are described in what role they make part.
* [Change] Minor improvement on model export process.
* [Change] Minor improvement on model validation widget.
* [Change] Minor improvement on crash handler report generation message. Full path to crash file is now shown.
* [Change] Improved the message displayed when user try to save an invalidated model.
* [Change] Minor adjustment on model export dialog size.
* [Change] Minor improvement on model overview widget.
* [Change] Minor adjustments on window title buttons for model export and database import forms.
* [Change] Improvement on connection config form. pgModeler now ask to save/update unsaved connection if the user forgot to.
* [Change] Minor update sql syntax highlighting configuration file.
* [Fix] Fix bug related to geometry type.
* [Fix] Minor fix on logical expressions evaluation on SchemaParser.
* [Fix] Minor fix on model export when showing the name of objects being exported.
* [Fix] Minor fix on list/view advanced objects of a relationship.
* [Fix] Minor fix on form resizing when showing the protected object alert.
* [Fix] Fixed a minor bug that was crashing pgModeler when visualizing many-to-many relationships.
* [Fix] Fixed some warnings triggered by clang compiler.
* [Fix] Fixed a crash when loading plugins on MacOSX.
* [Fix] Fixed the issue related to import roles from database. pgModeler will not query pg_shadow anymore since this view is a very restricted object. Now role passwords will be imported as ***** (according to docs).
* [Fix] Fixed the object name validation. pgModeler now accepts spaces within names.
* [Fix] Fixed the function editing form resizing.
* [Fix] Fixed a bug that was not loading "sql disabled" state for relationships.
* [Fix] Fixed incorrect behavior of "Zoom In" action on MacOSX.
* [Fix] Trying to fix the infinite loop of the Validation confirm dialog on Windows (more tests needed).

v0.6.2
------
<em>Codename: <strong>Daring Mammoth</strong></em><br/>
<em>Release date: December 20, 2013</em>

* [Change] Update Qt version to 5.2.0 on build scripts (Windows only).
* [Change] Linux binaries are now bundled with all needed Qt libs.
* [Change] Important change on the way that special primary keys are created for generalization/copy relationships. Now there is the need to create the relationship first, close the dialog and open it again in order to generate the columns that will be used on the primary key.
* [Fix] Fixed a bug on model loading and relationship validation that was causing pgModeler to ignore some special objects (constraints referencing columns generated by relationships).
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
