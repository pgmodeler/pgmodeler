v1.0.0-beta
------

<em>Release date: September 10, 2022</em><br/>
<em>Changes since: <strong>v1.0.0-alpha1</strong></em><br/>

<strong>Attention:</strong> pgModeler 0.9.x settings are not compatible with pgModeler 1.0.0-beta. This way, in the first start of the newer version, some settings will be automatically migrated if the tool detects the configuration files related to 0.9.4 or below in the system! <br/>

<strong>Summary:</strong> following the path to the stable release, today I present to you pgModeler 1.0.0-beta. The main focus of this release is to remove support for PostgreSQL 9.x which has not been maintained for over a year. By dropping support for an obsolete version of PostgreSQL, we can spend more energy on introducing new functionality that newer versions of the RDBMS bring rather than trying to maintain older versions which creates unnecessary complexity in the pgModeler code. Therefore, if you still need to work with PostgreSQL 9.x, you will need pgModeler 1.0.0-alpha1 or lower. <br/>

This release also brings minor improvements in several parts of the tool. Now, the data dictionary displays information related to indexes, triggers, and sequences used in the tables, making the generated file much more complete from the documental point of view. <br/>

Another improvement was the ability to generate SQL code split in the command line interface which allows generating SQL by objects in separate files. Furthermore, it is now possible to add to the generated SQL files the code of objects that are dependencies or children of the original object. <br/>

In the model design area, the magnifier tool has improved to allow better object interaction. Unlike previous versions, this tool now allows interaction with multiple objects which helps to handle large models where the visualization is impaired and the magnifier tool comes as a good way to handle objects. <br/>

As part of the continuous enhancement in the UI and usability, navigating through form fields using shortcuts and tab keys has been improved to be much more consistent and intuitive compared to previous versions. It's still not in the best shape but for now, it will help users who prefer the keyboard over the mouse when configuring database objects. This improvement was also applied to almost all other widgets and forms in the tool. <br/>

Finally, as usual, some bugs and crashes were fixed improving the stability of the tool. The following are some changelog entries for that version. For more details, please read the CHANGELOG.md file. <br/>

* [New] Added data dictionary entry for indexes, triggers, and sequences.
* [New] Columns, indexes, triggers, and constraints now have their own method to generate their data dictionaries.
* [New] Created the objects.sch to be reused when generating data dictionaries for columns, constraints, indexes, triggers, and sequences.
* [New] Added split SQL code generation modes to export form.
* [New] Added the ability to export dependencies or children SQL in split mode at CLI.
* [New] Added code generation options to DatabaseModel::saveSplitSQLDefinition.
* [New] Added an info message in the FindReplaceWidget reporting the search status (not found, cycle search, replacements made, etc).
* [New] Added a text search widget in SourceCodeWidget.
* [Change] Tab order adjustments in several forms and widget.
* [Change] pgModeler CLI menu and messages fixed/improved.
* [Change] Improved the interaction between the magnifier tool and the canvas area. Now the user can select multiple objects, move, and control the zoom with the magnifier tool activated.
* [Change] Adjusted the size of the magnifier frame according to the primary screen size.
* [Change] Improved the XMlParser exceptions raised to point the file that generated the error.
* [Change] Dropped the support for PostgreSQL 9.x.
* [Change] Connection class now aborts the connection to PostgreSQL 9.x servers.
* [Change] Changed the shortcut key of the magnifier tool from Ctrl + Alt to F9.
* [Change] Changed the default size of splash screen on screens with device pixel ratio = 1.
* [Fix] Fix a crash in macOS when right-clicking a blank portion of the canvas when there are objects selected.
* [Fix] Fixed a bug in BaseObject::isValidName that was not considered valid a name in the format schema."object".
* [Fix] Fixed the diff generation for materialized views.
* [Fix] Fixed a bug in CLI that was not correctly fixing domains in models created in 0.8.2.
* [Fix] Minor fix in standalone connection dialog to alert the user about unsaved connection configuration.
* [Fix] Minor fix in ConnectionsConfigWidget to avoid adding connections with duplicated aliases.
