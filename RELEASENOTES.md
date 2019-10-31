v0.9.2-beta2
------

<em>Release date: November 4st, 2019</em><br/>
<em>Changes since: <strong>v0.9.2-beta1</strong></em><br/>

<strong>Summary:</strong> the development of this new version was strongly focused on delivering the support to foreign table objects. This way pgModeler is now capable of handling all foreign objects implemented by PostgreSQL. <br/>

There was other important improvements including the support to diff presets which let the user to configure a set of options enabled/disabled for the diff operations. <br/>

Due to the release of PostgreSQL 12 some patches were applied to pgModeler in order to allow the users to connect to database servers of that version. This will allow all operations that depends on connections like export, diff, import to be performed on newer versions of PostgreSQL. <br/>

Lots of bugs were fixed too including crashes, false-positive results on diff feature and some problems on reverse engineering that was causing some kind of objects to be imported wrongly. <br/>

Finally, some changes of this version are detailed below. For the complete list of changes/fixes, please, read the CHANGELOG.md. <br/>

* [New] Added support to foreign tables.
* [New] Added support to select all foreign tables in ModelWidget.
* [New] Added support to swap objects ids using arrow keys by changing creation order between two close objects on the grid.
* [New] Added support to save diff settings in form of presets.
* [New] Added an option to BaseObject to force the comment escaping making multilined comments be presented as single lines but without changing their semantics.
* [New] Adding support to parse escaped text delimiters and value separators in CSVLoadWidget.
* [Change] Avoid unchecking PK checkboxes on TableWidget when adding a new column.
* [Change] Validating the dialogs geometry regarding to screens geometry in order to avoid their appearance in an invalid position (out of range).
* [Change] Restricting the usage of "Default partition" option for partitioning relationships when the partition table is a foreign table.
* [Change] Improved the Relationship class to accept foreign tables.
* [Change] Updated View class to accept reference foreign table columns.
* [Change] Moved the original implementation of DatabaseModel::createTable to a template method createPhysicalTable in order to reuse it to create tables and foreign tables.
* [Change] ForeignObject is not a BaseObject child anymore due to multi inheritance problem that it can generate in ForeignTable class.
* [Change] Disabling mime type update on installers.
* [Change] Changed the way temp source file is handled by the tool in order to solve a sharing violation problem on Windows.
* [Fix] Fixed an bug in SQL generation of columns that was wrongly removing a comma from decimal data types.
* [Fix] Fixed some catalog queries in order to support PostgreSQL 12.
* [Fix] Fixed the ObjectFinderWidget in order to fade out correctly the listed/not listed elements.
* [Fix] Fixed the quick actions menu at ModelWidget in order to include the "Edit data" action for foreign tables.
* [Fix] Fixed the DatabaseImportHelper::assignSequencesToColumns in order to correctly assign sequences to foreign table columns.
* [Fix] Fixed a crash in ModelValidationHelper that was caused by wrong checking when validating generic sql objects.
* [Fix] Fixed the source code display for tables and foreign tables on DatabaseExplorerWidget.
* [Fix] Fixed the CodeCompletionWidget to display foreign table children.
* [Fix] Fixed the diff process in order recognize partition foreign tables attach/detach.
* [Fix] Fixed the generation of XML code of partitioning relationships.
* [Fix] Improved the DataManipulationForm in order to restore the columns visibility when retrieving again the data of the current table.
* [Fix] Fixed the drop action in DatabaseExplorerWidget for user mappings.
* [Fix] Fixed the generation of SQL code of partition tables in order to avoid the inclusion of partitioned tables columns on the code.
* [Fix] Fixed a crash on DatabaseImportHelper when destroying detached inherited columns.
* [Fix] Fixed a regression when importing permissions related to functions.
* [Fix] Fixed the SwapIdsWidget postion/size saving and restoration.
* [Fix] Additional fix to the extension import to correctly indetify it as a data type handler.
* [Fix] Fixed a bug on Role's SQL generation due to a missing attribute.
* [Fix] Additional fix in GeneralConfigWidget to save correctly the settings of dockwidgets.
* [Fix] Minor fix in DatabaseModel to write ddl end token in the appended/prepended custom code.
* [Fix] Fixed a false-positive result when doing the diff between two fuctions that contains the same signature.
* [Fix] Fixed the diff generation for Role objects when setting up an blank password.
* [Fix] Fixed the "Prepend at the beggining of the model" option behavior on CustomSQLWidget.
* [Fix] Fixed the catalog query for extension in order to indetify correctly if this object handles a data type.
* [Fix] Fixed the "Clear items" action in popup menu at DataManipulationForm.
* [Fix] Fixed the importing of tables and views in such way to automatically create correctly the domains referenced by their columns.
* [Fix] Fixed a bug on ModelDatabaseDiffForm that was causing the form to be reseted on the middle of a diff process when the user minimize the diff dialog causing unexpected behavior (or even crashes sometimes).
* [Fix] Minor fix in ModelDatabaseDiffForm to avoid using uniform items height in output widget.
