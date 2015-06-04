v0.8.1-alpha1
------
<em>Codename: <strong>Faithful Elephant</strong></em>
<em>Release date: June 04, 2015</em>
<em>Changes since: <strong>v0.8.1-alpha1</strong></em><br/>

<strong>Summary:</strong> this release brings a another series of fixes for the majority of bugs reported for 0.8.1-alpha1. Several fixes and improvements were done to import, diff and export processes producing better results.

There are some UI improvements mainly related to usability in SQL tool to avoid the excessive use of button clicks to manage databases. One long awaited feature is now fully operational: the automatic keywords quotation when they are used as object's name. This feature is still under tests but will be improved until final release of this series.

The user's privacy was improved in this version too. Now the pgModeler's temp folder - were logs and security copies of working models are stored - is isolated in current user's home avoiding other users to see the contents of that folder.

Finally, several crashes were fixed and the application is more reliable. There are more fixes/changes/new features in this version and only the key ones are listed below. Please, refer to CHANGELOG.md to check the complete set of modifications.

* [New] Added the ability to handle databases in different connections at once without the need to disconnect from a server and connect to another.
* [New] Added an option to preserve database name (do not rename) in diff process.
* [New] Added an automatic quotation mechanism for PostgreSQL's reserved keywords when they are begin used as object's name.
* [New] Created a new class NumberedTextEdit which goal is to display source code with line numbering and highlight color.
* [Change] Improvements done to DataManipulationForm in order to avoid let connections open leading to application crash when the connection timeout is reached.
* [Change] Temporary folder now is isolated in pgModeler's configuration dir at user's home (platform specific) to avoid a user to see temp files from another.
* [Change] Disabled the cascade drop for roles and tablespaces (not supported) in DatabaseExplorerWidget.
* [Change] Minor improvement on SQLToolWidget to remove the excessive usage of buttons to connect and browse databases.
* [Change] Minor improvement on DataManipulationForm to load the first 100 rows of the current table each time the index of combobox changes.
* [Fix] Fix a crash when recreating Views that have child objects (rules or triggers) in DatabaseModel::storeSpecialObjectsXML.
* [Fix] Fixed a bug on DatabaseExplorerWidget that was selecting the wrong database when opening the data grid.
* [Fix] Fix a bug on DataManipulationForm that was preventing user to delete rows in tables with no primary key.
* [Fix] Fix a bug on diff process that was not detecting precision/length changes in column types.
* [Fix] Fixed a bug in CLI that was not considering the PWD when dealing with input/output files.
* [Fix] Fix a crash in PgModelerCLI when exporting model to PNG.
* [Fix] Fixed a bug on SyntaxHighlighter that was rehighlighting endlessly a document leading to application crash.
* [Fix] Fixed a crash when creating a new table containing a new index.
* [Fix] More fixes related to inherited columns and check constraints. Now diff infos containing modification for these kind of objects are discarded.
* [Fix] Fixed the import of inherited check constraints.
