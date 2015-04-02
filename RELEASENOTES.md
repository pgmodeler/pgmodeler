v0.8.1-alpha
------
<em>Codename: <strong>Faithful Elephant</strong></em>
<em>Release date: April 02, 2015</em>
<em>Changes since: <strong>v0.8.0</strong></em><br/>

<strong>Summary:</strong> this release brings a series of fixes for bugs reported in 0.8.0 as well minor improvements for new features introduced by that version.<br/>

In this release the user can generate SQL code for the object's dependencies and children in source code preview dialog for test purposes. The diff process was improved causing more reliable SQL diffs to be generated.<br/>

The reverse engineering process was patched too and now constraint triggers are correctly imported. The database management was improved too avoiding opened connections left behind and leading to a waste of resources. <br/>

Finally, the model fix procedure was patched and now it is capable to correctly fix models created in pgModeler 0.7.2.<br/>

There are more changes in this version and the key ones are listed below. Please, refer to CHANGELOG.md to a complete set of modifications.<br/>

* [New] Added a "Contributors" section in "About pgModeler" dialog.
* [New] The "plugins" folder is created automatically at startup if does not exits.
* [New] Added the ability to show original SQL code, dependencies and children's code for test purposes in source code preview dialog.
* [New] Added an action to save SQL code to file in source code preview dialog.
* [New] Added an option to list indirect refereces to an object in ObjectDepsRefsWidget.
* [Change] Deployment scripts on all platforms now uses PostgreSQL 9.4 and Qt 5.4.1 by default.
* [Change] File association procedures were moved to CLI. Now the user can install/remove file association by using '--dbm-mime-type' option (Windows and Linux).
* [Change] The build process now uses libxml2 from PostgreSQL installation (Windows).
* [Change] Improvements on how objects are recreated using the "recreate unmodifiable" option on Diff process.
* [Change] Enhanced the control of database explorer widgets and the SQL execution panes related to them.
* [Change] Added a clear error message when required fields are not set when creating/updating object.
* [Change] Installation folder/files arrangement reverted to previous settings in order to avoid "DLL entry point errors" errors (Windows).
* [Change] Minor change in pgmodeler.pri to set default output paths according to FSH standard (Linux).
* [Fix] Fix tab order in ConnectionsConfigWidget.
* [Fix] Fixed a bug when importing constraint triggers.
* [Fix] Minor bug fix when dropping table children objects in database explorer.
* [Fix] Minor fix when generating XML code for permissions.
* [Fix] Minor bug fix on database explorer at manage view to avoid left opened connections.
* [Fix] Added a patch in model fix process to correctly move indexes/triggers/rules from within tables to outside their xml definition.
* [Fix] Fixed a bug when configuring encoding for database. Now the "Default" value can be used normally.
* [Fix] Fixed a bug on model fix process that was removing empty lines (only with breaks) from functions definitions as well from other objects.
* [Fix] Fix crash when converting a serial column which is not assigned to a primary key.
