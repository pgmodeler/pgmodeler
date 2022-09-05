v1.0.0-beta
------

<em>Release date: September 9, 2022</em><br/>
<em>Changes since: <strong>v1.0.0-alpha1</strong></em><br/>

<strong>Attention:</strong> pgModeler 0.9.x settings are not compatible with pgModeler 1.0.0-alpha1. This way, in the first start of the newer version, some settings will be automatically migrated if the tool detects the configuration files related to 0.9.4 or below in the system! <br/>

<strong>Summary:</strong> <br/>


* [New] Added data dictionary entry for indexes, triggers, and sequences.
* [New] Columns, indexes, triggers, and constraints now have their own method to generate their data dictionaries.
* [New] Created the objects.sch to be reused when generating data dictionaries for columns, constraints, indexes, triggers, and sequences.
* [New] Added split SQL code generation modes to export form.
* [New] Added the ability to export dependencies or children SQL in split mode at CLI.
* [New] Added code generation options to DatabaseModel::saveSplitSQLDefinition.
* [New] Added a fix step in CLI to remove encrypted attribute from <role> tag.
* [New] Added an info message in the FindReplaceWidget reporting the search status (not found, cycle search, replacements made, etc).
* [New] Added a text search widget in SourceCodeWidget.
* [Change] Tab order adjustments in several forms and widget
* [Change] Adjusted the stylesheet of the data dictionary.
* [Change] pgModeler CLI menu and messages fixed/improved.
* [Change] Improved the interaction between the magnifier tool and the canvas area. Now the user can select multiple objects, move, and control the zoom with the magnifier tool activated.
* [Change] Adjusted the size of the magnifier frame according to the primary screen size.
* [Change] Improved the XMlParser exceptions raised to point to the file that generated the error.
* [Change] Dropped the support for PostgreSQL 9.x.
* [Change] Dropped the attribute "Encrypted" from the role form.
* [Change] Connection class now aborts the connection to PostgreSQL 9.x servers.
* [Change] Changed the shortcut key of the magnifier tool from Ctrl + Alt to F9.
* [Change] Changed the "edit item" icon on ObjectsTableWidget.
* [Change] Changed the default size of splash screen on screens with device pixel ratio = 1.
* [Fix] Fix a crash in macOS when right-clicking a blank portion of the canvas when there are objects selected.
* [Fix] Minor fix the warning icon size and position in FileSelectorWidget.
* [Fix] Fixed a bug in BaseObject::isValidName that was not considered valid a  name in the format schema."object".
* [Fix] Fixed the diff generation for materialized views.
* [Fix] Fixed a bug in CLI that was not correctly fixing domains in models created in 0.8.2.
* [Fix] Fixed the appearance of the about widget in pgmodeler-se.
* [Fix] Fixed the CLI in such a way to use QRegularExpression correctly on large text buffers.
* [Fix] Minor fix in standalone connection dialog to alert used about unsaved connection.
* [Fix] Minor fix in ConnectionsConfigWidget to avoid adding connections with duplicated aliases.
