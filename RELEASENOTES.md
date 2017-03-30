v0.9.0-alpha1
------

<em>Release date: April 2, 2017</em><br/>
<em>Changes since: <strong>v0.9.0-alpha1</strong></em><br/>

<strong>Summary:</strong> in this first beta release we have added some new design improvements as well important features and fixes that will improve the user's expirience.<br/>

One important feature introduced was the support to indexes in materialized views. This behavior is replicated to all areas of expertise of the software: design, code generation, reverse engineering, model/database comparison and database administration. This is a new step into the full support to all PostgreSQL objects and main features.<br/>

Instead of reinventing the wheel by creating an advanced source code editor we have added the ability to load contents of external source files and assigning them to objects which demands the usage of a SQL source, for instance, functions, views or custom SQL code attached to many objects. Also, the user can edit the current source in his/her prefered source code editor application. The default application used as well its startup parameters can be configured in the general settings.<br/>

Another cool improvement done in the design view was the ability of toggling the extended attributes area of tables and views on the fly. We call extended attributes those objects that are displayed at the bottom of tables and views which can be indexes, rules, triggers and constraints. Many users think that the extended attributes display is an useful feature for quick access purposes but others don't think the same and sometimes think that they can be annoying or take too much space in the canvas area. So we decided to provide a way, other than via general settings, to hide those attributes and persist their statuses (shown/hidden) in the dbm file so the current state can be restored in the next model loading. Talking about persisting statuses, we now added the ability to persist in the model file the fade out state of the objects in the design area. Still in the design area, the user can now control the zoom when navigating through the model via overview widget. To do that, just use the mouse wheel while the cursor is within the overview widget.<br/>

Finally, there were other minor features, fixes and improvements just to make pgModeler even better (some highlighted features below). The complete change log is available in the file CHANGELOG.md:<br/>

* [New] Added support to indexes in Views.
* [New] Added the support to edit/load the source code in NumberedTextEditor in external application.
* [New] Added the ability to save/load metadata related to fade out status and extended attributes display status.
* [New] Added the ability toggle the extended attributes area in tables and views. The toggle status is persisted in the model file and restores during loading
* [New] Added constraints to the extended attributes section in the tables at canvas area in order to improve the quick access to these objects.
* [New] Enabled the importing of view's indexes.
* [New] Fade status is now persisted in the dbm file and restored during loading.
* [New] Added the ability to control zoom factor from overview widget.
* [New] Added a shortcut for "Duplicate" action in design view.
* [New] Added support to (back)slash char in object's names.
* [New] Enabled the usage of NewObjectOverlayWidget for views.
* [Change] Changed the default characters used to escape values in DataManipulationForm and TableDataWidget from {} to // due to problems with json data.
* [Change] Improved the file manipulation in SQLExecutionWidget. Added option to save the commands to the current file or in another file (save as).
* [Change] View's children (indexes, rules, triggers) are now listed under their respective parent view in DatabaseExplorerWidget.
* [Change] Improved the diff between the complete database and a partial model representing it.
* [Fix] Added the missing support to drop event triggers from database model.
* [Fix] Fixed the drop cast command generation.
* [Fix] Minor fix in paste operation to restore the viewport position in design view.
* [Fix] Minor fix in diff process to detect view's index changes.
* [Fix] Fixed a bug in EventTrigger that was causing unknown exception to be thrown.
* [Fix] Fixed a bug on RoleWidget that was preventing roles to be removed from "Members Of" tab.
* [Fix] Minor fix in mouse cursor override operations.
* [Fix] Fixed a bug when importing functions and composite types that somehow depend upon array types.
* [Fix] Fixed a bug in function importing that was causing default values of parameters to be placed in the wrong position.
