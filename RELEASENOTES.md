v0.9.2-beta
------

<em>Release date: May 15, 2019</em><br/>
<em>Changes since: <strong>v0.9.2-alpha1</strong></em><br/>

<strong>Summary:</strong> a brand new version has come to you after a work of almost five months on it. This one brings important features requested long ago and several patches to improve the tool's usability. <br/>

pgModeler now supports foreign datawrappers and foreign servers natively. This means that you can create, export, import and diff these kinds of objects. For now, foreign tables aren't supported but the schedule is to release this support until the end of this development cycle when lauching the stable 0.9.2. <br/>

In order to provide a temporary workaround for the lack of foreign table support the generic SQL objects were improved in such way to store references to another objects in the model which helps to keep track of name changes and giving a touch of dynamicity to this that kind of object.  <br/>

Now its possible, for instance, create a foreign table from a generic SQL which references a foreign server and any name change on the referenced object will cause the code of the generic SQL (in this case, the foreign table) to be automatically updated avoiding export problems.<br/>

In SQL execution widget it's now possible to quickly filter the result set retrieved from a SQL command. This is useful if you need to filter a small set of data without the need of rewrite the the SQL command to get the desired result. The filter basically matches a keyword against a column of the rows on the result grid updating on-the-fly the data visualization. <br/>

This release also brings some changes in the available features: the first is the ability to search for objects in the canvas by matching other attributes not only the name. For instance, you can search items by matching a keyword against object's comments, data types, schemas and some others. <br/> 

A second change is the categorization of the action "New object" in order to diminish the amount of items displayed in the same menu and organizing object types hierarchically. <br/>

Now, for the bug fixes, in this release they fix crashes and broken code generation as well improve the objects handling in the canvas. <br/>

Lastly, some of the key change log entries of this release are listed below. For the complete list of changes/fixes, please, refer to the file CHANGELOG.md. <br/>

* [New] Added support to foreign server.
* [New] Added support to foreign data wrapper.
* [New] Generic SQL objects now support dynamic references to objects which can be used in the definition code.
* [New] Added support to compare foreign servers on diff process.
* [New] Added support to the reverse engineering foreign server objects.
* [New] Added code snippets for foreign data wrapper and foreign server.
* [New] Added support to diff foreign data wrappers
* [New] Added support to set permissions to foreign data wrapper.
* [New] Added the WRAPPER and SERVER key words to sql-highlight.conf.
* [New] Added the ability to view references to store referenced tables. This feature will cause relationships to be created between the view and the referenced tables. This is useful when we're using reverse engineering feature in which, in previous versions, couldn't determine the tables that were linked to a view. Now, with this feature a relationship is created between the view and all involved tables.
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
* [Fix] Fixed a bug that was causing domain constraints not to be extracted correctly during reverse engineering.
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
