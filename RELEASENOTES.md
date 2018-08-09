v0.9.2-alpha
------

<em>Release date: August 17, 2018</em><br/>
<em>Changes since: <strong>v0.9.1</strong></em><br/>

<strong>Summary:</strong> this release does not have an extensive change log but brings some long desired features as well important fixes. The first feature to talk about is the long awaited SQL execution cancelling which enables the user to abort the execution of any command issued to the server in the SQL execution field (Manage view) without the need to kill the application in order to interrupt the running SQL (like on previous releases). <br/>

Another feature introduced is the compact view of the model. This one makes the tables and views smaller by hiding the columns' data types and other informations. This feature came as a support to those developers who need to present their models to an audience without database specific knowledge. The trick here is to replace the original names of graphical objects by more friendly ones so the audience can understand the essence of the model being worked without worry about the tech details. <br/>

More features were implemented in this release too being them: the support to sequence options on indentity columns, the support to bulk data edition on data manipulation dialog, the support to paste CSV text from clipboard into the dialog where initial data can be specified for tables (in design view) and some other. <br/>

One important change done was a partial refactoring of the View editing form by moving the fields related to view's references to a dedicated dialog making the whole form a bit less complicated to use. I have plans to keep improving that dialog in special but in future releases a full UX/UI remodeling will be done in the application. <br/>

Below some of the changes of this release, for the complete list of changes, please, take a look into the CHANGELOG.md. <br/>

* [New] Added the support to cancelling SQL execution in SQLExecutionWidget.
* [New] Added support to truncate tables in DataManipulationForm.
* [New] Added support to aliases on some graphical objects to be is used in the compact view mode.
* [New] Added support to save/load object's metadata containing aliases information.
* [New] Added support to compact view of the model where graphical objects can have a more friendly name for a reduced view as well for those who don't need to see details about tables (clients of the business, for instance).
* [New] Added support to sequence options for identity columns.
* [New] Added the ability to paste CSV text from clipboard into the TableDataWidget.
* [New] Added support to bulk data edit in TableDataWidget.
* [Change] Changed the behaviour of the fade in/out of relationships linked to a table by applying the effect on the other tables that are related to the selected one.
* [Change] Refactored the view editing dialog by moving the references handling form to a dedicated modal dialog.
* [Change] Improved the model loading from file by blocking signals of relationships avoiding excessive/repetive rendering of objects. The whole model is fully rendered when the file was completely loaded.
* [Change] Minor adjustment on constraints rendering at extended attributes section of tables.
* [Fix] Minor fix on stylesheet in order to display the extended button on general toolbar.
* [Fix] Fix a shortcut conflict in DataManipulationform.
* [Fix] Fixed the offset of strings composing the StorageType.
* [Fix] Minor fix in SQLExecutionWidget allowing the output widget to be resized to a size lower than the default one.
* [Fix] Fixed the generation of Database object source in DatabaseExplorerWidget.
* [Fix] Minor fix in HintTextWidget to stay on top of all widget when being displayed.
* [Fix] Fixed a bug that was not quoting extension name when needed.
* [Fix] Fixed a crash when trying to remove a fk relationship when it was created from a foreign key which references protected columns (added by relationship).
* [Fix] Fix a crash when importing CSV files into DataManipulationForm.
