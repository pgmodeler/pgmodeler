v1.1.0
------
<em>Release date: February 29, 2024</em><br/>
<em>Changes since: <strong>v1.0.6</strong></em><br/>

<em><strong>Attention:</strong> Some configuration files were changed in pgModeler 1.1.0 causing a break in backward compatibility with pgModeler 1.0.x settings. This way, at the first start of the newer version, pgModeler will try to migrate the older settings to the newer ones automatically!</em><br/><br/>

<strong>Summary</strong> <br/>

**Infinite canvas support:** Added support for infinite canvas.

**Improved code completion:** A long-awaited feature is finally arriving pgModeler, the code completion based on living database object names. From now on, in the SQL execution widget, will be possible to list column/table names in the middle of the INSERT/DELETE/TRUNCATE/UPDATE/ALTER/DROP commands. This feature also considers table aliases and lists the correct columns. It is worth mentioning that this feature is still experimental despite the good results on different kinds of SQL commands. <br/>

**Improved UI theme management:** In 1.0.x, pgModeler wasn't able to correctly follow the system's color set (dark/light). Now, in 1.1.0-alpha, selecting "System default" UI theme in Appearance settings makes pgModeler properly configure the UI color theme as well as the source code highlight settings to follow the system's color schema. <br/>

**Improved model loading, objects' searching, and validation speeds:** One of the most annoying things on pgModeler for me was the speed of the operations like objects' search, model validation, and, mainly, database model file loading. During the development of this version, I decided to face the challenge of improving these three operations, so I delved into the internals of the tool looking for the major bottlenecks. After selecting the problematic ones, I took the path of rewriting some mechanics instead of trying to fix them. The two main bottlenecks that degraded the speed of the mentioned operations were the objects' name validation/formatting as well as the retrieval of objects' dependencies and references. Those seemed simple operations that I even could imagine that they were making pgModeler struggle to handle big models. For the objects' name formatting and validation, I decided to create an internal name cache to avoid calling those procedures repeatedly. A simple solution that brought a surprisingly good result. For the objects' dependencies and references handling, I completely ditched the methods written for that purpose and created something infinitely simpler. Instead of calling every time a procedure that runs countless loops and recursive calls, I just made the objects store internally which other objects are their references and dependencies. Those changes made models that were loading/validating in several minutes to be processed in a few seconds. I still have some other bottlenecks to solve, but those two already removed, gave pgModeler an amazing performance.<br/>

**Views creation is now way simpler:** Instead of that clunky interface to configure views on previous versions, now the user can create this kind of object by using freely typed SQL commands with special placeholder variables enclosed by {} that we call references. Any reference in the typed SQL command that defines the view will be replaced by the referenced object, which can be columns, functions, procedures, tables, foreign tables, and views (yeah, views!). Once the view is created, pgModeler will create relationships between the referenced tables (foreign tables, and views) and the new view. Of course, not everything is roses, the feature is not backward compatible with models designed in the previous version, which means if you have models containing views you'll need to use the pgmodeler-cli fix process to make the proper corrections.<br/>

**Extensions can now have multiple child data types:** In previous releases, pgModeler had a special flag in the extension's editing form labeled "Handles data type". That flag served to inform pgModeler that the extension needed to be used as a data type, for example, creating an extension named "hstore" and checking the mentioned flag, would create the type "hstore" making it usable in the columns, function parameters, and so on. The problem with this approach is that if an extension installed more than one data type in the database, it was needed to make some workarounds to have a second type available to be used in the database modeling process. So, in pgModeler 1.1.0-beta, the "Handles data type" flag was ditched and now the user can specify a free number of data types that the extension handles. pgModeler will handle the data types when adding or removing the extension. Models that use the old extension format can be fixed by using the pgmodeler-cli model fix process.<br/>

**Improved tool's executables relocation:** pgModeler already has some mechanisms to customize the paths associated with the assets and executables once installed in the system. One of them is the environment variables, but sometimes the user doesn't want or even has no privileges to change environment variables in the system. Thinking of that, this new version introduces a special configuration file called "pgmpaths.conf" which goal is to configure the paths where the pgModeler main executable as well as the auxiliary tools can find all the needed folders, assets, and configuration files. This file must be created in the same folder as a pgModeler's executable and must be filled with lines in the format variable=path. The "variable" refers to one of the available environment variables understood by pgModeler (refer to the installation instructions, section "Environment variables" for details) and the "path" is a relative or absolute path to the resource associated with the environment variable.<br/>

**pgModeler CLI plug-ins support:** ....

**SQL session plugin:** This version introduces the SQL session plugin, available in the paid version of the tool, which implements simple routines to save the current opened SQL command execution sessions in a specific configuration file which can be restored in the next pgModeler execution by clicking the action "Restore SQL session", close to the connections combo box, in the Manage view.<br/>

**Backup utility plugin:** This version introduces the backup utility plugin, available in the paid version of the tool, which implements a user-friendly interface for the commands _pg_dump_, _pg_dumpall_, _pg_restore_ and _psql_ commands. This extra feature was developed mainly focused on attending to those users less comfortable with command-line tools, being possible to dump and restore databases without leaving pgModeler's GUI. Of course, advanced users are welcome to use the plugin and help to improve it! In a nutshell, besides configuring the backup tools parameters with a simple form, it allows the creation of presets per backup tool for different needs, it also has some facilities that automate the backup file name generation by using a default backup folder and name patterns.<br/>

**Other changes and improvements**<br/>
* [New] Now during object copy/paste or duplication, in case of name conflict, the user is asked to type a new name for each conflicting object.
* [New] Adding the ability to restore the scene rect from the database model file.
* [New] Adding support to expand canvas rect via the main window.
* [Change] Removed the confirmation dialog in ModelWidget::copyObjects related to copying dependencies. Now the copy of dependencies during the copy of an object is triggered by the shortcut Ctrl+Shift+C (copy selection and dependencies) or Ctrl+Shift+X (cut selection and dependencies).
* [Change] Minor improvement in ModelObjectsWidget::saveTreeState/restoreTreeState to correctly restore the vertical scrollbar position.
* Added support for inksaver color theme which uses only black and white colors for models that are used for printing.<br/>
* Added support for using object comments as aliases in database import.<br/>
* pgModeler now asks the user about closing SQL execution tabs that are not empty (with typed commands).<br/>
* Add support for remembering decisions on the alerts regarding unsaved models/open SQL tabs.<br/>
* Added an option in GeneralConfigWidget to reset the exit alert display status.<br/>
* Added a basic form to inspect changelog XML code.<br/>
* Added missing multirange types.<br/>
* Improved the relationship point addition and selection via mouse clicks.<br/>
* The "dot" grid mode is now the default in the appearance.conf file due to better drawing performance.<br/>
* Improved the scene background (grid, delimiter, limits) drawing speed for big models.<br/>
* Improving the objects' filtering in reverse engineering by introducing an "any" filter type.<br/>
* Data manipulation form now shows a confirmation message before closing when items are pending save.<br/>
* Added support for overriding the canvas' background color when exporting the model to PNG.
* The "Display unique results" option on objects' dependencies & references dialog is now checked by default.
* Adjusted the CSV pasting in the table data editor.
* Adjusted the extension's attributes display in the database explorer to list types related to an extension.
* The code completion widget now resizes according to the displayed items' width.
* The code completion will not display a "no items found" popup if no element is found matching the word at the cursor's position.
* Adjusted the reverse engineering process so relationships can be created from the link between two views.
* Minor change in reverse engineering to avoid importing extension child types into the model since the extension itself, when imported, already creates the types.
* [Change] Minor adjustment in connections configuration form in such a way to reload connections where they a used only when the user applies new connection settings on the form or restores original connections by clicking "Cancel".
* [Change] Minor improvement on the object ids swapping form in such a way as to preserve the sorting parameters after swapping object ids.
* [Change] Minor change in the operation list widget in such a way as to use icons instead of labels for operation list size and current history position.
* [Change] Refactored signal/slot connections that needed exception handling all over the tool avoiding crashing the application when an exception is raised (macOS only).
* [Change] Minor adjustment in model creation to avoid marking it invalidated even if it's new and empty.
* [Change] Minor adjustment in code completion widget to include sequences when listing living database objects.

**Bug fixes**<br/>
* [Fix] Fixed the plugins' loading process by ignoring the ones that don't implement the correct interface (PgModelerCliPlugin or PgModelerGuiPlugin).
* [Fix] Fixed a bug in MainWindow::loadModel that was not registering the loaded file as a recent model.
* [Fix] Fixed a malformed diff code when adding a column to a partitioned table.
* [Fix] Minor fix on ObjectsScene::drawBackground in order to draw the canvas background in the correct color defined in the settings.
* [Fix] Fixed a bug that was preventing tables and schemas from being updated graphically when adding/removing relationships in some circumstances.
* [Fix] Minor fix in graphical objects tooltips in libcanvas.
* [Fix] Fixed the trigger catalog query to avoid referencing tgparentoid field when importing from PostgreSQL 12 or below.
* [Fix] Fixed the PNG export process in GUI and CLI to handle infinite canvas.
* [Fix] Fixed a bug in ModelFixForm that was not locating pgmodeler-cli on Windows.
* [Fix] Fixed a bug in ObjectsFilterWidget that was setting wrong object types when doing a partial diff using filters generated from the changelog.
* [Fix] Minor fix in CodeCompletionWidget in order to list columns of tables non-schema qualified. In that case, pg_catalog and public will be used as default schemas.
* [Fix] Columns marked as PK are now restored when handling them in TableWidget.
* Minor fix in the object finder widget to avoid disconnecting a null selection which could lead to crashes.<br/>
* Minor fix in the database model widget to hide the new object overlay when moving a selection of objects in the design area.<br/>
* Minor fix in the object removal routine in a model widget that was not erasing an object in case it shared the same name of other objects in the same schema.<br/>
* Minor fix in the object addition routine to validate the layer of the object being added. If one or more layers are invalid the object will be moved to the default layer 0.<br/>
* Minor fix in pgmodeler-cli when extracting the objects' XML code during model file structure repair in order to restore correctly the layers name/count.<br/>
* Fixed a bug in partial reverse engineering that was not correctly importing functions in some specific conditions.<br/>
* Fixed a bug in partial reverse engineering that was not importing some objects' dependencies correctly.<br/>
* Fixed a bug in the appearance configuration widget that was not updating the example model colors when changing the UI theme.<br/>
* Fixed a crash when double-clicking the overview widget.<br/>
* Fixed the data dictionary schema files for tables and views.<br/>
* Fixed a bug in the database model that was causing FK relationships of a hidden layer to be displayed after loading the model.<br/>
* Fixed a bug in the scene move action that was causing the grid to not be displayed after a panning/wheel move.<br/>
* Fixed settings storing for the grid options in MainWindow.
* Fix a crash that was happening only on Windows.
* Fixed a bug in the generation of diff commands for identity columns.
* Fixed a bug in list widget items painting that was causing the rendering of artifacts sometimes.
* Fixed a bug in pgmodeler-cli that was aborting the fix process during the parsing of the model changelog.
* Fixed a crash when trying to load an invalid model from the recent model's menu.
* Fixed sample model structure to the new view's format.
* Fixed several bugs in the code completion widget when completing code using live database object names.
* [Fix] Fixed the catalog query operator.sch so argument data types can have the schema name prepended.
* [Fix] Fixed a crash when trying to display function/procedure/operator source code that references a domain in the parameters.
* [Fix] Fixed the catalog queries for function and procedure to correctly format parameters' data type signatures.
* [Fix] Fixed a problem in object ids swapping widget that was not capturing arrow key press on macOS.
* [Fix] Fixed a bug in the importing of partitioned table indexes which was affecting the results produced by the diff process.
* [Fix] Fixed a bug in the code completion widget that was crashing the application when trying to list columns of an alias related to a non-schema-qualified table.
* [Fix] Fixed a bug in reverse engineering related to the importing of partition tables hierarchy.
* [Fix] Fixed a bug in diff that was generating false-positive results for columns with the same user-defined type.
* [Fix] Fixed a bug in the line numbers widget, within source code input fields, that was not aligning the line numbers with their respective lines when the font size was different from the default.
* [Fix] Fixed a broken build related to XML parser that was using an xmlError * reference instead of a const xmlError * reference.
* [Fix] Minor fix in code completion widget to avoid the displaying of the widget in a position that extrapolates the screen limits.
* [Fix] Minor fix in the icon positioning in the list and tree widget items.

Finally, for more details about this version's changelog, please, refer to CHANGELOG.md.
