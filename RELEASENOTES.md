v0.8.0-beta
------
<em>Codename: <strong>Faithful Elephant</strong></em>
<em>Release date: November 29, 2014</em>
<em>Changes since: <strong>v0.8.0-alpha2</strong></em><br/>
<strong>Summary:</strong> after two months of work this release brings a great set of changes / fixes and the first version of the model database diff feature, being a giant step into the maturity.<br/>
Despite being experimental this feature works nicely in small and simple databases. In large or small but complex ones the process still generates false-positive differences but these issues will be gradually solved in future releases like it was with reverse engineering.<br/>
The graphical objects on the canvas were completely redesigned being more pleasing to the eyes with rounded corners and smooth colors. Also, there was several improvements on dialogs design, where explicit hint texts were replaced by discreet help buttons reducing the size of these windows on screen. Another change was the reformulation of some portions of the main window. Now, pgModeler works in form of views being them: <em>welcome</em>, <em>design</em> and <em>manage</em>.<br/>
Welcome view is where the user can start to work, loading models or creating new ones. The design view is the canvas area, where main work occurs. Finally, and this is considered a new feature, is the separation of SQL tool from the canvas area with the introduction of the manage view, where the user can manage several database instances. In the side of fixes, a lot of bugs (23 in total) were removed during this long development process. The entire list of new features / changes / fixes are quite long (90 entries) and only the most relevant is listed below. The complete change long can be found on the file CHANGELOG.md in binary packages or the source code.<br/>

* [New] Introduced the first version of model database diff feature.
* [New] Enabled the usage of cached catalog queries on Catalog class in order to increase the performance of the whole import process.
* [New] Created a styled text box object.
* [New] Created the new class RoundedRectItem in order to generate rounded corner rectangle items on scene.
* [New] Added a sql disabled info item for relationships.
* [New] Added the ability to copy text from validation widget output.
* [Change] Major changes on sql tool. Now it's possible to manage several database instances on the same server connection.
* [Change] Improvements on DBMS export process. Now the process detects when a object is being created, changed or dropped returning the correct message to the user.
* [Change] Improved the way many-to-many relationships can be configured. Now the generated table can have a single column as primary key or a multi-valued one.
* [Change] Removed the restriction from Catalog and DatabaseImportForm to hide the "postgres" database.
* [Change] Minor fix on fields which accepts expressions to show scroll bars as needed.
* [Change] Changed the order of actions on left control bar at main window.
* [Change] Minor improvement on export to png process. The output image is generated with a margin.
* [Change] Minor change on sample models and asset images.
* [Change] Changed the style of graphical resentation for schemas, views, tables. Now they are drawn with rounded borders.
* [Change] Change the relationship cardinality pattern from (1,n) to 1:n.
* [Change] Minor adjustments on relationships custom points descriptors in order to give better selection and movement.
* [Change] Minor improve when moving a schema object. Relationship points will be moved together.
* [Change] Minor change on relationship editing form, by default random line colors are enabled.
* [Change] Minor fix on temporary models saving process.
* [Change] Explicit hint texts were moved to the instances of the new class HintTextWidget.
* [Change] Minor improvement on database editing form. LC_COLLATE and LC_CTYPE can be freely modified.
* [Change] Minor improvement on primary key constraints. Columns added to them will be marked as not-null by default. This is done to avoid false-positive changes on the model db diff process.
* [Change] Improvements on main window in order to give more visibility to SQL tool and central widget. Now there are three different views (welcome, design and manage).
* [Fix] Fixed a bug when generating XML code for materialized views that was causing these objects to break DTD rules.
* [Fix] Minor fix on code generation of permissions.
* [Fix] Minor fix on permissions editing form.
* [Fix] Fixed a bug on database import process that was crashing the application whenever importing a composite type.
* [Fix] Minor fix on reverse engineering process when importing columns which reference user defined type that are in pg_catalog.
* [Fix] Minor bug fix on database import process that was causing some crashes.
* [Fix] Minor improvements on "move to schema" feature on model widget. The references to moved object are now correctly updated.
* [Fix] Minor fix on SyntaxHighlighter that was not correctly applying the default font to the parent object.
* [Fix] Minor fix on index class and editing form to accept FILLFACTOR no matter the indexing method used.
* [Fix] Minor fix when removing table children objects and restoring them from operations list.
* [Fix] Fixed a crash when reverse engineering a model. The crash was due to trying to handle a not existent graphical object for pg_catalog schema.
* [Fix] Fixed the resizing of schema objects.
* [Fix] Additional fixes for export model to png. Now the correct bounding rect is calculated.
* [Fix] Fixed a bug when exporting model to png that was exporting unnecessary blank areas.
* [Fix] Minor fix on model validation helper to avoid include the database as a referer of other objects.
* [Fix] Fixed a bug when renaming objects and invalidating their references.
