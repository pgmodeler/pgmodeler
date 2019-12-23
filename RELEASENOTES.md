v0.9.2
------

<em>Release date: December 30, 2019</em><br/>
<em>Changes since: <strong>v0.9.2-beta2</strong></em><br/>

<strong>Summary:</strong> this is the end of a long development cycle that started still in May, 2018 when the stable 0.9.1 was released and is composed by 84 new features, 134 fixes and 146 changes in total. <br/>

This release was intended to be just a fine tuning of what was constructed in alpha and beta versions and it only includes new relavant feature included specifically to generate data dictionaries in HTML format from database models. <br/>

This was a request from many users that needed an extra form of documenting their databases. Data dictionary generation feature is still considered experimental despite of being a relatively simple functionality. So, there's always room for improvements. <br/>

Finally, the changes of this version are detailed below. For the complete list of changes/fixes, please, read the CHANGELOG.md. <br/>

* [New] Add support to data dictionaries generation in HTML format in ModelExportForm.
* [New] Added options to control data dictionary generation in CLI.
* [New] Plugins now can optionally be associated to a menu action or not. Generally, an plugin not associated to a menu action is automatilly executed during the startup (see PgModelerPlugin::initPlugin).
* [Change] Making BaseRelationship::getReferenceForeignKey() public.
* [Change] Isolated duplicated code in MainWindow::isToolButtonsChecked that checks if any tool button of the bottom or right widget bars is checked.
* [Change] Removing the plugins from core code.
* [Change] Modified pgmodeler.pro to include plugins folder when present in the source root (either in debug or release mode).
* [Change] Ignoring plugins folder in the core code.
* [Change] Minor improvement on ModelWidget::rearrangeSchemasInGrid in order to consider the amount of tables to determine the minimum grid size used to rearrange table.
* [Change] Minor text adjustments in CLI.
* [Change] Added an additional checking during relationship creation in order to avoid the creation of 1-* or n-n relationships involving partition tables.
* [Change] Removing the restriction to create 1-1, 1-n and n-n relationships in which the involved tables are partitioned ones.
* [Change] Disabling the SQL statment ALTER...OWNER TO in the object's SQL when the owner role has its SQL disabled but the object itself not. This will avoid reference errors when validating/exporting code.
* [Fix] Fixed the "Save" action enabled state according to the current model's modified state.
* [Fix] Fixed a crash when the user tried to edit connections in ModelDatabaseDiffForm and right after select a connection in the "Compare to" field.
* [Fix] Fixed the generation of escaped comments for columns.
