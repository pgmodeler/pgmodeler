v0.8.0-alpha1
------
<em>Codename: <strong>Faithful Elephant</strong></em><br/>
<em>Release date: August 15, 2014</em><br/>
<em>Changes since: <strong>v0.8.0-alpha</strong></em><br/>
<strong>Summary:</strong> dispite the designation <em>alpha1</em> this release brings important improvements on stability and speed being more reliable than the current stable release <strong>0.7.2</strong>. The key features of this version are: it's possible to handle table's data through the "data grid" feature on SQL tool, several improvements on SQL tool related to command execution, improved validation process, improved code generation speed, improved startup speed, and many others. Update is <strong>HIGHLY RECOMMENDED!</strong><br/>
<strong>NOTE:</strong><em> models generated in previous versions will demand the use of model fix feature!</em>.

<h4>Change log</h4>
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
