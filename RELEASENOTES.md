v1.2.0
------
<em>Release date: May 9, 2025</em><br/>
<em>Changes since: <strong>v1.1.0</strong></em><br/>

<em><strong>Attention:</strong> pgModeler 1.2.0 introduces configuration changes that may affect compatibility with 1.1.x settings. On the first launch, pgModeler will automatically attempt to migrate your existing settings. Please note that this is an early development release where stability issues may occur. It's recommended to back up all models and databases before use. Please report any found bugs for prompt resolution. The mentioned migration process helps transition to improved configurations while minimizing disruption to your workflow.</em><br/><br/>

<strong>Summary:</strong> After one year and three months, 72 new features, 93 changes/improvements, and 74 bug fixes were delivered to this new major release. The result of this hard work is a tool even more optimized, with features that will make the difference in any aspect of database modeling and administration! Below, we have the compilation of key new features, changes, and fixes implemented since pgModeler 1.1.0. Of course, some more technical changes were omitted but you can see everything in the file CHANGELOG.md. <br/><br/>

**New features** <br/>

* Improved debug mode: The debugging capabilities have been expanded with dedicated output tabs in the database import and diff forms. Now, everytime the user checks the debug mode option in any of the mentioned forms, a new tab will be displayed and any useful information that helps to track down errors/bugs will be logged there.

* Multiple objects source preview: To make the code preview more flexible, the user can now visualize the source code of multiple selected objects in the canvas or the database objects tree. When using this feature, the code in the source code preview widget will be displayed in the order in which the objects are created. This feature is useful when you need the code of a small portion of the model and don't want to generate the whole code just to extract the desired parts of it.

* Relationships' FK columns indexes: Now, the relationships that automatically create foreign key columns, can also create indexes over that columns too. This can speed up a bit more the modeling process. The generated indexes, like relationships, are named after the specific pattern and the user has total control over the generated names in the relationship editing form or by defining a global name pattern under relationships settings.

* Improved layers setup: The operation to move objects to certain layers was significantly improved in this version. Now, the quick action "Quick > Set layers" opens a dialog where the user can move the selected objects to one or more existing layers or even create a layer on the fly and assign it to the selection in the design view. Additionally to that improvement, the layers configuration widget (the one that is toggled by the button "Layers" at the bottom of the design view) received an option that makes relationships follow the visibility of the linked tables. This means that if one table is moved to a layer the relationships connected to it will move to that layer too.

* Transactional export process: The export process is now capable of running the commands at once inside a transaction block. This is useful if you want an atomic execution of the generated DDLs, rolling back everything if one command fails in its execution. Note that the transactional option does not affect database and tablespace creation commands, since, by design, these commands need to be executed outside a transaction. The command-line interface tool also received support for the transactional export process and it's enabled by default like in the GUI. To deactivate it during the execution of any export operation use the options -nt or --non-transactional.

* Improved code completion: The code completion widget now supports the completion of names in ALTER/DROP commands. It even detects the type of objects being modified or dropped suggesting the names filtered by the specified types. Another enhancement done is the capability of displaying column names on ORDER BY clause.

* Tabbed data handling: Before this version, data manipulation was performed via a standalone dialog, which could make data handling more difficult due to the number of standalone windows open. Now, all browsed tables are reunited in a single dialog but lying in their own tabs, facilitating data visualization.

* Improved schema microlanguage: The pgModeler's code templating language (aka schema microlanguage) was improved in such a way as to support escaped character sequences that refer to metacharacter tokens. The supported escaped characters (and their related metacharacters) are \s ($sp), \t ($tb), \n ($br), [ ($ob), ] ($cb), { ($oc), } ($cc), $ ($ds), # ($hs), % ($ps), @ ($at), & ($am), \ ($bs) and * ($ds). The schema language now also supports the "include" statements that inject portions of code stored in other files into the currently parsed schema file. This is pretty handy for avoiding code duplication and facilitating the maintenance of schema files.

* Markdown data dictionaries: pgModeler now supports the generation of data dictionaries in Markdown (.md) format in the model export form. This is useful to integrate data dictionaries generated by the tool with other documentation tools that use that file format to keep documentation pages. The pgmodeler-cli tool also received support for Markdown dictionaries through the option "--markdown".

* Improved older configs copy: From now on, in the first run, pgModeler will try to copy the configuration files from a previous major version immediately before the current one. For example, running 1.2 the files to be copied will be from 1.1 and not from 0.9.x anymore. This will increase the chances of reusing settings from previous versions diminishing the annoying situation of reconfiguring the tool every time it is updated.

* Improved code generation: The DDL generation for objects that support "CREATE OR REPLACE" was updated to include the "OR REPLACE" portion, this is the case for functions, procedures, views, and others. The diff feature received the option "Replace modified objects" which causes objects to be replaced via "CREATE OR REPLACE" instead of being dropped and created again.

* Quick create constraints, indexes, and relationships: This feature, introduced through an exclusive plugin in pgModeler Plus, allows the creation of constraints, indexes, and relationships based on the objects selected in the design view, without the need to open a single editing form or fill lots of fields. The objects created also support name patterns, and they can be configured in the plugin's settings menu in the main window.

* Query variables plugin: pgModeler Plus received a new plugin that helps the developers test their queries using variables that are replaced in the command at the moment of its execution. Basically, a query that contains some variables prefixed by $ like this "SELECT $cols FROM $schema.$table WHERE $condition" has the values for each variable replaced and the parsed query executed. The variables and their values can be specified in a special widget that is toggled by the button "Variables" in the SQL execution widget. The main goal of this new feature is to accelerate the query testing mainly if you write parametrized queries based on some ORMs syntaxes. This plugin supports four variable formats: $variable, :variable, @ variable, and {variable}.

* Improved plugin API: The pgModeler's plugin development interface was improved and now allows more portions of the tool to receive user-created features to increase its roll of functionalities. Unfortunately, for now, you still need a basic knowledge of C++ and Qt. But for future releases, I plan to simplify even more the plugin interface in such a way as to allow non-C++ developers to create their custom features for pgModeler.

* Removal of the support for Qt 6.2 and 6.3: The support for Qt versions 6.2 and 6.3 was removed since it was causing a lot of conditional compilation instructions (the famous C/C++ macros) to be used. This approach tends to make the code hard to read and maintain. So, since Qt 6.2 and 6.3 official upstream support has ended, we decided to make the code compliant with newer framework versions. To be more precise, pgModeler now builds on Qt 6.4.x and above.

**Miscellaneous** <br/>

* Added support for PostgreSQL 17.
* Improved the reverse engineering so columns can also be imported using the option "Import to the working model" in database import form.
* Graphical objects added to the in canvas area will now blink and the viewport will center on them to indicate to the user where they were put.
* Adjusted pgmodeler-cli to raise errors when the ignore export error options are used in transactional export mode.
* Minor adjustment in diff form by making transactional mode mutually exclusive with ignore export errors options.
* Minor fix in the file selector widget that was not applying correctly the palette colors in the line edit field.
* Minor fix in extensions creation/loading processes by ignoring duplicated schemas and use the ones already available in the model.
* Fixed a bug in the data dictionary generation for views.
* Fixed a bug in the SQL generation of database model SQL related to an unknown database attribute.
* Fixed a bug when importing composite types having attributes using arrays of other user-defined types.
* Fixed a bug when displaying the source, in Database Explorer, of a composite type that has one or more attributes referencing used-defined types.
* Fixed a bug that was generating errors when running catalog queries of some objects in older versions of PostgreSQL.
* The model objects widget now accepts an Alt + click over a graphical object, highlighting it in the design view.
* Redundant search/replace instances all over the tool were removed due to the integrated search/replace widget in the source code editor.
* The extension editing form now properly supports custom schema names in data types.
* The database import process and the database model itself have been refactored for more reliable handling of extension-owned objects using the new extension object structure.
* The comparison operations in the diff process have been optimized to filter system objects correctly.
* The function behavior type was simplified by dropping the STRICT type since it has the same semantics as RETURNS NULL ON NULL INPUT.
* The database import form, when in debug mode, will remain open so the user can inspect the commands and objects created during the process.
* Fixed some crashes during diff operations on extension-created tables.
* Fixed the importing of columns using arrays of user-defined types.
* Fixed the time zone persistence on the timestamp data type.
* Removed false-positive diffs for functions with comments or STRICT behavior.
* Fixed reverse engineering of uppercase type names.
* When the export, import, and diff processes finish, the taskbar blinks when the window is not visible.
* Minor adjustment in the layers configuration widget to accept Enter/Return to apply settings.
* Fixed some shortcut conflicts in the main window.
* Minor fix in the "Open relationship" action in the design view.
* Fixed the index catalog queries when using pgModeler in compatibility mode (PG 9.x).
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
* The restriction of specifying OUT parameters in procedures was removed in compliance with newer PostgreSQL versions.
* Added support for the options check_option, security_invoker, and security_barrier in views.
* The syntax highlighting feature was completely refactored being now more precise and the configuration files more simple.
* Minor fix in pgmodeler-cli in such a way as to warn about invalid changelog entries avoiding aborting the entire model fix process.
* Fixed a bug in the reverse engineering feature that was not retrieving comments of sequences causing false positives to be generated in the diff process.
