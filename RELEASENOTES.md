v1.0.1
------
<em>Release date: February 17, 2023</em><br/>
<em>Changes since: <strong>v1.0.0</strong></em><br/>

This patch release for pgModeler 1.0.x brings the following changes and fixes:

* The appearance config now correctly adjusts the drop shadows on tool buttons improving the color contrast according to the current theme.
* When removing a primary key of a table in the editing dialog, pgModeler will now ask the user to uncheck all columns marked as PK in the Columns tab.
* The catalog query for transform objects was disabled for PostgreSQL 9.x to allow compatibility mode to work without errors when browsing databases on that version.
* Improving the visibility of some elements in the main menu bar and in the about widget.
* Fixed the assignment of layer colors when loading model created in older pgModeler versions using compatibility mode.
* When activating the action "Schemas rectangles > Hide" in the canvas area the child tables are properly updated in such a way to show the schema name in their title bars.
* Fixed the disable state change for delimiter scale lock action when changing the current main window view.
* Fixed the action "Open in file manager" in the file selector widget.
