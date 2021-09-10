v0.9.4-beta
------

<em>Release date: September 17, 2021</em><br/>
<em>Changes since: <strong>v0.9.4-alpha1</strong></em><br/>

<strong>Attention:</strong> the database model file structure has changed since the last stable 0.9.3. Models created in older releases will certainly fail to load due to incompatibilities because some attributes in the XML code don't exist anymore or have changed during the development of 0.9.4-beta. Before loading your database models in this new release, please, make sure to have a backup of them all and follow the steps presented by the model fix tool to patch the documents' structure. Not paying attention to this situation may cause irreversible data loss! If the fix procedures aren't enough to make your database models loadable again, please, ask for help at the official support channels!<br/>

<strong>Summary:</strong> this release took a bit more to be released than expected but it's finally here. As any other beta version, this one brings few new features since it is more focused on polishing all that was made on alpha stages in preparation for the stable version. <br/>

One new feature brought is the ability to generate GRANT and REVOKE commands to setup new role memberships through the diff process. Another addition is the support for included columns also known as non-key columns on indexes. <br/>



* [New] Added support for the generation of GRANT/REVOKE commands to control roles memberships via diff process.
* [New] Added support for included columns on indexes. 
* [New] Added the ability for referencing view columns on indexes.
* [New] Added support for toggling update notifications for alpha/beta versions.
* [New] Added support for save and restore the tree state in DatabaseExplorerWidget.
* [Change] Restored the file pgmodeler.appdata.xml.
* [Change] Allowing swap cluster level object ids in SwapObjectsIdsWidget when the objects are of the same kind.
* [Change] Improvements in pgmodeler-se in such a way to control syntax highlighting settings per open editor.
* [Change] Avoided the use of a working dir in .desktop file when installing mime types system wide.
* [Change] Improved the way layers rectangles are updated after undoing/redoing operations in OperationListWidget.
* [Change] Changed the default match type in foreign key constraint to MATCH SIMPLE.
* [Change] Minor adjustment in the ModelWidget::updateSceneLayers in order to force the update of all schemas.
* [Change] Exceptions raised in BugReportForm when the "lastmodel" file isn't found are now ignored.
* [Change] Minor change in ModelValidationHelper in order to use table's signature instead of name during name validation process.
* [Fix] Fixed a crash in diff process caused by the ModelExportHelper instance when canceling the export to DBMS when an exception was raised.
* [Fix] Fixed a crash in SwapObjectsIdsWidget when the user tried to swap objects using arrow keys.
* [Fix] Minor fix in DatabaseExplorerWidget in order to display view's children source code.
* [Fix] Fixed the building of IndexWidget on Qt 5.14 and below.
* [Fix] Fixed the problem on ModelDatabaseDiffForm that was not accepting pressing Return/Esc to respectively run the diff and close the dialog due to a conflict with the event loop instance in ModelDatabaseDiffForm::exec().
* [Fix] Minor fix in the configuration of ObjectGroupId element in DatabaseImportForm::updateObjectsTree in order to avoid crashes when no root element is specified.
* [Fix] Fixed the generation of aggregate functions when they use functions that contain parameters in which data type have spaces (double precision, character varying).
* [Fix] Fixed a segfault that can happen when changing the number of parameters of a function already associated with an aggregate.
* [Fix] Additional fix for layes not being correctly loaded from dbm file.
