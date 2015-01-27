v0.8.0-beta
------
<em>Codename: <strong>Faithful Elephant</strong></em>
<em>Release date: ???, 2015</em>
<em>Changes since: <strong>v0.8.0-beta1</strong></em><br/>
<strong>Summary:</strong> this last beta release brings several bug fixes mainly for import and diff processes. Some changes were done in order to improve the modeling experience (see below).<br/>
One important change made was the refectoring of build scripts that will permit pgModeler to be packaged more easily. The new build mode is still being shaped and tested but is already functional.<br/>
Below some key changes/fixes of this release, more details can be found on CHANGELOG.md file.

* [New] Added a bug report form on main window to give user the chance to report a bug without use crash handler.
* [New] Added action to enable/disable an object's sql from quick actions menu at ModelWidget.
* [New] Created a new namespace PgModelerUiNS to store shared constants and function in libpgmodeler_ui subproject.
* [Change] Improvements on crash handler to reuse the code from bug report form.
* [Change] Changed the default PREFIX on pgmodeler.pri to /opt/pgmodeler when building on Linux
* [Change] Several adjustments on deployments scripts to use the new build variable settings.
* [Change] Additional improvements on start-pgmodeler.sh and startapp.
* [Change] Crash/bug report files now have extensions .bug instead of .crash
* [Change] Modifications done on .pro files that will permit custom output paths when building pgModeler from source, enabling it to be package to several linux distros.
* [Change] Modifications done on DatabaseImportForm in order to be in the same standard as ModelExportForm and ModelDatabaseDiffForm.
* [Change] Minor improvements on ModelExportHelper in order to show the correct actions (commands) being executed.
* [Change] Improvements on ModelExportForm by including an output tab in order to display all actions taken during the export process.
* [Change] Adjustments on PgModelerCLI, ModelExportForm and ModelExportHelper to accept the "drop objects" option.
* [Change] Minor adjustment on ModelDatabaseDiffForm in order to lower the chances to crash the app if user try to repeatedly cancel and start over the diff process.
* [Change] Minor change on the generation of DROP statements attached to object's SQL.
* [Fix] Minor fix on CLI that was wrongly considering <dbmodel> tag attributes default-* as xml code for database objects causing errors on fix process.
* [Fix] Minor fix on diff process that was ignoring column's data type changes.
* [Fix] Minor fix on column SQL generation that was removing quotation on sequences names when using nextval() function call as default value.
* [Fix] Minor fix on ModelFixForm to correctly find the startapp script on MacOSX.
* [Fix] Fixed bug when import/diff user defined types that contains $ in the middle of their names. Now the names are quoted to avoid errors when referencing those types.
* [Fix] Minor fix when saving/restoring sessions. Now, empty sessions (without loaded files) are correctly saved.
* [Fix] Fixed a crash when importing a database in a second time after some error has occurred previously.
* [Fix] First version of a fix to solve problems related to inheritance diff/import. Now pgModeler does not try to drop inherited columns on diff process.
* [Fix] Fixed a crash when destroying constraints stored on operation history.
* [Fix] Fix on the import process to correctly identify and create inherited columns that aren't linked to constraints or other objects. For those referenced by other objects they will be created as detached columns.
* [Fix] Fixed a bug on rule.sch catalog query.
* [Fix] Minor fix on generation of DROP commands.
* [Fix] Fixed some catalog query schema files and the Catalog class itself in order to correctly select/discard objects linked directly or not to extensions.
* [Fix] Minor fix on generation of commands related to extensions. According to the rule, extensions names can't be schema qualified.
