v0.9.2-alpha1
------

<em>Release date: December 17, 2018</em><br/>
<em>Changes since: <strong>v0.9.2-alpha</strong></em><br/>

<strong>Summary:</strong> finally, after four months, we have a new version out of the oven! This one has A LOT of changes that range from several code refactoring and performance improvements to the introduction of new interesting features like table partitioning and canvas layers.<br/>

First, let's talk about the changes and improvements. The objects drawing operations received a good set of patches making it a bit faster and less memory consuming. Putting these improvements in numbers, we have reduced in ~40% the internal size of a database model which means less objects to keep in memory as well to be drawn giving a good performance gain. You can see more details here: https://bit.ly/2Q3XMT5<br/>

The overall performance of the reverse engineering was improved which, in consequence, have enhanced the diff process performance as well. The diff process received some fixes and changes and now it is generating less false-positive changes. <br/>

In the new features side, we added scene layers which goals is to introduce a new level of visual segmentation of graphical objects. Another new feature created is the support to view columns that are deduced from the relationships between these objects and the tables. <br/>

Still in the design view in order to be more close to PostgreSQL 10+ new features pgModeler is now capable of handling declarative table partitioning (this includes code generation, reverse engineering and models comparison). Also, tables and views can have their attributes paginated or even collapsed to minimize the area occupied by these objects making big objects easier to visualize. <br/>

Now, about the bug fixes, several crashes were eliminated making the tool more stable for different usages, the reverse engineering received some patches so it can import correctly user-defined type in form of arrays and many others <br/>

Finally, some of the key changes of this release are listed below. For the complete list of changes, fixes and new feautures, please, read the CHANGELOG.md. <br/>

* [New] Added support to scene layers.
* [New] Added support to view's columns importing in DatabaseImportHelper. 
* [New] Added the ability to load view columns from database model file in DatabaseModel::createView.
* [New] Added a tab "Columns" in ReferenceWidget where the user will be able to insert columns to be used as view columns.
* [New] Added support to pagination of tables and views columns pagination.
* [New] Added a fix step on CLI to remove the deprecated attribute hide-ext-attribs from tables and views xml code.
* [New] Added a configuration option to control attributes per pages in tables and views.
* [New] Added support to save collapsing states and current attributes pages to the database model file.
* [New] Added support to OLD/NEW tables aliases on triggers.
* [New] Added support for partition attaching/detaching detection in diff process.
* [New] Added support to importing partitioned/partition tables on DatabaseImportHelper.
* [New] Added support to resize grid cells to fit contents on ObjectsTableWidget.
* [New] Added support to hide columns on data manipulation dialog.
* [Change] Minor fixes in OperationList in order to force views to be updated correctly when operating over a table which is referenced by those objects.
* [Change] Minor adjustments on SchemaView and BaseTableView (and its children classes) to update the geometry when they switch from invisble to visible state.
* [Change] Improved the update of views when referenced columns and tables change their structure.
* [Change] Improved database model loading times by avoiding the rendering of tables while the children objects (indexes, trigger, rules, etc) are being added.
* [Change] The zoom in/out level is now sensible on how much the user rolls the mouse wheel.
* [Change] Code refactoring done in order to make it more close to C++14 in order to take advantage of new features introduced by that standard.
* [Change] Improved the diff process in such way to avoid generating unnecessary/noise commands related to changing types of columns to integer and setting nextval() call as default value.
* [Change] Move the FK settings, copy options and name patterns group boxes to a dedicated tab on RelationshipWidget.
* [Change] Improved the models destruction when closing the application.
* [Change] Improved the Index/Exclude/ParitionKey elements handling by creating a generic form/grid that handles these kinds of objects (ElementsTableWidget).
* [Change] Modified the RelationshipWidget in order to handle partitioning relationships.
* [Change] Modified RelationshipConfigWidget in order to write name partterns for partitioning relationships.
* [Change] Improved the reverse engineering performance by avoiding update relationships as they are being imported.
* [Change] Improved the object duplication feature in ModelWidget.
* [Change] When the model is loaded it is copied to the temporary models storage as a first version of the temporary dbm file.
* [Change] pgModeler will now use the official docs url in the help buttons.
* [Fix] Fixed a crash while renaming view's children objects.
* [Fix] Fixed the rendering of views which contain only a single reference that is the whole object's defintion.
* [Fix] Fixed the column name deduction for recursive views.
* [Fix] Fixed a bug that was causing crashes while configure new constraints on tables.
* [Fix] Fixed a regression in schema's rectangle selection.
* [Fix] Fixed an artifact when user switched on and off the compact view.
* [Fix] Fixed a bug in DatabaseModel::destroyObjects that could lead to segfault when the destroyed model had permissions on it.
* [Fix] Fixed a bug when importing columns which data types is some user defined type in form of array, e.g., custom_type[].
* [Fix] Fixed a bug that was causing special primary key configured on a relationship to make the original primary key of the table to disappear after disconnect the relationship. Now pgModeler stores in memory the original PK prior the connection of relationship and creation of the special PK. When disconnected the relationship, the original primary key is restored on its parent table.
* [Fix] Fixed the class Relationship to reuse compatible columns when handling partitioning relationships.
* [Fix] Fixed the diff process in such way to create new columns with their respective COMMENT ON statement when present.
* [Fix] Fixed the detection of comment changes for columns on diff process.
* [Fix] Making pgModeler honor the columns arrangement in primary keys.
* [Fix] Fixed a bug that was causing FK relationship deletion to crash the application sometimes.
* [Fix] Some fixes were done in the ModelOverviewWidget in order to support large models without exceed the screen size when configuring the size of the overall widget.
