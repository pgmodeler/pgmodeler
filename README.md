Introduction
------------

PostgreSQL Database Modeler, or simply, pgModeler is an open source tool for modeling databases that merges the classical concepts of entity-relationship diagrams with specific features that only PostgreSQL implements. The pgModeler translates the models created by the user to SQL code and apply them onto database clusters from version 8.0 to 9.1.

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

Change Log
----------

v0.5.0_beta
-----------

ATTENTION: Since version 0.5.0 is in beta stage the change log can change until the release date: May, 17.

* [New] Complete main window restyling.
* [New] Added a model validation tool to prevent reference break and name conflicts.
* [New] Added an object navigation using keyboard on model widget. Pressing Alt + [left|right] keys will switch between graphical objects.
* [New] Introduced the pgmodeler-cli. A command line tool to handle model export without loading the graphical interface.
* [New] Added an option to list available configured connections on pgmodeler-cli.
* [New] pgModeler now alerts the user when he try to save an invalidated model.
* [New] pgModeler now aborts app closing when the user wants to do a last saving on modified models.
* [New] Added support to hide relationship labels and table extended attributes on configuration dialog.
* [New] Added "Recent Models" menu.
* [New] Introduced the Xml2Object plugin to help on develpment testings.
* [New] Added partial support to PostgreSQL Extensions objects.
* [New] Added JSON datatype.
* [New] Added support for rules and trigger on views.
* [New] Added support for user defined range types.
* [New] Added support for collations on composite types (user defined).
* [New] Added built-in range types.
* [New] Added support to INCLUDING/EXCLUDING options when dealing with copy relationships.
* [New] Added support for EXCLUDE constraint support
* [New] Added NO INHERIT option to check constraints.
* [New] Added REPLICATION option to roles.
* [New] Added FOR ORDER BY option and removed Recheck from OperatorClassElement.
* [New] Added collation support for index elements.
* [New] Added [NOT] LEAKPROOF key word to functions.
* [New] Added collation attribute to domains.
* [New] Required fields are now highlighted on editing forms.
* [New] pgModeler creates system objects (e.g, public schema and SQL, C, plpgsql languages) when adding a new model.
* [Change] Relationship points are moved when the parent relationship is being moved together with other objects.
* [Change] Simplified the model loading operation. pgModeler will not try to recreate objects with unsatisfied dependencies instead errors will be raised.
* [Change] Minor changes on FK relationship creation.
* [Change] User-added foreign-keys had code generation changed.
* [Change] Minor improvements on PgModelerPlugin structure.
* [Change] DummyPlugin renamed to Dummy.
* [Change] Improvements on building process for all supported systems.
* [Change] Removed "Save widget positions" from configuration form.
* [Change] Removed fullscreen mode from main window.
* [Change] Removed unused/deprecated error messages.
* [Change] Removed deprecated files COMPILING.md and PLUGINS.md.
* [Change] Subproject libutil was renamed to libutils due to some conflicts on Linux systems.
* [Change] Startup scripts removed. Since the environment variables are set by the installer on Windows and for Unix the variables are set using the new pgmodeler.sh script.
* [Change] "Disable SQL code" option added for all types of objects. Except for textboxes and base relationships (view-table relatioships and fk relationships).
* [Change] Fixed permissions for views.
* [Change] PostgreSQL 8.x support completely removed.
* [Change] Schema files (for SQL code) aren't organized in folders anymore. All code (for different PostgreSQL versions) will be in the same .sch file for each object.
* [Change] Spatial types had SRID digit count upgraded to 5.
* [Change] One-to-one relationships now generates unique names for UNIQUE constraints.
* [Change] Several class improvements, performance tunings and forms readjustments.
* [Fix] Now relationship labels' position are restored when loading the model file.
* [Fix] Minor fixes on database model code generation.
* [Fix] Corrected the glicthy wheel scroll/zoom on model widget.
* [Fix] Corrected the visual update of schema's rectangle when adding a column on a child table.
* [Fix] Corrected a bug that was preventing a new model to be saved correctly.
* [Fix] Minor fixes on model widget copy/paste operations.
* [Fix] Models now are correclty auto saved when modified.
* [Fix] Corrected operator's signature generation.
* [Fix] Corrected a bug on textbox with unicode texts.
* [Fix] Index and Rule editing forms now handles correctly unicode expressions.
* [Fix] Corrected a bug that was avoiding the name "remembering" during relationship loading.

The complete change log can be found on [CHANGELOG.md](https://github.com/pgmodeler/pgmodeler/blob/master/CHANGELOG.md) file.

Older Releases/Code
-------------------

The older code and releases of pgModeler can be found at [Sourceforge.net](http://sourceforge.net/projects/pgmodeler)
