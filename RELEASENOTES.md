v0.9.3-beta
------

<em>Release date: July 10, 2020</em><br/>
<em>Changes since: <strong>v0.9.3-alpha1</strong></em><br/>

<strong>Summary:</strong> here we are again bringing another round of improvements to pgModeler. This release basically enhances what has been done in 0.9.3-alpha1 plus bringing an important experimental feature: the partial diff.<br/>

After a entire month exclusively dedicated to the partial diff implementation we finally have something usable. This feature consists in taking an input database or model, apply user-provided filters and diminishing the amount of objects to be compared. This brings a significant gain of speed in the process because only a subset of the database/model will be handled. <br/>

In order to make the partial diff as user-friendly as possible, the database model received an internal changelog which consists on a list of modification made during a design session. This list can be queried in such way that only the objects modified that match the filter(s) criteria are used in the partial diff process. </br>

Talking about model's internal changelog, this one can be persisted into the model file so it can be restored in the next design session, thus, the user is free to apply a partial diff anytime as long as the changelog isn't cleaned up. <br/>

The database import process was patched in such way that a sequence will be automatically created when a column refers it from the default value. This will happen only when the automatic dependency resolution is active.  <br/>

The objects filtering widget introduced in 0.9.3-alpha1 was slightly redesigned by moving options to a popup menu in order to make the widget more compact and easier to use. <br/>

The command line interface received lots of improvements too, from a simple menu redesign to the support to partial diff.

Several bugs were fixed in this release too. The majority related to crashes but there was some fixed bugs that were affecting the database design and the diff process. <br/>

Finally, some of the change log entries in this version are listed below. For the complete list of changes/fixes, please, read the CHANGELOG.md. <br/>

* [New] Added support to partial diffs between a model and database or between two databases.
* [New] The CLI now validates the mixing of options of different operation modes.
* [New] Created an internal changelog on DatabaseModel to register object's modification over time being useful for partial diff operations.
* [New] Added the widget called ChangelogWidget to control the model's internal changelog settings in the design view.
* [New] Added support to save/load changelog from model file.
* [New] The CLI now supports partial diffs too.
* [New] Added a warning message prior to the partial diff without using "Do not drop missing objects" option.
* [New] Added cast and user mapping to the list of filterable objects.
* [New] Added a hint text in ObjectsFilterWidget to inform about exact match searching.
* [New] Added support to DatabaseImportHelper to create a sequence assigned to a column via default value if automatic dependencies resolution is enabled.
* [Change] Improved the sequences assignments to columns in DatabaseImportHelper::assignSequencesToColumns.
* [Change] Minor adjustment on objects grids in SwapObjectsIdsWidget, DatabaseImportForm and ModelDatabaseDiffForm by changing the columns order.
* [Change] Minor change in ObjectsFilterWidget by making the action "Only matching" checked by default.
* [Change] Minor improvement on CLI to accept the value "all" for the paramenter --force-children in order to force all table children at once.
* [Change] Improved the objects filtering in such way to allow filter by name or signature.
* [Change] Improved the UI of ObjectsFilterWidget by moving all options to a popup menu.
* [Change] Several changes in all catalog queries in order to support signature matching.
* [Change] Adjusted the appimages building process.
* [Change] Changing the default font size of graphical objects to 10pt in order to try to solve the intermittend issue of disappearing texts.
* [Change] Updated the French translation.
* [Fix] Fixed a regression in ObjectFinderWidget that was not opening objects form with double-click on an item on the results grid.
* [Fix] Fixed a bug when drawing relationships in FK to PK connection mode. Now when one of the tables is collapsed the center points of both are used as connection points to the relationship.
* [Fix] Fixed a crash in ModelDatabaseDiffForm while enabling/disabling the partial diff tab.
* [Fix] Fixed the diff process for inheritance relationships created for two existing tables.
* [Fix] Fixed a crash in LayersWidget when renaming a layer.
* [Fix] Minor fix in DatabaseImportHelper::assignSequencesToColumns.
* [Fix] Minor fix in ConstraintWidget that was trying to validate FK relationships for generated tables of many-to-many relationships.
* [Fix] Fixed a bug in Catalog::getObjectsOIDs that was executing a catalog query for a certain object when it was not being filtered.
* [Fix] Minor fix in ModelDatabaseDiffForm tabs enabling/disabling steps.
* [Fix] Fixed the QProcess usage in ModelFixForm due to QProcess::start() deprecation in Qt 5.15.
* [Fix] Fixed a crash when trying to move several objects to a layer from the object finder widget and in the selected set one or more objects aren't graphical ones.
