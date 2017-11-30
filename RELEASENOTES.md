v0.9.1-alpha1
------

<em>Release date: November 30, 2017</em><br/>
<em>Changes since: <strong>v0.9.1-alpha</strong></em><br/>

<strong>Summary:</strong> this second alpha release brings minor improvements and small fixes being the last version released this year. <br/>

Some importing issues related to PostgreSQL 10 were fixed and now you can use the reverse engineering and diff tool without much problems in newer server versions. <br/>

Attending to some requests, now it's possible to create relationships by right-clicking tables and activating the relationship creation via New > Relationship menu. <br/>

Now pgModeler is able to compare two databases in the diff tool. As known, prior versions would compare a loaded model and a database. In this release, the user can still compare a loaded model against a database but also has the option to select a database to use as the source of the modifications. This can be pretty helpful mainly when the user made changes directly to the database and doesn't want or can't have the model representing these changes and need to apply them directly to another database. <br/>

In the object finder when results contain graphical objects these one can optionally be faded in or out in order to make them visually highlighted facilitating the process of locating them in the canvas.<br/>

Below the log entries for this version. Consider to read the complete CHANGELOG.md to take note of all changes.<br/>

* [New] Added the ability to compare two databases, and not only a model and a database, in diff tool.
* [New] Added the relationship creation buttons on the object overlay when a single table is selected.
* [New] Added the "Relationship" action in "New" submenu on table's popup menu so the user can create relationships using the selected table as source. This avoids the need to use blank areas of the canvas to start creating relationships.
* [New] Improved the data manipulation dialog in such way that when dealing with deletes in tables without PK, tuples with NULL values can be correctly considered.
* [New] Improved the validations on ResultSet class.
* [New] Added a method to indicate if a column value is null in ResultSet.
* [New] Added support to fade in/out objects in object finder in order to highlight the graphical objects retrieved from the search.
* [New] Added an attribute in pgmodeler.conf to store the current status of the "Fade in" button in object finder widget.
* [Change] Minor improvement in the diff generated metadata.
* [Change] Increased the maximum allowed amount of lines in command history.
* [Change] Minor adjustment on diff tool so the connections combo can be correctly updated when the user edit connections from within that form.
* [Change] Improved the progress info of diff process so it can be more accurate.
* [Fix] Fixed the way PostgreSQL 10+ version is returned from Connection::getPgSQLVersion.
* [Fix] Fixed the sequence importing on PostgreSQL 10.
