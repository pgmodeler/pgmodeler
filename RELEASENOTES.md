v0.9.0-alpha1
------

<em>Release date: February 06, 2017</em><br/>
<em>Changes since: <strong>v0.9.0-alpha</strong></em><br/>

<strong>Summary:</strong> this new alpha release of the major version 0.9.0 brings some design improvements that were requested long time ago and only now they could be implemented.<br/>

One feature introduced is the object fade which can be used to isolate certain objects in the model to improve the visualization. This feature can be activated via "Fade in/out" menu item by right clicking the canvas area or using the same action in the main window's side bar. There is an special behavior that applies to tag objects: if you have one or more tables and views tagged you can fade in or out them by the associated tag. To do so just right click the desired tag in the model objects view and choose the available fade options. The fade works for all graphical objects and can be applied for each object type separately or to the current object selection in the canvas. The default fade factor is 10% and may vary from 0% to 70%, which can be adjusted in the general settings. <br/>

Another feature brought after some requests is the column duplication. The original request was to make possible the duplication of columns when editing tables. But we gone further, the user can duplicate almost all kind of objects from columns and tables to functions and aggregates. In order to use this feature right click any object in the canvas or in the model objects view and select 'Duplicate'. This feature is still experimental and will be adjusted in future releases if needed. <br/>

There were other minor features, fixes and improvements just to make pgModeler even better. Below, some highlighted features and fixes, the complete log is available in CHANGELOG.md:<br/>

* [New] Added support to object fading in ModelWidget.
* [New] Added the support to persist the object opacity factor in config file.
* [New] Added support to column, constraint, trigger, rule and index duplication in TableWidget.
* [New] Added support to item duplication in ObjectTableWidget.
* [New] The database explorer now creates the root item in the tree as the server itself which contains data related to this latter.
* [New] Added the support to parenthesis in the middle of objects' names.
* [Change] Improved the PgSQLTypeWidget to enable the length, precision, dimension fields as the user types the desired datatype. This will avoid jumping to the wrong field when pressing tab.
* [Change] Minor tweaks done in order to minimize the diff detection related to default values of columns.
* [Change] Dropped the automatic LC_COLLATE and LC_CTYPE generation in Database object. Since this was causing more problems than helping when import the database and validating/exporting it.
* [Change] In DataManipulationForm the filter input field is automatically focused when the filter toggle button is activated.
* [Fix] Fixed a bug when using diff to create columns and update constraints.
* [Fix] Fixed a problem when importing database that contains citext extension installed in pg_catalog.
* [Fix] Minor fix in ConnectionsConfigWidget that was causing duplicated connections to share de same host info wrongly.
* [Fix] Fixed a crash when restoring objects' metadata from backup file.
* [Fix] Fixed a bug that was preventing inheritance relationships to be created when the same pair of tables existed in different schemas.
* [Fix] Fixed a bug that was causing column name patterns to be used wrongly in many-to-many relationships.
* [Fix] Fixed a bug that was causing the duplication of permissions during the database import which was leading to the complete failure of the entire process.
* [Fix] Fixed the problem with invalid type error when trying to edit a 'timestamp with timezone' column.
* [Fix] Fixed a bug in Relationship that was not setting NOT NULL flag for columns of the multi-valued primary key of many-to-many relationships causing the diff process to fail in some specific cases.
