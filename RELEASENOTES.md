v0.8.0-alpha
------
<em>Codename: <strong>Faithful Elephant</strong></em><br/>
<em>Release date: July 21, 2014</em><br/>
<em>Changes since <strong>v0.7.2</strong></em><br/>

* [New] Added support to using global settings for relationships on the editing form of those objects.
* [New] A new section was created on settings dialog to manage global configurations for relationships.
* [New] Enabled the movement of schema objects without the need to select their children. This operation does not applies to protected or system schemas like public, pg_catalog.
* [New] Created a more elaborated central widget with some basic operations like create, load, load recent models and restore previous session.
* [New] Reletionships 1:1, 1:n and fk now supports a new style of link between tables. The foreign key columns will be directly linked to primary key columns.
* [New] Created a class called ColorPickerWidget to handle operations where the user need to configure colors for objects.
* [New] Relationship objects now supports custom line colors.
* [New] Added the static methods setPgSQLVersion and getPgSQLVersion on BaseObject class in order to override the PostgreSQL version used to generate code for all instances of that class.
* [New] Introduced a method DatabaseModel::getXMLParser() in order to return the xmlparser of the model to permit user create new objects from xml code within the current database model.
* [Change] Major change on main UI style in order to make a cleaner interface and gain more space on canvas to handle models.
* [Change] Minor model objects style refreshment.
* [Change] Minor optimization on class AppearanceConfigWidget by adding color pickers in order to select color of the elements.
* [Change] A significant code optimization was made on TagWidget by changing the tool buttons that was handling color configuration by color pickers.
* [Change] Added a color picker on RelationshipWidget to permit the configuration of custom line color.
* [Change] Minor layout adjustment on update notifier widget.
* [Change] Important change on SchemaParser class which is not a singleton anymore in order to minimize crashes due to race conditions on running threads.
* [Change] Important change on XMLParser class which is not a singleton anymore in order to minimize crashes due to race conditions on running threads.
* [Fix] Fixed a bug related to partial command execution on SQL tool.
* [Fix] Minor fix on model navigation widget when closing a model.
* [Fix] Fixed a bug that was crashing pgModeler when switching models with the overview widget opened.
* [Fix] Fixed a bug that was eventually making pgModeler enter on a endless looping when calculating the chained operations length.
* [Fix] Fixed a regression related to special objects recreation after rename a primary key column. The problem was that the relationship was wrongly storing the old name of the columns at connection time.
