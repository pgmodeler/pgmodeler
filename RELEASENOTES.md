v0.7.2
------
<em>Codename: <strong>Brave Mastodon</strong></em><br/>
<em>Release date: June 30, 2014</em><br/>
<em>Changes since <strong>0.7.1</strong></em><br/>

* [New] Added support for event trigger objects. The export and import processes were adjusted to handle this kind of object.
* [New] Added support for UNLOGGED tables.
* [New] Enabled PostgreSQL 9.4 export and import processes.
* [New] Enabled the jsonb datatype for PostgreSQL 9.4.
* [New] Added an option to convert integer type to serial ones on columns which default value is a nextval(seq::regclass) call.
* [New] Introduced a "new object" overlay widget which gives user a quick access to actions that create objects.
* [New] Added a step to fix indexes with old <condition> tag on command line interface.
* [New] Added support to item interaction on "object's dependencies and references" dialog.
* [New] Added support to generate temporary names for database, roles and tablespaces when running validation process. This will avoid errors if the original database and the other objects already exists on the server.
* [New] Updated the CLI to include the option to generate temporary object's names.
* [New] Added suppport to save and restore the last position and zoom on the canvas. This behavior can be deactivated on general settings.
* [New] Added support to prepend SQL commands on object's definition.
* [New] Added zoom info popup that appears whenever the user changes the current zoom factor.
* [New] Added a basic routine to check if there is some new version available on pgModeler site.
* [Change] Rolled back to Qt 5.2.1 the version used by the macdeploy.sh script due to crash reported in issue #494 and #482.
* [Change] SQL tool improvement. Added a "drop cascade" action and fixed a bug when refreshing a specific item on database tree.
* [Change] Changed the MacOS X deployment script to use Qt 5.3.
* [Change] Minor size adjustment on the new object overlay.
* [Change] The model objects widget now clears the selection when it lost the focus.
* [Change] Minor change on new object overlay behavior. The overlay will be hidden when any tool button on it is pressed or the ESC key is pressed.
* [Change] The partial database import was improved. Now pgModeler is capable to resolve dependencies of foreign key constraints, creating the needed tables automatically. Additionally, domains are automatically imported when referenced by columns.
* [Change] Renamed the methods setCheckExpression and getCheckExpression methods to setExpression and getExpression because the expression is used either for check and exclude constraints.
* [Change] Renamed the attribute "condition" to "predicate" on index class.
* [Change] Improved the way SQL code is disabled/enabled on editing forms. User now is asked to apply the same disabling/enabling status to object's refereces.
* [Change] Minor size adjustment on SQL append dialog.
* [Change] Changed the order of the tabs on user-defined type editing form.
* [Change] Minor appearance improvement on textbox editing form.
* [Change] Custom  indexes for columns and constraints added by relationships are now stored on tables. In previous version the relationship was the responsible for that but this approach was provoking the bug related on issue 449.
* [Change] Remove unused parser attributes and commented old code.
* [Change] Removed attributes and methods from relationship which were responsible to control columns, attributes and constraints indexes.
* [Fix] Minor fix on ColumnWidget, disabled the line breaking in the field "Expression".
* [Fix] Fixed the model fix dialog on MacOS X and Windows platforms.
* [Fix] Schemas names are now correctly striked out on the rectangle when their SQL code is disabled. The system schemas public and pg_catalog will continue not to have their names striked out.
* [Fix] Fixed a bug that was preventing default values of domains to be loaded from file.
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
* [Fix] Made visible the field related to predicate expression on exclude constraint editing form.
* [Fix] Added reference checking between an exclude constraint and operators.
* [Fix] Minor fix on copy operation that was not resetting the copied objects list if the user clicked several times the "copy" command without paste.
* [Fix] Minor fix on the model object widget when disabling the controls.
* [Fix] Minor fix on ModelWidget::createSequenceForColumn() operation. The generated sequence is now correctly assigned to the column.
* [Fix] Minor fix on pgModeler command line interface menu text.
* [Fix] Minor fix on permission.dtd file. Attribute "index" was incorreclty marked as required when its optinal.
* [Fix] Minor fix on semantics of one-to-one relationship. When one of the tables are of mandatory participation the foreign key columns must not accept null values.
* [Fix] Minor fix on Windows and Linux deployment scripts.
* [Fix] Fixed a bug that was changing the columns positions whenever a relationship was edited on the model.
* [Fix] Minor fix when generate the string that denotes the assigned constraints to a column.
* [Fix] Minor fix on function editing form when handling the result table columns. The variadic field is disabled on the parameter form.
* [Fix] Fixed a crash when removing a view linked to tables.
* [Fix] Minor fix on view reference code generation.
* [Fix] Minor editing form size adjustments.
* [Fix] Minor compilation fix on Windows system.