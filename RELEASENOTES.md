v0.9.2-beta
------

<em>Release date: May 13, 2019</em><br/>
<em>Changes since: <strong>v0.9.2-alpha1</strong></em><br/>

<strong>Summary:</strong>

Finally, some of the key changes of this release are listed below. For the complete list of changes, fixes and new feautures, please, read the file CHANGELOG.md. <br/>

* [New] Added support to foreign server.
* [New] Added support to foreign data wrapper.
* [New] Generic SQL objects now support dynamic references to objects which can be used in the definition code.
* [New] Added support to compare foreign servers on diff process.
* [New] Added support to the reverse engineering foreign server objects.
* [New] Added code snippets for foreign data wrapper and foreign server.
* [New] Added support to diff foreign data wrappers
* [New] Added support to set permissions to foreign data wrapper.
* [New] Added the WRAPPER and SERVER key words to sql-highlight.conf.
* [New] Adding the ability to view references to store referenced tables making possible to identify which tables are linked to a view.
* [New] Added missing data type macaddr8.
* [New] Added support to result set filtering in the SQL execution widget.
* [Change] Change "New object" action in popup menu in order categorize object types when clicking the database object diminishing the amount of items displayed on the screen.
* [Change] Improved the object search mechanism in such way that various attributes of the object can be matched. New searchable attribute may be added in the future.
* [Change] Minor improvement on ModelDatabaseDiffForm to show the connection id of the databases being imported in the output tree.
* [Change] Improved the ModelValidationWidget in such way that is possible to operate over objects on the output list through their respective context menu (the same as in the ModelWidget).
* [Change] Now its possible to trigger the swap ids dialog for two selected objects, causing their ids to be swapped more quickly.
* [Change] Allowing copied object to be pasted multiple times. This feature works only with copy/paste operation without remove the pasted objects from the clipboard, for cut/paste the behaviour is unchanged.
* [Change] Increased the maximum limit of SQL history.
* [Change] Minor improvements in SQLToolWidget and SQLExecutionWidget to avoid segmentation faults when trying to close a execution tab while the command is still running.
* [Fix] Fixed a bug that was causing a fk relationship not to be deleted if the fk tied to it was changed by the user.
* [Fix] Fixed a bug on CLI that was not fixing broken models correctly when they had no role declaration.
* [Fix] Fixed a bug that was causing tables not to be moved on the canvas using mouse.
* [Fix] Fixed a bug that could crash the application when no language was specified to a funcion and the SQL/XML code was being generated.
* [Fix] Minor fix a bug on index importing.
* [Fix] Fixed a crash when a query executed in SQLExecutionWidget was a DDL one or was not returning results.
* [Fix] Fixed a bug that was causing syntax error if the last column of a table had the SQL code disabled.
* [Fix] Fixed a bug on diff process due to a missing attribute on the generation of diff code for inheritance relationships.
* [Fix] Fixed a bug when rendering several self relationships attached to the same table.
* [Fix] Fixed the CLI in order to restore the layers information when fixing a broken model.
* [Fix] Fixed a bug in object finder that was causing objects from a hidden layer to be displayed causing inconsistency on the layer state.
