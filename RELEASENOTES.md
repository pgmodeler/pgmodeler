v0.8.2-beta1
------

<em>Codename: <strong>Faithful Elephant</strong></em><br/>
<em>Release date: March 31, 2016</em><br/>
<em>Changes since: <strong>v0.8.2-beta</strong></em><br/>

<strong>Summary:</strong> we are close to the end of series 0.8.x since this is the last beta release for the 0.8.2. This version took a bit longer than the previous one but brings lots of fixes and some cool new features.<br/>

Starting from new features, we have an experimental ability to diff partial models meaning that you can generate SQL commands by comparing a small set of the model (imported) against the full database. In this case missing objects in the model will not generate DROP commands to be applied to the database.<br/>

Another experimental feature related to the data modeling is the database objects metadata handling. In pgModeler, we call metadata a volatile attribute or object that exists only during design time. Examples of metadata are: tag and textbox objects, object's positioning, SQL disabling and protection statuses, current zoom and canvas positions. These objects are lost when you export the model to the database server remaining only in the model file. This feature can be useful if you are adding objects directly to the database without update the source model. In order to keep the model synchronized with the database, you can import the latter generating an up-to-date model, extract a metadata file from the old model and apply it to the new model in order to recover almost all info about objects' positioning and other attributes. This is a small workaround to the missing database -> model diff (remember that we have only the model -> database diff) to be implemented in the future. <br/>

In database management section, the objects' tree is now loaded on demand meaning that only selected objects in the tree and some of its children will be retrieved from the database. Still in the manage view, the user can now visualize object's source code by middle-clicking an item or hitting F7. Both features are still experimental and may be improved until the final release.<br/>

Several performance improvements in design view were done in order to provide a better responsiveness when handling huge models which contain a lot of relationships by using placeholders when moving tables or views avoiding the excessive update of relationship objects.<br/>

Finally, key bug fixes and changes are described below. The full list of news, changes and fixes is available in the CHANGELOG.md. <br/>

* [Fix] Fixed a bug in import process that was not properly creating dependency objects when auto resolve deps was checked.
* [Fix] Fixed the import of triggers, index, rules to automatically create their parent table if they are not yet created (when using auto resolve deps).
* [Fix] Fixed the OID filtering in DatabaseExplorerWidget.
* [Fix] Fixed the drop command generation for extension objects.
* [Fix] Fixed a bug that was not updating relationships when importing objects to the current database model.
* [Fix] Fixed a bug when initializing PgSQLType instances when the provided type name is a user defined one. Random precision/dimension is not created anymore.
* [Fix] Fixed the object search when using exact match option.
* [Fix] Fixed the diff process that was not processing 1-1 or 1-n relationships correctly.
* [Fix] Fixed a bug when pressing ESC key in the middle of object's movement that was canceling it.
* [Fix] Fixed a bug when selecting a protected object using right button. The parent object is not selected incorrectly anymore.
* [Fix] Fixed a bug when importing operator classes that was generating incorrect XML code for this kind of object prior the creation in the output model.
* [Fix] Fixed a bug when importing functions with unamed parameters.
* [Fix] Fixed some leaks and crashes when canceling the creation of new tables, views or relationship from their editing forms and closing the application.
* [Fix] Fixed a bug related to quoted name validation that was wrongly raising errors related to long names. The validation of name size now discards the quotes from the count.


