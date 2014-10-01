Change Log
---------

v0.8.0-alpha2
------
<em>Codename: <strong>Faithful Elephant</strong></em><br/>
<em>Release date: September 30, 2014</em><br/>

* [New] Added an option on general settings to disable ask to validate model before save, export and diff.
* [New] If the user try to save, export or diff a model and it is invalidated, pgModeler will first validate and then proceed with the pending operation.
* [New] Added an entry on general settings to control how graphical objects are created.
* [New] Added an option to disable render smoothness to improve performance on large models.
* [New] Added the method Catalog::getObjectsOIDs in order to retrieve all database objects oid and store them in maps. This method is used as an auxiliary for model-database diff process.
* [New] Added the method ConnectionsConfigWidget::fillConnectionsComboBox() in order to reuse the code that fills up a combobox with configured connections.
* [New] Added the ability to create special primary key on many-to-many relationships.
* [New] Added the ability to save the dock widgets configuration on the main configuration file.
* [New] Added methods to retrieve objects by their names on DatabaseModel class.
* [Change] The configuration form was reestrucured decreasing the size occupied on the screen.
* [Change] Minor improvement on model fix dialog.
* [Change] Minor improvement on constraint import. Fillfactor attribute now is correcly retrieved.
* [Change] Removed extern directives referencing configurations from SyntaxHighlighter and ModelWidget converting the used attributes to static ones.
* [Change] Changed the way threads are created, as well the import and diff helpers instances to avoid race conditions and crashes.
* [Change] Schemas by default will be created to show the bounding rectangle.
* [Change] Minor improvement on BaseObject::getTypes() in order to exclude some types from the resulting vector.
* [Change] Minor widgets adjustment on modeldatabasediffform.ui, modelexportform.ui and databaseimportform.ui.
* [Change] Updates on several resource images.
* [Change] Minor change on example.dbm file.
* [Change] Minor change on DatabaseImportHelper to accept a DatabaseModel instance instead of ModelWidget.
* [Change] Minor adjustments on deployment scripts.
* [Fix] Fixed issue with Inno Setup invalid bitmap error (Windows).
* [Fix] Fixed a bug that was preventing encrypted password to be configured for roles.
* [Fix] Fixed a crash whenever user quit the application on Mac OSX.
* [Fix] Minor fixes in order to compile using Qt 5.3.2.
* [Fix] Fixed a bug when starting the creation process of relationships on model widget.
* [Fix] Fixed a bug related to individual permission exclusion and object removal error when there are permissions attached to it.
* [Fix] Fixed typos on demo version warning messages.
* [Fix] Fixed a bug that was not generating FILLFACTOR attribute for constraints.
* [Fix] Minor fix on object naming rules. Now the dollar sign ($) is accepted in the middle of object's name.
* [Fix] Fixed a bug when import domains which have the same name as some tables.
* [Fix] Fixed a bug on trigger class and editing form that was preventing the "FOR EACH ROW" attribute to be saved.
* [Fix] Fixed a infinite loop on operation list class when calculating the chain size.
* [Fix] Fixed a crash when creating relationships. Apparently this crash was caused by a faulty access on some threads right after close the relationship dialog.
* [Fix] Fixed the bug that was preventing a sequence to be assinged to a column.
* [Fix] Fixed a bug on relationship validation process that was causing errors mainly related to generalization relationships.
* [Fix] Fixed a bug on xml parser when reading DTD files from paths that contains accentuated chars.
* [Fix] Minor fix on Messagebox class when replacing <br> tags on message text.
* [Fix] Minor fix on conditional compilation macros entries for demo version.

v0.8.0-alpha1
------
<em>Codename: <strong>Faithful Elephant</strong></em><br/>
<em>Release date: August 15, 2014</em><br/>

* [New] pgModeler now is capable to handle table's data through the new data manipulation form.
* [New] The SQL tool received a basic find/replace widget.
* [New] Created a new environment variable for samples directory, PGMODELER_SAMPLES_DIR.
* [New] Source code dialog now optionally appends the SQL of permissions for the current object.
* [New] Added an option to the source code privew dialog to include table's children objects' SQL.
* [New] Added a field on all object's editing form to expose the object's internal id.
* [New] Added an option on the database import process to generate random colors for relationships.
* [New] The model objects widget gained a filtering field that is capable to list objects by their name or internal id.
* [New] Added support to custom SQL for rules, indexes and triggers.
* [New] Added two new sample models.
* [Change] Changed the shortcut for "About pgModeler" to F4 key.
* [Change] Minor update on shortcuts and tooltips of buttons on bottom control bar at main window.
* [Change] Minor improvement on model validation. Now the DDL executed for any object is shown on the output field.
* [Change] Minor change on object's id interval generation.
* [Change] Minor adjust on model export process to force the code cache invalidation when using the unique name generation option.
* [Change] Message box objects are now used on demand and no more as class attributes.
* [Change] The software startup was hugely improved by removing singleton dialogs from main window's constructor, now they are used on demand.
* [Change] Minor update on sql-highlight.conf file.
* [Change] Sample models can be now loaded direclty from the main windows's central widget.
* [Change] Enhancement on the messages of model validation output.
* [Change] Minor fix on object's schema files to correctly generate the appended/prepended SQL code.
* [Change] Major improvement on all core classes (resposible to represent database objects) in order to keep their cache syncronized with the current configuration.
* [Change] Object finder now highlights the parent table if the user selects a child object of the first.
* [Change] Minor improvement on CLI. Added a routine to change a table structure, moving the tags rule, index and trigger to outside of it.
* [Change] Changed the way on how the relationship connection points are determined. Now they depends on how the tables' center points are distant from each other.
* [Change] More improvements done on database model objects classes in order to give more speed on the code generation.
* [Change] Update for French (fr_FR) translation.
* [Chage] Minor improvements on swap objects ids dialog.
* [Change] Huge improvement on validation process mainly for reverse engineered models. Now pgModeler honors the imported structure and in a few cases there will be inconsistencies.
* [Change] The objects rule, index and trigger will have the SQL/XML code generated outside of table's definition due to validation process that sometimes needs to swap id's of those objects.
* [Change] The object selector will trigger the object selection if user click the input field.
* [Change] Changed the 'line color' attribute of relationships to 'custom color'.
* [Fix] Fixed a crash provoked by the constraint editing form when switching the constraint type on the second time the form is opened.
* [Fix] Minor fix on database import process that was wrongly checking all tables as unlogged.
* [Fix] Minor fix on sample models pagila.dbm and usda.dbm to remove the unlogged attribute.
* [Fix] Minor fix on class CodeCompletionWidget to make it persistent as well to remove duplicate items from listing.
* [Fix] Minor fix on message box instances to resize according to the height of the current message.
* [Fix] Fixed a crash whenever the user cancelled the changes on settings and tried to connect to a server using the SQL tool.
* [Fix] Minor fix on SQLToolWidget to disable controls in some situations.
* [Fix] Minor fix on BaseRelationship and Relationship classes to correctly invalidate envolved object's when connecting or disconnecting.
* [Fix] Fixed a crash when closing the last model and the object finder was visible with result list filled.
* [Fix] Object finder now doesn't show duplicated items.
* [Fix] Minor fix on model objects widget when assigning a null model.
* [Fix] Fixed the problem with function parameter name not being generated due to cached code.
* [Fix] Minor fix on index editing form layout.
* [Fix] Fixed the import of rule object on DatabaseImportHelper class.
* [Fix] Fixed a bug when displaying the cardinalities for fk relationships.
* [Fix] Fixed a bug on the reverse engineering process that was wrongly migrating 'timestamp with time zone' data type as 'timestamp' only.
* [Fix] Fixed a bug on the reverse engineering process that was preventing the migration of operator classes / indexes that contains duplicated elements.

v0.8.0-alpha
------
<em>Codename: <strong>Faithful Elephant</strong></em><br/>
<em>Release date: July 21, 2014</em><br/>

* [New] Added support to using global settings for relationships on the editing form of those objects.
* [New] A new section was created on settings dialog to manage global configurations for relationships.
* [New] Enabled the movement of schema objects without the need to select their children. This operation does not applies to protected or system schemas like public, pg_catalog.
* [New] Created a more elaborated central widget with some basic operations like create, load, load recent models and restore previous session.
* [New] Reletionships 1:1, 1:n and fk now supports a new style of link between tables. The foreign key columns will be directly linked to primary key columns.
* [New] Created a class called ColorPickerWidget to handle operations where the user need to configure colors for objects.
* [New] Relationship objects now supports custom line colors.
* [New] Added the static methods setPgSQLVersion and getPgSQLVersion on BaseObject class in order to override the PostgreSQL version used to generate code for all instances of that class.
* [New] Introduced a method DatabaseModel::getXMLParser() in order to return the xmlparser of the model to permit user create new objects from xml code within the current database model.
* [Change] Minor adjustment on central widget buttons style on MacOS X.
* [Change] Disabled the collapsed main menu on MacOS X due to particularities on GUI of this system.
* [Change] Minor adjustment on main menu construction on Linux/Windows.
* [Change] Major change on main UI style in order to make a cleaner interface and gain more space on canvas to handle models.
* [Change] Minor model objects style refreshment.
* [Change] Minor optimization on class AppearanceConfigWidget by adding color pickers in order to select color of the elements.
* [Change] A significant code optimization was made on TagWidget by changing the tool buttons that was handling color configuration by color pickers.
* [Change] Added a color picker on RelationshipWidget to permit the configuration of custom line color.
* [Change] Minor layout adjustment on update notifier widget.
* [Change] Important change on SchemaParser class which is not a singleton anymore in order to minimize crashes due to race conditions on running threads.
* [Change] Important change on XMLParser class which is not a singleton anymore in order to minimize crashes due to race conditions on running threads.
* [Fix] Fixed a bug related to partial command execution on SQL tool.
* [Fix] Minor fix on model navigation widget when closing a model.
* [Fix] Fixed a bug that was crashing pgModeler when switching models with the overview widget opened.
* [Fix] Fixed a bug that was eventually making pgModeler enter on a endless looping when calculating the chained operations length.
* [Fix] Fixed a regression related to special objects recreation after rename a primary key column. The problem was that the relationship was wrongly storing the old name of the columns at connection time.

v0.7.2
------
<em>Codename: <strong>Brave Mastodon</strong></em><br/>
<em>Release date: June 27, 2014</em><br/>

* [New] Added the missing attribute BUFFERING for Gist indexes.
* [New] Added an experimental installer for Linux based upon the Qt Installer Framework.
* [New] Added an file called RELEASENOTES.md. This file will be read by the update notifier on server in order to return to the GUI the change log and additional release info.
* [Change] Minor change on MacOS X deployment script to generate the dmg file with a custom icon.
* [Change] Restored the start-pgmodeler.sh file due to difficulties that some users are having when running pgModeler on Linux systems.
* [Change] Created the folder "installer" to store all files related to installers of all platforms
* [Fix] Fixed a regression related to sequences not being included on database model XML code.
* [Fix] Fixed deployment on Linux. Now start-pgmodeler.sh is correclty copied.
* [Fix] Fixed the display of relationship actions on NewObjectOverlayWidget

v0.7.2-beta1
------
<em>Codename: <strong>Brave Mastodon</strong></em><br/>
<em>Release date: June 21, 2014</em><br/>

* [New] Added support for event trigger objects. The export and import processes were adjusted to handle this kind of object.
* [New] Added support for UNLOGGED tables.
* [New] Enabled PostgreSQL 9.4 export and import processes.
* [New] Enabled the jsonb datatype for PostgreSQL 9.4.
* [Change] Rolled back to Qt 5.2.1 the version used by the macdeploy.sh script due to crash reported in issue #494 and #482.
* [Change] SQL tool improvement. Added a "drop cascade" action and fixed a bug when refreshing a specific item on database tree.
* [Fix] Minor fix on ColumnWidget, disabled the line breaking in the field "Expression".
* [Fix] Fixed the model fix dialog on MacOS X and Windows platforms.
* [Fix] Schemas names are now correctly striked out on the rectangle when their SQL code is disabled. The system schemas public and pg_catalog will continue not to have their names striked out.
* [Fix] Fixed a bug that was preventing default values of domains to be loaded from file.

v0.7.2-beta
------
<em>Codename: <strong>Brave Mastodon</strong></em><br/>
<em>Release date: June 06, 2014</em><br/>

* [New] Added an option to convert integer type to serial ones on columns which default value is a nextval(seq::regclass) call.
* [Change] Changed the MacOS X deployment script to use Qt 5.3.
* [Change] Minor size adjustment on the new object overlay.
* [Change] The model objects widget now clears the selection when it lost the focus.
* [Change] Minor change on new object overlay behavior. The overlay will be hidden when any tool button on it is pressed or the ESC key is pressed.
* [Change] The partial database import was improved. Now pgModeler is capable to resolve dependencies of foreign key constraints, creating the needed tables automatically. Additionally, domains are automatically imported when referenced by columns.
* [Fix] Fixed compilation process on Windows system.
* [Fix] Fixed the crash handler call inside the main program.
* [Fix] Minor fix on "object's deps. & refs." dialog that was triggering an error when trying to open a table child object from there.
* [Fix] Added a entry on DatabaseModel::getObjectDependecies() to get the sequence that a column depends on.
* [Fix] Fixed a bug column editing dialog that was wrongly removing the sequence assigned to a column.
* [Fix] Fixed sample plugins compilation when using a LIBDIR value other than the default.
* [Fix] Fixed the build number generation at compilation time.
* [Fix] Minor fix on TableTitleView class to correctly strikeout the schema name only when the schema rectangle is not visible.
* [Fix] Minor adjustment on databaseimportform.ui size.
* [Fix] Fixed the custom page size bug due to patches provided by Qt 5.3. A conditional compilation statement was added on portions where the custom page size is used. This way the code can be build on any Qt version but the fix will available only on 5.3.
* [Fix] Minor fix on the Linux deployment script.


v0.7.2-alpha1
------
<em>Codename: <strong>Brave Mastodon</strong></em><br/>
<em>Release date: May 30, 2014</em><br/>

* [New] Introduced a "new object" overlay widget which gives user a quick access to actions that create objects.
* [New] Added a step to fix indexes with old <condition> tag on command line interface.
* [New] Added support to item interaction on "object's dependencies and references" dialog.
* [New] Added support to generate temporary names for database, roles and tablespaces when running validation process. This will avoid errors if the original database and the other objects already exists on the server.
* [New] Updated the CLI to include the option to generate temporary object's names.
* [New] Added suppport to save and restore the last position and zoom on the canvas. This behavior can be deactivated on general settings.
* [New] Added support to prepend SQL commands on object's definition.
* [New] Added zoom info popup that appears whenever the user changes the current zoom factor.
* [Change] Renamed the methods setCheckExpression and getCheckExpression methods to setExpression and getExpression because the expression is used either for check and exclude constraints.
* [Change] Renamed the attribute "condition" to "predicate" on index class.
* [Change] Improved the way SQL code is disabled/enabled on editing forms. User now is asked to apply the same disabling/enabling status to object's refereces.
* [Change] Minor size adjustment on SQL append dialog.
* [Change] Changed the order of the tabs on user-defined type editing form.
* [Change] Minor appearance improvement on textbox editing form.
* [Fix] Made visible the field related to predicate expression on exclude constraint editing form.
* [Fix] Added reference checking between an exclude constraint and operators.
* [Fix] Minor fix on copy operation that was not resetting the copied objects list if the user clicked several times the "copy" command without paste.
* [Fix] Minor fix on the model object widget when disabling the controls.
* [Fix] Minor fix on ModelWidget::createSequenceForColumn() operation. The generated sequence is now correctly assigned to the column.
* [Fix] Minor fix on pgModeler command line interface menu text.
* [Fix] Minor fix on permission.dtd file. Attribute "index" was incorreclty marked as required when its optinal.
* [Fix] Minor fix on semantics of one-to-one relationship. When one of the tables are of mandatory participation the foreign key columns must not accept null values.
* [Fix] Minor fix on Windows and Linux deployment scripts.


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


v0.7.1
------
<em>Codename: <strong>Brave Mastodon</strong></em><br/>
<em>Release date: April 15, 2014</em><br/>

* [New] Added option to invert panning mode and range selection triggers.
* [New] Added support to use relationship attributes as special primary keys.
* [Change] Improvement on unique name generation for columns and constraints when connecting relatioships.
* [Change] Improvement on copy / paste operations.
* [Change] Minor workaround in order to try to fix the crash due to thread conflict mainly on Windows system.
* [Fix] Minor fix on custom columns positioning.
* [Fix] Input and output files are now correctly escaped on the model fix form and the process works fine.

v0.7.1-beta1
------
<em>Codename: <strong>Brave Mastodon</strong></em><br/>
<em>Release date: April 8, 2014</em><br/>

* [Change] Minor change on project's description text on about dialog.
* [Fix] Workaround for the slow editing of function's definition. Disabled the automatic syntax highlighting.
* [Fix] Minor fix on reverse engineering process. In some cases the process was aborted due to duplication of relationships caused by an incorreclty name generation for this kind of object.
* [Fix] Minor fix on model objects widget when changing the visible object types.
* [Fix] Fixed the conflict with panning mode and graphical object addition operation.
* [Fix] Fixed a regression introduced by 0.7.1-beta on model fix process.

v0.7.1-beta
------
<em>Codename: <strong>Brave Mastodon</strong></em><br/>
<em>Release date: April 6, 2014</em><br/>

* [New] Created a small interface to pgmodeler-cli that enables the user to fix a broken model inside pgModeler GUI.
* [New] Added support to assign a sequence as default value of a column. The sequence will be converted to "nextval('seqname'::regclass) and the validation process will check if the sequence is correctly referenced by the table that owns the column.
* [Change] Changed the default behavior of left click on blank areas of canvas. Instead of create a range selection the user will move the viewport (panning mode). To enable range selection user must press SHIFT and click/move to draw the selection rectangle.
* [Fix] Minor fix on connection class in order to accept empty passwords as well passwords that contains spaces.
* [Fix] Fixed the column listing on constraint editing form after remove one or more columns.
* [Fix] Fix a crash when canceling the model saving dialog on Windows.
* [Fix] Minor patch on the model fix process.
* [Fix] Fixed wrong default values for relationship added columns.
* [Fix] Fixed a bug related to file paths with ampersand on config files.
* [Fix] Fix the broken sql for inheritance when a child table don't have columns but only constraints.

v0.7.1-alpha1
------
<em>Codename: <strong>Brave Mastodon</strong></em><br/>
<em>Release date: March 29, 2014</em><br/>

* [Change] Major model validation improvement. Now special objects are correctly validated.
* [Fix] Fixed a crash when closing a model that contains a view that references columns added by relationships.
* [Fix] Fixed wrong working directory handling on CLI.
* [Fix] Fixed a bug on file loading process that could left behind some objects depending on size and arrange of the loaded model.
* [Fix] Fixed the undesired behavior when moving a table to another schema.

v0.7.1-alpha
------
<em>Codename: <strong>Brave Mastodon</strong></em><br/>
<em>Release date: March 21, 2014</em><br/>

* [Fix] Fixed connection config. Empty passwords are now accepted.
* [Fix] Fixed schema object code generation.
* [Fix] Fixed the usage of PGMODELER_SCHEMAS_DIR environment variable on import process.
* [Fix] Fixed "ALTER ... SET OWNER" DDL for materialized views.
* [Fix] Fixed duplicated semicolon at end of permissions defintion.

v0.7.0
------
<em>Codename: <strong>Brave Mastodon</strong></em><br/>
<em>Release date: February 25, 2014</em><br/>

* [New] Addded a  catalog attribute "hide-postgres-db" in order to avoid listing "postgres" maintainance DB on import operations.
* [New] Added options to hide system/extension objects on SQL tool improving the object listing performance.
* [New] Added support to custom compilation output directory through qmake variables BINDIR, LIBDIR and RESDIR.
* [New] Added support to deferrable unique, exclude and primary key constraints.
* [New] Added support to custom colors on tables and views through tag objects.
* [New] Added support to export models to png image page by page.
* [New] Canvas can now be moved using Control + Arrow keys. If the shift is pressed the movement factor is increased.
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
* [Change] Added support to on-demand updates on sql tool object's tree.
* [Change] Improved the tab navigation experience on editing forms.
* [Change] Minor change on SQL tool to ommit binary data values.
* [Change] Dropped the navigation through object using Alt + <left|right> due to the difficulty to understand the order in which objects are highlighted.
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
* [Fix] Fixed bug that was permitting paste already formatted text (html) on source code input fields.
* [Fix] Fix broken range type generation.
* [Fix] The DELETE privilege is now correclty saved on model.
* [Fix] Fixed drop object command on SQL tool.
* [Fix] Fixed bug that was crashing pgModeler when a error was raised on view edit form.
* [Fix] Fixed a bug that was crashing the application when deleting relationship attributes or constraints.
* [Fix] Fixed bug related to the range selection weird behavior when finishing creating a object.
* [Fix] Minor fix on OperationList undo/redo methods to update types names on tables that references a modified type.
* [Fix] Minor fix on View assignment operator to correctly rename the type associated with "this" object.
* [Fix] Minor fix on DatabaseModel to correctly return the references to a view type.
* [Fix] Fixed bug that was causing indexes/triggers that references columns added by relationship have the sql code generated twice.
* [Fix] Minor fix on ResultSet class to identify bytea columns.
* [Fix] Minor fix on CLI menu to add new export modes.
* [Fix] Fixed a crash dealing with duplicated columns on a table.
* [Fix]  Fixed bug when deleting tables and fk relationships together.
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

v0.6.2-beta
------
<em>Codename: <strong>Daring Mammoth</strong></em><br/>
<em>Release date: November 29, 2013</em>

* [New] Added an option to drop database before a export process.
* [New] Disabling SQL code now disables the code of all referrer and child objects (experimental!).
* [New] Added support for columns to reference "table types".
* [Change] Object names are trimmed on editing forms to avoid unnecessary error triggering.
* [Change] Minor improvement on crash handler form.
* [Change] Minor change on SQL validation message.
* [Change] Minor improvement on operation list.
* [Fix] Fixed a crash when adding foreign keys to a new table.
* [Fix] Fixed the import of foreign key constraints from PostgreSQL 9.3.
* [Fix] Fixed the creation of fk relationships when the involved tables has too long names. pgModeler no more complains about "already existent object".
* [Fix] Minor fix when showing self fk relationships.
* [Fix] Minor fix on pgmodeler-cli: working directory is now set correctly.
* [Fix] Minor fix when retrieving advanced (generated) objects from relationships.
* [Fix] Fixed a bug that was not properly removing table objects when the user was canceling the table's editing.

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

v0.6.0-beta
------
<em>Codename: <strong>Daring Mammoth</strong></em><br/>
<em>Release date: September 16, 2013</em>

* [New] Added experimental reverse engineering support.
* [New] Added an experimental option --fix-model to pgmodeler-cli to permit the user to fix the structure of an older model (generated in pgModeler < 0.6.0) or a corrupted file.
* [New] Added an option to debug the import process printing any generated code to stdout.
* [New] Added support for bidirectinal FK relationships.
* [New] Added a statement "SET search_path [schemas]" on database model SQL code.
* [New] Added missing PostgreSQL built-in types.
* [New] Configured connections now can be duplicated in order to reuse it's attributes.
* [Change] Minor change on main compilation script. The subproject "tests" are included only when compiling in debug mode.
* [Change] Major change on validation widget. Fixes are now applied using a thread and can be aborted any time user want.
* [Change] Change on model saving process. pgModeler will not deny to save invalidated anymore. Now it will ask the user to validate the model or save an invalidate one anyway.
* [Change] Changed the behavior of the operation list. In the first exception the entire list are emptied.
* [Change] Changed the way foreign keys are generated. They always will be generated at end of database definition to avoid reference breaking.
* [Change] Minor improvements on model code generation and copy operations.
* [Change] Removed deprecated "rtree" indexing type.
* [Fix] Minor fixes on PgSQLType class. User types aren`t removed instead they are deactivated to avoid reference breaking.
* [Fix] Minor fix on selecting the children objects of a schema.
* [Fix] Minor fixes on scene and relationship avoiding crashes when destroying the whole graphical scene.
* [Fix] Fixed bug on deleting self relationships.
* [Fix] Minor fix on model export process. The last line of the SQL code now is correctly extracted and executed.
* [Fix] Minor fix on sequence class to accept owner.
* [Fix] Minor fixes on the splash screen control code.
* [Fix] Fixed a bug that was crashing pgModeler at startup.
* [Fix] Fixed a bug that was causing pgModeler to crash when loading operators which name contained '&' char.
* [Fix] Fixed bug related to sequence values assignment.
* [Fix] Fixed "Operation with not allocated object" error on applying validation fixes.
* [Fix] Fixed relationship label position saving.
* [Fix] Minor fix on main window. pgModeler now is not closed while the validation is running.

v0.6.0-alpha1
------
<em>Codename: <strong>Daring Mammoth</strong></em><br/>
<em>Release date: August 05, 2013</em>

* [New] Added catalog query for triggers.
* [New] Added catalog query for rules.
* [New] Added indexing method to exclude constraint.
* [New] Added catalog query for constraints.
* [New] Added catalog queries for tables and columns
* [New] Added catalog queries for view, domain, sequence and user defined types.
* [New] Added catalog query for collation.
* [New] Added catalog query for operator family.
* [New] Added catalog query for operator class.
* [New] Object dependencies form now can list indirect dependencies.
* [New] Added an environment variable to set a different location for crash handler executable.
* [New] Objects that has SQL disabled now is shown with name striked out.
* [Change] Minor change on MainWindow::closeEvent()
* [Change] Moved app_style variable to GlobalAttributes::DEFAULT_QT_STYLE.
* [Change] Minor improvement on Exception::getExceptionsText method.
* [Change] Improvements on copy/paste operations.
* [Change] Removed unused linker parameters.
* [Change] Crash handler executable renamed to "pgmodeler-ch".
* [Fix] Fixed possible leak when destroying a ModelWidget instance. Objects from  scene were not being deleted correclty. Fix tests in progress.
* [Fix] Fixed the "Save current session" option on GeneralConfigWidget that wasn't doing it's job correctly.
* [Fix] Fixed a bug that was crashing pgModeler at startup when restoring previous sessions or temporary models.
* [Fix] Minor fix on trigger code generation.
* [Fix] Fixed incorrect loading of multiple triggers/rules on views.
* [Fix] Minor fix on model validation. Operator classes are now checked during the validation process.
* [Fix] Fixed generation of constraints in form of ALTER command. In some cases the constraint code wasn't appended to table's definition.
* [Fix] Minor fixes on cast object.
* [Fix] Minor fixes on databasemodel on retrieving dependencies/references for objects.
* [Fix] Fixed crash handler path variable on MacOSX.

v0.6.0-alpha
------
<em>Codename: <strong>Daring Mammoth</strong></em><br/>
<em>Release date: July 19, 2013</em>

* [New] Added the widget to swap objects IDs on model validation form.
* [New] Added indexing type "spgist" to IndexingType class.
* [New] Model validation and export now works with threads.
* [New] Functions now have a "leak proof" attribute.
* [New] VARIADIC key attribute added to function parameters.
* [New] User now can completely disabled UI stylesheets by calling executable with param "-no-stylesheet".
* [New] Added the class Catalog to handle the basis of reverse engineering by reading the pgcatalog/information_schema.
* [New] System tablespaces pg_global and pg_default are now automatically created as new models are added.
* [Change] Minor change on schema area selection. The entire schema area and children can be selected (and moved) by using "Select children" action or SHIFT + left-click.
* [Change] Improved the transition between opened models. The problem was solved putting the temporary models saving in a separated thread.
* [Change] Minor improvement on model restoration operation. Models that fails to be restored can be kept so the user can try to fix them manually (until pgModeler is closed).
* [Change] Minor adjustments on model loading progress.
* [Change] Minor chages on model export form. Export to DBMS is the default option.
* [Change] Minor improvements on pgmodeler-cli. Added "--simulate" option to export dbms.
* [Change] Disabled notice output to console for Connection class. User can re-enable it by calling Connection::setNoticeEnabled() [recompile source needed].
* [Change] Minor changes on PgSQL base types. "[NO] LEAKPROOF" removed from FunctionType class.
* [Change] Minor improvements on SchemaParser, now its possible to make parser ignore empty attributes.
* [Change] libdbconnect renamed to libpgconnector for semantics reasons.
* [Change] Minor improvements on CrashHandler. Added buttons to load report and save embedded model when in analysis mode.
* [Fix] Minor fix on linuxdeploy.sh script related to grep command execution.
* [Fix] Fixed some leaks when destroying objects that are registered as PgSQLType (table, sequence, extension, domain, view, type). Now these type are correctly remove from user type listing.
* [Fix] Fixed bad sql code generation when disabling sql of columns/constraints.
* [Fix] Fixed dependency retrieving for operator classes.
* [Fix] Fixed incorrect reference to "replicate" option on Roles.
* [Fix] Fixed the "ignore duplicity" bug for columns when exporting model.
* [Fix] Fixed library build order now libpgmodeler is built before libpgconnector.
* [Fix] Minor fix on UI stylesheet related tooltips when using Fusion theme.
* [Fix] Fixed the assignment of LC_COLLATE, LC_CTYPE and template db to database instance on DatabaseWidget.


v0.5.2
------
<em>Codename: <strong>Lovely Duda</strong></em><br/>
<em>Release date: June 27, 2013</em>

* [New] User now can append free SQL commands to database objects via "Append SQL" command.
* [New] Introduced an experimental code completion on fields that permits code input.
* [New] User can create default sequences from serial columns. This feature does not apply to columns generated by relationships.
* [New] Introduced a feature to break relationship lines in straight angles and to remove all user added points.
* [New] Added support to change font size on textboxes.
* [Change] Removed the code "OIDs=FALSE" from table's SQL.
* [Fix] Minor fix on Mac OSX deployment script.

v0.5.1_r1
---------
<em>Codename: <strong>Lovely Duda</strong></em><br/>
<em>Release date: June 13, 2013</em>

* [New] Added deployment scripts on all platforms to compile and pack pgModeler. Note: On Windows the script must run on GNU environment port like Cygwin or MingW.
* [New] Added an special field to columns to easily identify which relationship generated it. (only for columns added by relationship)
* [Change] Model overview widget is now shown always stay on top.
* [Change] Minor improvements on syntax highlighter.
* [Change] Improvements on model validation widget. Output panel now shows the currently validated object (SQL validation).
* [Fix] Removed from user defined type DTD the mandatory use of collation object.
* [Fix] Collation's SQL generation corrected. Encoding is appended to LOCALE keyword.
* [Fix] Corrected the wrongly used COLLATION keyword on user defined type.
* [Fix] Corrected a bug that was crashing pgModeler when selecting relationship created objects on object finder result list.
* [Fix] Extension object naming corrected.
* [Fix] Extension object removal corrected.
* [Fix] Corrected a bug on CLI that was not finding dependencies paths correctly.
* [Fix] Minor fix on task progress widget on MacOSX.
* [Fix] Splash screen corrected on MacOSX
* [Fix] Corrected a bug on relationships that was crashing pgModeler when specifying column name pattern.

v0.5.1
------
<em>Codename: <strong>Lovely Duda</strong></em><br/>
<em>Release date: June 1, 2013</em>

* [New] Code base ported to C++11 and Qt5.
* [New] MacOSX compilation now generates an application bundle: pgmodeler.app
* [New] pgModeler is now capable of associate dbm files to its executable being possible opening a model from file manager by clicking it (except for MacOSX, see MacOSX notes).
* [New] Added support for loading models by calling pgModeler gui executable from terminal (e.g. pgmodeler model1.dbm model2.dbm)
* [New] pgModeler logo redesign.
* [New] Added special primary keys support to one-to-one and one-to-many relationships.
* [New] Relationships now supports patterns to define generated objects names. The manual suffix and auto-suffix generation are deprecated.
* [New] Columns/constraints generated by relationship can have position changed on parent table.
* [New] Added smallserial built-in datatype.
* [Change] Improvements on model validation tool. pgModeler will not save the model while isn't completely validated.
* [Fix] minor fix on point insertion on relationships.
* [Fix] Corrected the wrongly displayed interval fields.
* [Fix] Corrected self relationship validation.
* [Fix] Corrected a bug on editing numeric data types.
* [Fix] Minor fixes on build scripts (All platforms).
* [Fix] Added the missing directive CONFIG+=console to main-cli.pro
* [Fix] Minor fixes on task progress exhibition on quick tasks.
* [Fix] pgmodeler-cli output fixed on Windows system.
* [Fix] Table inheritance now copies columns with NOT NULL attribute correclty configured.
* [Fix] Plugin build scripts fixed. Now generated libraries are correctly copied to build directory.
* [Fix] Editing a column with 'numeric' datatype does not generate errors anymore.

v0.5.0
------

<em>Release date: May 17, 2013</em>

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
* [Change] Minor improvements on when showing Views.
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
* [Fix] Minor fixes on connection configuration form.
* [Fix] Corrected a bug that was crashing pgModeler when adding new schemas.
* [Fix] Corrected a bug that was crashing pgModeler when validation model.
* [Fix] Corrected a bug that was preventing the popup menu to be configured correctly on model widget.
* [Fix] Menu bar style correctly applied on Windows system.
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

v0.4.1_r1
---------

<em>Release date: March 19, 2013</em>

* [Change]: user can now prepend a CTE (commom table expression, a.k.a "with queries") on view's definition.
* [Change]: user can now create a single reference containing a expression that defines the entire view.
* [Change]: improvements on permissions, user now can control GRANTs and REVOKEs via permission editing form.
* [Fix]: fixed invalid UTF-8 chars on function definition.
* [Fix]: fixed unavailable "nocreatedb" role option.

v0.4.1
------

<em>Release date: March 16, 2013 </em>

* [New]: introduced the  "Disable SQL code" option for roles/tablespaces.
* [New]: user now can add objects by right-clicking group items on "Model Objects" dockwidget tree.
* [New]: added the abbreviation for time and timesptamp data types both with timezone: timetz and timestamptz.
* [New]: introduced a object highlight action on "model objects" dockwidget.
* [Change]: major changes on SQL code generation/export. Introduced a token to help export process to identify the end of each DDL command.
* [Change]: minor improvements on role editing form.
* [Change]: when generationg XML code empty tags that stores pure texts are now created with a <![CDATA[]]> tag in order to avoid malformed xml code.
* [Change]: index FASTUPDATE and FILLFACTOR params is now activated according the indexing type.
* [Change]: index fill factor now is optional.
* [Change]: chinese, portuguese and french translations update.
* [Fix]: pgModeler no longer crash when in error state (showing an exception) and try to auto save the models.
* [Fix]: minor size adjustments on forms.
* [Fix]: corrected a bug related to one-to-many relationship validation (endless looping) when changing to automatic suffix generation.
* [Fix]: corrected the "apply button disabled" bug on constraint edit form.
* [Fix]: IN/OUT keywords now appears on functions signature.
* [Fix]: corrected translation bypassing on index edit form.
* [Fix]: pgModeler no longer crash when triggering the print action.
* [Fix]: triggers no longer complains about assigning a function without parameters.
* [Fix]: corrected the loading process for indexes.
* [Fix]: corrected some bugs related to GiST and index sorting.
* [Fix]: minor fix on quick rename action when renaming a column with primary key.
* [Fix]: corrected a bug that was causing pgModeler to complain about duplicated elements when loading indexes.
* [Fix]: corrected a bug related to main window title when save a model with a different filename.
* [Fix]: fixed a bug related reload a model file after editing a foreign key.
* [Fix]: corrected a bug related to invalid chars at task progress.

v0.4.0_r1
---------

<em>Release date: March 04, 2013 </em>

* [New]: introducing the "pgModeler Wiki" as the main project's support resource.
* [Fix]: when main windows is closed the overview widget is closed too.
* [Fix]: corrected a bug on operation list widget that was converting an item name to UTF-8 twice.

v0.4.0
------

<em>Release date: February 27, 2013 </em>

* [New]: introduce a "New object" submenu when activating the schema context menu (right-click)
* [New]: tables and view are now graphically separated by colored rectangles representing its schemas.
* [New]: compiling pgModeler now works perfectly on Mac OSX system.
* [New]: introduced the 'Quick actions' menu that permits: rename, move to another schema, change onwer and edit permissions.
* [New]: the relationship editing form gained an "advanced" tab which shows the objects generated and/or represents the relatioship itself.
* [New]: the user now can add relationships only creating foreign keys on tables (fk relationships).
* [New]: added a french UI translation (provided by [toorpy](https://github.com/toorpy)).
* [Change]: all relationships type are now grouped together on "Model objects" widget.
* [Change]: chinese UI translation updated (provided by: [Bumanji](https://github.com/Bumanji)).
* [Change]: user now can remove fk relationships directly without needing to remove the related foreign keys.
* [Change]: field semantics adjustments on relationship editing form.
* [Change]: graphical object can be now selected and have the context menu activated only with a single right-click.
* [Change]: minor improvements on plugin base class: PgModelerPlugin.
* [Change]: widget size adjustments to better showing on Mac OSX system.
* [Change]: crashhandler now shows the compiled and running versions of Qt.
* [Change]: french UI translation reviewed and updated (provided by [babs](https://github.com/babs)).
* [Change]: 'Objects of Model' when used as object picker now expand all the nodes by default.
* [Change]: 'Objects of Model' now memorizes the tree state when update an object and / or opening another model.
* [Change]: PostGiS 'geometry' type can have a free assigned SRID value.
* [Change]: editing forms when shown set the focus on the first field, generally, the object name.
* [Change]: 'Objects of Model' widget displays the nodes in alphabetical order.
* [Change]: the printing options for the model were moved to the general configuration form.
* [Change]: relationship validation method now removes fk relationships when the foreign keys that gerenates is no longer exists.
* [Change]: copy/cut/delete commands does not manipulates system objects like schema public and languages C, SQL and plpgsql.
* [Change]: pgModeler startup scripts are now path location free meaning that software can be installed where the user desires.
* [Fix]: corrected a bug related  constraint name on domain XML code generation.
* [Fix]: corrected a bug that was causing crash when click "Apply" on Type editing form with fields not filled.
* [Fix]: corrected the "invalid constraint name" error on domain editing form.
* [Fix]: corrected the empty DEFAULT clause for columns, types and domains.
* [Fix]: corrected a bug related to incorrectly initialized OID attribute when creating tables.
* [Fix]: corrected a bug when creating a view with WHERE statement.
* [Fix]: corrected a bug related to one-to-many relationships semantics.
* [Fix]: corrected some bugs that was causing crash when removing all operations from operation list.
* [Fix]: minor bug fixes related to object selection over the model.
* [Fix]: corrected a bug on load model dialog filter (chinese UI only).
* [Fix]: pgModeler no longer crash when editing objects style.
* [Fix]: corrected bug that was deleting two sequeces at once.
* [Fix]: pgModeler no longer crash when removing (disconnecting) relationship that has special primary keys.
* [Fix]: minor fixes on the startup scripts on all platforms.
* [Fix]: corrected an incorrect reference to output stream on Windows system.
* [Fix]: shortcuts and popup menu now works correctly when selection an object on 'Objects of Model' tree.
* [Fix]: the pgsql base types (represented by tables, sequences, user defined types and domains) are now updated correctly when the related schema is renamed.
* [Fix]: corrected some weird SRID value on non spatial types.
* [Fix]: corrected bug on objects table when move rows to last / first.
* [Fix]: typos corrections on some error messages and dialog titles.
* [Fix]: 'referenced columns' combobox on constraint editing form are filled correctly when the dialog is shown in a second time.
* [Fix]: pgModeler no longer crash when creating many-to-many relationships.
* [Fix]: pgModeler no longer crash when the user activates the print dialog.
* [Fix]: corrected bug that was removing fk relationships when pasting objects.
* [Fix]: corrected SQL syntax error of 'timestamp with time zone'.
* [Fix]: corrected constraint type showing on editing form.
* [Fix]: corrected bug on cyrillic typed enums and check constraints expressions.
* [Fix]: corrected bug on enumeration type editing form.
* [Fix]: corrected bug on 'truncate' table privilege code generation.
* [Fix]: corrected column default value code generation.
* [Fix]: dummyplugin build process corrected on Windows.
* [Fix]: corrected bug on column comment code generation.
* [Fix]: corrected bug that was deleting two tables at once.

v0.3.4
------

<em>Release date: October 17, 2012</em>

* [New]: added chinese UI translation (provided by [gjunming](https://github.com/gjunming)).
* [New]: added basic support for PostGiS 2.0 only data types: box2d, box3d, geometry and geography (suggested by [george-silva](https://github.com/george-silva) on [issue#28](https://github.com/pgmodeler/pgmodeler/issues/28))(EXPERIMENTAL). Note: when using these data types make sure that PostGiS extension is installed on database cluster since pgModeler WILL NOT install it automatically or generate the command to do it!
* [New]: added a model restoration feature to reopen models after unexpected quit (crash).
* [New]: added a crash handler to pgModeler. Now signal SIGSEGV is trapped (in most cases) and the crash handler pops up permiting the user to generate an error report. (EXPERIMENTAL)
* [New]: to facilitate the error reporting exceptions stack now can be showed in text format. Users can post the complete error stack when creating an issue.
* [New]: icon added to pgModeler executable (Windows only)
* [Change]: update on pt_BR translation file.
* [Change]: removed "pgmodeler" prefix from translation files.
* [Change]: added the field "Underline" on textbox editing form.
* [Fix]: corrected the "AlwayOnTop" bug on model overview widget. ([issue#30](https://github.com/pgmodeler/pgmodeler/issues/30))
* [Fix]: little fix on startup scripts. Corrected de PGMODELER_ROOT on both Linux and Windows systems. ([issue#29](https://github.com/pgmodeler/pgmodeler/issues/29))
* [Fix]: corrected the referece to environment variables PGMODELER_*. Now pgModeler search for necessary paths on current directory if some of these variables are not set.
* [Fix]: corrected the validation of UTF-8 names that have 3 bytes length.
* [Fix]: corrected the sources path reference on project (.pro) files. Now lupdate command do not generates empty TS files.
* [Fix]: corrected a bug that was causing crash where user try to edit protected objects.
* [Fix]: corrected the exhibition of UTF-8 messages on ```throw``` statements.

v0.3.3
------

<em>Release date: October 09, 2012</em>

* [Change]: pgModeler license were update to GPLv3.
* [Change]: Error massages and entire UI were translated to en_US. Now people can contribute more easily with translation files. [(issue#8)](https://github.com/pgmodeler/pgmodeler/issues/8)
* [Change]: The left side image were removed form all forms giving more space to show widgets.
* [Change]: pgModeler now shows a messagebox at startup if any critical error is raised instead to show them on stdin.
* [Fix]: Translation files now are correctly loaded depending on system language. [(issue#23)](https://github.com/pgmodeler/pgmodeler/issues/23)
* [Fix]: Compilation process and execution is working correctly on Windows system. [(issue#11)](https://github.com/pgmodeler/pgmodeler/issues/11)
* [Fix]: No more crashes when dealing with relationships that have special triggers/indexes/columns. [(issue#8)](https://github.com/pgmodeler/pgmodeler/issues/8) [(issue#24)](https://github.com/pgmodeler/pgmodeler/issues/24)

v0.3.2
------

<em>Release date: September 27, 2012</em>

* [Change]: The default extension for the models now stands for ".dbm" [(issue#9)](https://github.com/pgmodeler/pgmodeler/issues/9)
* [Change]: Tables and sequences now can be used as function return type as well parameter type. This is valid for other objects that make use of base types (except for table columns).
* [Change]: The relationship conversion command now need to be confirmed by the user.
* [Fix]: Compilation process now works correctly on Windows system.
* [Fix]: Adjusted the size of some forms to show their fields properly.
* [Fix]: The "make distclean" command now make the correct cleanup on build/ directory.
* [Fix]: Startup scripts "start-pgmodeler.(sh|bat)" where adjusted. To prevent errors pgModeler need to be started through these scripts.
* [Fix]: Corrected the reference to the plugins directory. [(issue#7)](https://github.com/pgmodeler/pgmodeler/issues/7)
* [Fix]: The action "New Object -> Tablespace" now is displayed properly.

v0.3.1
------

<em>Release date: September 18, 2012</em>

* [New]: Relationships generates column suffixes automaticaly. This behavior can be changed on the relationship editing form.
* [New]: Added two samples to pgModeler.
* [Change]: Tables are now created with "With OIDs" attribute by default.
* [Change] The graphical update method on overview widget has improved preventing unecessary processing.
* [Fix]: Class CenaObjetos now doesn't delete objects twice.
* [Fix]: Eliminated bug that caused crashing on pgModeler when closing a model.

v0.3.0
------

<em>Release date: September 12, 2012</em>

* [New]: Added a model overview widget.
* [New]: Added export feature that generates PNG image of the models.
* [Fix]: Corrected the naming of columns generated by many-to-many relationships.
* [Fix]: Corrected generation of XML/SQL code by the model.

v0.2.0
------

<em>Release date: August 31, 2012</em>

* [New]: Added an interface to implement third party plugins. Check [PLUGINS.md] (https://github.com/pgmodeler/pgmodeler/blob/master/PLUGINS.md) for details.
* [New]: Added a short cut to easily control the zoom on the model. Use Crtl + Mouse wheel up (zoom up) or Crtl + Mouse wheel down (zoom down).
* [Change]: Due to the plugin interface the compilation method changed back to the form of shared libraries + executable.
* [Fix]: No more crashes when removing an primary-key of a table which has relationship with other tables. [(issue#2)](https://github.com/pgmodeler/pgmodeler/issues/2)
* [Fix]: Adjusted the semantics of one-to-one relationships.

v0.1.2
------

<em>Release date: August 24, 2012</em>

* [New]: Added a functionality to save modified models before closing the software.
* [Change]: Updated the en_US dictionary with the texts of the above functionality.
* [Fix]: Dockwidgets no longer disappear unexpectedly when the main window is minimized.
* [Fix]: Operations performed before creating a table object (column, constraint, trigger, index, rule) are no longer removed when any exception is thrown in the creation of these object.
* [Fix]: Fixed bug that caused user-defined types had wrong SQL/XML code generated by the model.
* [Fix]: Functions and Types received an own range of id in order to create these objects in a correct way.
* [Fix]: Eliminated segmentation faults caused by the destruction of relationships which possessed attributes/constraints.
* [Fix]: Adjusted the translation to SQL code of one-to-one relationships.
* [Fix]: Eliminated segmentation fault when editing relationships and/or undoing an operation involving a relationship.
* [Fix]: Identifiers relationships now correctly display the thick line beside the weak entity.

v0.1.1
------

<em>Release date: August 14, 2012</em>

* [Fix]: Correction of the actions for inserting graphic objects (table, text box, vision and relationship) in Windows environment.
* [Fix]: Correction on the display of the maximize button in the window decoration in Windows environment.
* [Fix]: Adjust on the position and spacing of widgets in editing forms.
* [Fix]: The XML parser can now correctly read DTD files in Windows environment.
* [Fix]: The compilation method is no longer in the form of shared libraries + executable and passed to be as standalone executable only.

v0.1.0
------

<em>Release date: August 9, 2012</em>

* First pgModeler release.
