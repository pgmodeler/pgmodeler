v0.9.1
------

<em>Release date: May 14, 2018</em><br/>
<em>Changes since: <strong>v0.9.1-beta1</strong></em><br/>

<strong>Summary:</strong> after eight months of hard work we finally have the stable release 0.9.1. This version is a mark in the pgModeler's development and so important as the release 0.8.2 in terms of improvements because it brought features requested long ago and that will make pgModeler even better to work with, being some of them: crow's foot notation support, multiple relationships for the same table pair, support to row level security, identity columns and much more. <br/>

For this release specifically (considering only the work after 0.9.1-beta1), the golden rule was to fix bugs, adjust the current features and introduce few new things and thus was done: 21 bug fixes, 14 changes/improvements and 7 new features.

The majority of new features are related to the CLI which received the ability to import and diff databases which can make it even better to integrate to automtated deployment processess or run quick tasks over models or databases without the need to open pgModeler's GUI.<br/>

Some important improvements were done in the overall performance of the SQL tool. The query execution and results display are a lot better now being quicker and less eager in terms of memory consumption avoiding crashes when querying large tables. The data manipulation also received a patch that makes the row duplication more efficient.<br/>

Now, for the fixes, another patch was introduced in order to resize correctly dialogs in HiDPI/Retina displays. Several reverse engineering bugs were fixes and now databases are imported more precisely compared to previous releases. Lastly, a bug that seems to happen only on macOS while dealing with large models is finally fixed and was related to some running threads messing around with the main process avoiding it to correctly save the model and causing hang ups sometimes.<br/>

The whole set of changes of this release (considering the alpha and beta stages) has 152 entries being 43 new features, 47 changes/improvements and 62 bug fixes. Below we highlight some of news for the work since 0.9.1-beta1, for the complete list of changes, please, take a look into the CHANGELOG.md. <br/>

* [New] Added support to line selection by clicking and moving the mouse over the line numbers widget in any source code field.
* [New] The validator now checks if the model has columns referencing spatial data types and creates the postgis extension automatically when fixing the model.
* [New] Added support to RESTART IDENTITY on truncate tables in DatabaseExplorerWidget.
* [New] Added support to diff operation in CLI.
* [New] Added support to import database from CLI.
* [New] Adding missing types regrole and regnamespace.
* [Change] Improved the copy/duplicate operation in order to copy rules, index, trigger and policies together to their parents.
* [Change] Added column names to the code completion widget used in the filter widget at DataManipulationForm.
* [Change] Improved the SQLExecutionWidget in such way that it'll display large amount of data more quickly and consuming less memory.
* [Change] Minor improvement in SQLExecutionWidget to show the amount of time took to run a query.
* [Change] Minor improvement in the text find widgets in SQL tool in order to make them closable via dedicated button.
* [Change] Improved the set tag operation in ModelWidget in order to cleanup the assigned tags to a set of objects.
* [Change] Refactored all the CLI options.
* [Change] Improved the performance of the row duplication action in DataManipulationForm.
* [Change] Improved the performance of "Move to schema" operation.
* [Change] Changed the behaviour of select and fade buttons in ObjectFinderWidget in such way to enable the user to select/fade the objects in the listing (or not included in the results).
* [Fix] Fixed a bug when import identity columns in certain cases when the identity column was followed by another column which data type was not accepted for identity, e.g, text after smallint.
* [Fix] Fixed the tab behavior on comment box in all editing forms of database objects.
* [Fix] Fixed the catalog query for user defined types.
* [Fix] Fixed the import of user defined types which names contains uppercase characters.
* [Fix] Fix window scaling on HiDPI/Retina screens.
* [Fix] Fixed a bug in ModelExportHelper that was failing to remane the database when the command appeared.
* [Fix] Fixed a bug in CollationWidget that was referencing the collation attributes LC_??? using the wrong constant.
* [Fix] Fixed the behaviour of the message box that warns about the need of validate the model prior to export, save or diff. Now rejecting the dialog (i.e. closing it) will be considered that the user wants to proceed with the pending operation even with an invalid model.
* [Fix] Fixed the import of comments for constraints,triggers, index and rules.
* [Fix] Fixed a bug in the aggregate import process.
* [Fix] Minor fix in DataManipulationForm to avoid the generation of a where clause when the filter is filled only with spaces.
* [Fix] Minor fix in the magnfier tool to use the same render hints as the canvas viewport.
* [Fix] Fixed a bug in the diff process that was trying to recreate the whole database when the "Force recreation" option was set.
* [Fix] Fixed a bug when showing the source of tables in DatabaseExplorerWidget when these objects have permissions assigned.
* [Fix] Fixed the saving process for large models by stopping the threads related to temp models saving while the model file is being written.
