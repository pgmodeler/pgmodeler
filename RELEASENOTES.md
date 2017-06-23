v0.9.0-beta2
------

<em>Release date: July 1st, 2017</em><br/>
<em>Changes since: <strong>v0.9.0-beta1</strong></em><br/>

<strong>Summary:</strong> this release was meant to be the final 0.9.0 but due to the several changes, bug fixes and new features introduced it was more prudent to bring it as the second beta instead of the stable version so we could gather as many as possible feedback and fix any remaining issue in order to finally launch the pgModeler 0.9.0 stable. <br/>

The first change presented by this version is that pgModeler will not fail or refuse to connect to a PostgreSQL server if the version of the latter is not supported yet. For example, we are about to receive PostgreSQL 10 but pgModeler supports only 9.0 to 9.6, so if you try to connect to a newer server pgModeler will connect to it but falling back to 9.6 until it receives the needed patches to be able generate code specifically to PostgreSQL 10. There is no magic here, pgModeler only relies in the backward compatibility of the server which is able to run code of the 9.x series without problems. If you plan to use this feature in PostgreSQL 8.x there are bad news for you: it'll not work and we really recommend you to stop using 8.x series.<br/>

The database base design was greatly improved by introducing: the objects' auto arrangement feature, the magnifier tool, the advanced object selection, the canvas info widget, the bulk relationships points removal and finally the generic SQL object. The auto arrangement feature consists in a process that will arrange tables and views in a hierarchical way by walking through the relationships that connect these objects placing them vertically aligned with their counterpart (parent). Tables and views without any link as well textboxes are placed at the bottom of the rearranged model.<br/>

The magnifier tool is the ability to show a portion of the zoomed out model where the mouse cursor is in the normal scaling so the user can identify which object is currently focused. Better than detail the feature here is to give it a try so you can understand how it works: in a zoomed out model (zoom factor < 100%) hold Ctrl + Alt and a preview box will appear at the bottom-right corner of the canvas; move the mouse and see the preview being updated. In this mode you can select the focused object or even activate the context menu for it.<br/>

The object selection was improved in such way that the user is able to select objects by their kind (tables, view, schemas, relationships, textboxes or all kinds) via "Select All" action in canvas' area context menu. Through this new feature, relationships can have all their custom points removed at once when all objects of this kind are selected. All these improvements in the design view led the creation of the canvas info widget which, currently, displays the current zoom factor, the cursor position in the canvas area, the selected object(s) postion and bounding rect at the bottom of the main window. This widget is quite simple but may receive improvements in future release.<br/>

We have the Generic SQL objects which is a way to use custom SQL code freely without attach them to other objects via Custom SQL dialog. In short, Generic SQL objects are plain text objects that store user defined code and the held code is executed in the order that it was created. As any object in the model, Generic SQL objects can have their creation order modified in the Swap Ids dialog to fit the user's need. The user must take extreme caution with these objects since pgModeler will not do any sanity check or even check if the code can harm the server when executed.<br/>

Finally, several fixes were done including a good amount of memory leaks that surely will improve the overall performance. Below some key changes for this release, please refer to the file CHANGELOG.md for the complete changes history:<br/>

* [New] Added an action to reset labels distance in BaseRelationship and ModelWidget.
* [New] Added a widget that shows some info about the canvas and the selected objects at the bottom of main window in design view.
* [New] Enabled the usage of snippets in other portions of the software like GenericSQLWidget, FunctionWidget, ViewWidget, CustomSQLWidget.
* [New] Added the ability to quickly jump to the tables related to a relationship.
* [New] Added support to select all objects in the canvas by type (table, view, textbox, schema, relationship).
* [New] Added support to bulk relationship points removal.
* [New] Added a magnifier tool so the user can visualize objects when the zoom is too small. This tool allows the user to click to select or activate the context menu over the objects.
* [New] Added support to generic sql objects that serve as an improved way to use custom SQL.
* [New] Added support to handle metadata related to generic sql objects.
* [New] Added the first object auto-arrange algorithm.
* [Change] pgModeler will now accept (connect) to a PostgreSQL server even if the version of the server is not supported falling back to the most recent supported.
* [Change] Improvements on import/diff/export performances by adjusting the way the output widget handles items height.
* [Change] Improved the ModelOverviewWidget to handle huge models better.
* [Change] Improved the objects swapping by adding an objects grid where user can interact with it in order to choose which objects to swap.
* [Fix] Several memory leaks removed in different portions of the application.
* [Fix] Minor fix in RelationshipView to show the line circles for n:n relationships.
* [Fix] Fixed a bug when zooming out using wheel that was causing duplicated zoom in a single wheel turn.
* [Fix] Minor fix in TableView and TableObjectView to present tables in a more compact fashion minimizing the space used in the canvas.
