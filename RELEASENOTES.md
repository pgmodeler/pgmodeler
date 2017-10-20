v0.9.1-alpha
------

<em>Release date: October 20, 2017</em><br/>
<em>Changes since: <strong>v0.9.0</strong></em><br/>

<strong>Summary:</strong> this release opens the path to the new major release (0.9.1) bringing some new features based upon old requests, improvements to current features and fixes for bugs reported after the launch of 0.9.0 last month. <br/>

This first alpha release of 0.9.1 branch finally introduces the Crow's foot notation bringing an improved readability to the database models. The support is still experimental and has some limitations that may be removed in future releases. The new notation can be toggled in the relationship settings tab at configurations form.<br/>

Another addition done in pgModeler is the ability to toggle schemas' rectagles at once without the need to do it object per object. To do it, just right click a blank portion of the canvas area and use the 'Schemas rectangles' action. Still related to schemas, a new automatic arrangenment algorithm was introduced and will scatter tables inside their respective schemas and then scatter the schemas themselves. <br/>

Now, for the bug fixes, most of them is related to the code generation, reverse engineering and diff process making these features more accurate and reliable. <br/>

Finally, some adjustments were done in the objects drawing and alignment in design view due to the introduction of the Crow's foot notation. Minor changes were done as well in the Manage view in order to enable the CSV buffer sharing between the database manipulation form and the sql execution pane (and vice-versa).

Below, some key change log entries for this version. Consider to read the complete log to take note of all new features, changes and fixes.<br/>

* [New] Added support to crow's foot notation.
* [New] Added the crow's foot notation switch in RelationshipConfigWidget.
* [New] Added the schema arrangement (scattered).
* [New] Added an action to toggle schemas rectangle on ModelWidget.
* [New] CLI now loads the relationship and general settings to reflect relationship styles in export modes.
* [New] Added support to apostrophes in the middle of object's name.
* [Change] Minor improvement on SQLExecutionWidget and DataManipulationForm in order to make possible to paste csv buffer from SQLExecutionWidget to DataManipulationForm.
* [Change] Minor adjustments in the pen width of relationship lines and objects borders.
* [Change] Minor improvement when aligning objects to grid forcing the relationships updating.
* [Change] Improved the performance of (de)selection of several objects at once in ModelWidget and ObjectsScene.
* [Change] Improved the import of index objects.
* [Change] Minor tweak to enable clipboard usage in macOS when copying data in DataManipulationForm.
* [Fix] Fixed a bug in ObjectsScene that was not emitting signals of deselection correctly.
* [Fix] Fixed a bug in SQLToolWidget that was not cleaning up the source code pane when all databases were disconnected.
* [Fix] Fixed a bug that was causing the diff process to try to remove the not null constraint of a primary key.
* [Fix] Fixed a bug that was recognizing the creation of a constraint but wasn't generating the SQL in diff process.
* [Fix] Minor fix in order to avoid the inheritance/dependency descriptor to be rotated to the wrong size when curved lines are being used.
* [Fix] Fixed the generation of sql comments for database and tablespace.
* [Fix] Fix a bug in GeneralConfigWidget that was reverting the grid optins everytime the user applyed settings.
* [Fix] Fixed the genaration of index elements containing expressions.
* [Fix] Fixed the import of operators and operator classes.
* [Fix] Fixed the generation of operator signature by removing the length/precision of the types.
* [Fix] Minor fix in CSVLoadWidget::loadCsvFromBuffer in order to preserve the line breaks avoiding the creation of unecessary lines.
* [Fix] Fixed the import of exclude constraint.
* [Fix] Fixed the import of timestamp(0) type.
