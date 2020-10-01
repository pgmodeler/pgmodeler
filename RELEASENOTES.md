v0.9.3-beta1
------

<em>Release date: October 5, 2020</em><br/>
<em>Changes since: <strong>v0.9.3-beta</strong></em><br/>

<strong>Summary:</strong> we are quite close to the end of another amazing development cycle with this last beta release of 0.9.3! <br/>

This launch brings mainly bug fixes but in order to make pgModeler even more complete, we've added full support to procedures and transforms. <br/>

Additionally to new things, pgModeler was improved in such a way to render database models more properly in high DPI screens. <br/>

There were some important changes in the code generation and exporting too. For the former, the SQL code of the database object will respect the disabled status. In previous versions, the SQL code was always deactivated. <br/>

The model exporting process will now abort the execution if the database object has it's code disabled. This behavior does not apply to model validation which forces the enabling of the database object's SQL so the whole model can be properly checked. </br>

pgModeler now will not generate ADD CONSTRAINT instructions for constraints during diff when the parent table is also being created in the process. <br/>

Also, default values and functions arguments containing json/jsonb values are now properly formatted and saved to the XML code avoiding corruption of the database model in very specific cases. <br/>

Finally, some of the changelog entries in this version are listed below. For the complete list of changes/fixes, please, read the CHANGELOG.md. <br/>

* [New] Added the version descriptor for PostgreSQL 13.
* [New] Added support to procedures in design, import and diff processes.
* [New] Added support to transforms in design, import and diff processes.
* [New] Added support to modifying attributes toggler colors from appearance settings.
* [New] Tag objects now include attribute toggler colors.
* [Change] Changed the behavior of the generation of SQL code for database object, now it'll respect the SQL disabled status of the object.
* [Change] The ModelExportHelper will abort the export process if the SQL code of the database object is disabled.
* [Change] The database model is now flagged as modified everytime the objects are swapped.
* [Change] Improved the ObjectSelectorWidget in order to save/restore the geometry of internal ModelObjectsWidget instances.
* [Change] pgModeler will alert about a possible data/work loss if the user is trying to save a model in which there're other instances loaded other tabs.
* [Change] Making the class Function be a direct child of BaseFunction.
* [Change] Moved the common code between functions and procedures to a base class called BaseFunction.
* [Change] Replaced the attributes PhysicalTable::DataSeparator and DatabaseExplorerWidget::ElemSeparator usages by PgModelerNs::DataSeparator.
* [Change] Refactored the schema files in order to remove code duplication related to ddl-end token.
* [Change] Minor improvement in ConfigurationForm by adding a splitter between config items (left) and settings page (right).
* [Change] Minor improvements on objects rendering in order to consider screen dpi when configuring objects border sizes.
* [Change] Minor refactoring in the parameter/signature generation in class Function.
* [Change] Making the NewObjectOverlay less transparent in order to enhance reading.
* [Fix] Fixed a bug in constraint.sch that was avoiding the correct importing of exclude of constraints.
* [Fix] Minor fix in the SyntaxHighlighter in order to highlight correctly multline blocks (specially comments).
* [Fix] Fixed a bug in DatabaseImportHelper that was causing failure when importing some objects' permissions.
* [Fix] Fixed a bug on MainWindow that was wrongly showing the main menu bar in certain cases at startup.
* [Fix] Minor fix in BaseFunction::createSignature in order to remove OUT keywords from signature.
* [Fix] Fixed a bug when importing triggers in which functions arguments contain json/jsonb values. Now values are properly formatted.
* [Fix] Fixed a bug in XmlParser::convertCharsToXMLEntities that was not converting json/jsonb default values correctly breaking the entire XML code of the database model.
* [Fix] Fixed a bug in Parameter class that was causing default values to be ignored.
* [Fix] Fixed a bug in SchemaParser related to exceptions being raised wrongly in expressions evaluation.
* [Fix] Fixed a bug in ModelFixForm that was passing arguments to CLI in wrong format.
* [Fix] Fixed a bug in CLI that was ignoring input-db parameter when doing diff.
* [Fix] Minor fix in the graphical objects rendering in 4k screens when QT_AUTO_SCREEN_SCALE_FACTOR is set to true. Now they are rendered in acceptable proportions.
* [Fix] Minor fix in ModelsDiffHelper in order to avoid generating ALTER...ADD COSTRAINT for constraints (check and unique) in which parent table is also being created.
