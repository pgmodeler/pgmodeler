v1.1.1
------
<em>Release date: March 14, 2024</em><br/>
<em>Changes since: <strong>v1.1.0</strong></em><br/>

This patch release for pgModeler 1.1.x brings the following improvements and fixes:

* Added support for fixing split database models in the CLI tool on the paid version.
* Added an XML code searching widget in the source code preview dialog.
* Added an option to place objects in random positions imported to a working model.
* Added alerts about unsupported server versions on the database import form and diff form.
* Renamed all features related to "Find" to "Search" for better semantics.
* pgModeler will not change the positions of the original objects in a model when importing new objects to that model.
* Minor layout adjustments in the diff form by moving the export and import options to dedicated group boxes.
* Minor improvement in the code completion feature to avoid repeatedly querying system catalogs having the list already filled. The list is cleared and repopulated only if the cursor position gets back to the initial position when the widget was displayed.
* Fixed the XML code generation for policies.
* Minor adjustment in the enabled status toggling of several widgets in the database import form.
* Fixed the behavior of the "Auto browse" attribute of connections in the database import and diff forms.
* Fixed the horizontal canvas movement when the Shift key is held and the mouse wheel movement is made.
* Fixed a crash in table edit form when handling foreign tables.
* Fixed the partial diff/import filtering which was ignoring a wildcard pattern and listing all objects.
* Minor fix in the SQL code pasting in the command input field of the SQL execution section.
* Fixed a bug when importing collations with the ICU provider.
* Minor workaround in the object rename widget to be displayed with the name input focused but the contents of that field deselected.
* Fixed the relationship conversion feature to preserve the original layer and line color information on the converted objects.
* Minor fix in the code completion feature to properly list column names when using non-schema-qualified table names in INSERT/DELETE commands.
* Fixed a bug in code generation of generic SQL objects.
