v0.9.0-alpha1
------

<em>Release date: February 07, 2017</em><br/>
<em>Changes since: <strong>v0.9.0-alpha</strong></em><br/>

<strong>Summary:</strong> this new alpha release of the major version 0.9.0 brings some design improvements that were requested long time ago and only now they could be implemented.<br/>

One feature introduced is the object fade which can be used to isolate certain objects in the model to improve the visualization. This feature can be activated via "Fade in/out" menu item by right clicking the canvas area or using the same action in the main window's side bar. There is an special behavior that applies to tag objects: if you have one or more tables and views tagged you can fade in or out them by the associated tag. To do so just right click the desired tag in the model objects view and choose the available fade options. The fade works for all graphical objects and can be applied for each object type separately or to the current object selection in the canvas. The default fade factor is 10% and may vary from 0% to 70%, which can be adjusted in the general settings. <br/>

Another feature brought after some requests is the column duplication. The original request was to make possible the duplication of columns when editing tables. But we gone further, the user can duplicate almost all kind of objects from columns and tables to functions and aggregates. In order to use this feature right click any object in the canvas or in the model objects view and select 'Duplicate'. This feature is still experimental and will be adjusted in future releases if needed. <br/>

The long awaited improved primary key creation is implemented. This feature greatly reduces the time used to create a primary key in a table. Now, the user just needs to create the desired columns in the table's editing form and use the checkbox in the sectoin 'PK' to mark the desired columns as primary key. The drawback of this feature is that an automatic name is assigned to the generated primary key which can cause conflicts with other objects in the model demanding validation to fix any problem and the advanced attributes of the constraint can't be handled thus default values are used. Fortunately, once created, the primary key will be available in the tab 'Constaints' so the user can do the fine tuning if needed.<br/>

Finally, the SQL command history is now persisted in a configuration file called 'sql-history.conf' and kept in the current user's configuration storage. The commands in the history are saved per database connection. There is a limit of commands that can be saved in the file and this attribute can be adjusted in the general settings. There is also a context menu in the SQL history which enables the user to reload, save or clear the history for the current connection.<br/>

There were other minor features, fixes and improvements just to make pgModeler even better (some highlighted features below). The complete change log is available in the file CHANGELOG.md:<br/>

* [New] Added support to easily create primary keys just by checking the desired columns in table's editing form.
* [New] Added support to use middle button to handle panning mode.
* [New] Added a more user friendly message at startup whenever a missing or corrupted configuration file is detected. The user is now presented to an option to restore default settings for the problematic file.
* [New] Now any default file restored in ConfigurationForm has a backup saved into the directory 'backups' inside the configuration storage.
* [New] Added support to object fading in ModelWidget.
* [New] Added the support to persist the object opacity factor in config file.
* [New] Added support to column, constraint, trigger, rule and index duplication in TableWidget.
* [New] Added support to item duplication in ObjectTableWidget.
* [New] The database explorer now creates the root item in the tree as the server itself which contains data related to this latter.
* [New] Added the support to parenthesis in the middle of objects' names.
* [Change] Improvements done in the SQL history at SQL execution widget. Now the command history is saved into a specific file and restored when the application starts.
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
