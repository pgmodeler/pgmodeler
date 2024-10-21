v1.2.0-alpha1
------
<em>Release date: October 31, 2024</em><br/>
<em>Changes since: <strong>v1.2.0-alpha</strong></em><br/>

<em><strong>Attention:</strong> Some configuration files were changed in pgModeler 1.2.0-alpha1 causing a break in the backward compatibility with some pgModeler 1.1.x settings. This way, at the first start of the newer version, pgModeler will automatically try to migrate the older settings to the newer ones! Be advised that this is a release of a version in the early stages of development. Bugs, malfunctioning, crashes, and other problems are expected and will be resolved as soon as they are reported. Make sure to make backups of your models and databases before using this version!</em><br/><br/>

It took almost 4 months but here we are glad to deliver to the community pgModeler 1.2.0-alpha1 which bundles 23 new features, 22 improvements, and 14 fixes since the release of the previous alpha. This time, some important decisions were taken to make the code base a bit less complicated to maintain, at the same time some other portions of the tool were refactored to allow the plugin development to be more flexible, and, of course, we are bringing some new features. The key improvements and fixes are described below:<br/>

* **Removed the support for Qt 6.2 and 6.3:** The support for Qt versions 6.2 and 6.3 was removed since it was causing a lot of conditional compilation instructions (the famous C/C++ macros) to be used. This approach tends to make the code hard to read and maintain. So, since Qt 6.2 and 6.3 official upstream support has ended, we decided to make the code compliant with newer framework versions. To be more precise, pgModeler now builds on Qt 6.4.x and above.<br/>

* **Improved plugin API:** The pgModeler's plugin development interface was improved and now allows more portions of the tool to receive user-created features to increase its roll of functionalities. Unfortunately, for now, you still need a basic knowledge of C++ and Qt, maybe in the future the plugin interface can be simplified in such a way to allow non-C++ developers to create their custom features for pgModeler.<br/>

* **Transactional export process:** The export process is now capable of running the commands at once inside a transaction block. This is useful if you want an atomic execution of the generated DDLs, rolling back everything if one command fails in its execution. Note that the transactional option does not affect database and tablespace creation commands, since, by design, these commands need to be executed outside a transaction. <br/>

* **Improved code completion:** The code completion widget now supports the completion of names in ALTER/DROP commands. It even detects the type of objects being modified or dropped suggesting the names filtered by the specified types. <br/>

* **Improved layers setup:** The operation to move objects to certain layers was significantly improved in this version. Now, the quick action "Quick > Set layers" opens a dialog where the user can move the selected objects to one or more existing layers or even create a layer on the fly and assign it to the selection in the design view. Additionally to that improvement, the layers configuration widget (the one that is toggled by the button "Layers" at the bottom of the design view) received an option that makes relationships follow the visibility of the linked tables. This means that if one table is moved to a layer the relationships connected to it will move to that layer too.<br/>

* **Tabbed data handling:** Before this version, data manipulation was performed via a standalone dialog, which could make data handling more difficult due to the number of standalone windows open. Now, all browsed tables are reunited in a single dialog but lying in their own tabs, facilitating data visualization.

* **Query variables plugin:** The paid version of the tool received a new plugin that helps the developers test their queries using variables that are replaced in the command at the moment of its execution. Basically, a query that contains some variables prefixed by $ like this "SELECT $cols FROM $schema.$table WHERE $condition" has the values for each variable replaced and the parsed query executed. The variables and their values can be specified in a special widget that is toggled by the button "Variables" in the SQL execution widget. The main goal of this new feature is to accelerate the query testing mainly if you write parametrized queries based on some ORMs syntaxes. This plugin supports four variable formats: $variable, :variable, @variable, and {variable}.

* **Miscellaneous: **
		* Added support for PostgreSQL 17.
		* Added support for displaying FK's update/delete actions in the data dictionary.
		* The file selection dialog now starts on the user's home by default and saves that last accessed directory, using it the next time it is opened.
		* Minor bug fix in code generation of tablespace, database, and user mapping objects.
		* Minor fix in the object search feature when searching by source/referenced constraint columns.
		* Minor fix in objects' grids to allow sorting the "ID" column as an integer value.
		* Fixed a bug in function editing form that was not resetting "SETOF" flag when changing the return mode to "Table".
		* Fixed a malformed markdown code when a table or view had a comment.
		* Fixed a bug in diff process that was ignoring changes in columns.
		* Fixed a crash in the database import process when destroying detached inherited columns.
		* Fixed a bug in database import that was crashing the application while trying to retrieve user mapping comments.
		* Fixed a bug in the index object that was preventing the removal of included columns.
