v0.8.0-alpha2
------
<em>Codename: <strong>Faithful Elephant</strong></em><br/>
<em>Release date: September 30, 2014</em><br/>
<em>Changes since: <strong>v0.8.0-alpha1</strong></em><br/>
<strong>Summary: this release brings important fixes that improved stability as well small features as a preparation for model-database diff scheduled to the first beta. The key feature for this release is that the code is able to be compiled using the lastest Qt version (5.3.2) without issues or crashes. All binaries releases are now aligned to the same Qt version.

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
