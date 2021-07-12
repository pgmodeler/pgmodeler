v0.9.4-alpha1
------

<em>Release date: July 19, 2021</em><br/>
<em>Changes since: <strong>v0.9.4-alpha</strong></em><br/>

<strong>Attention:</strong> the database model file structure has changed since the last stable 0.9.3. Models created in older releases will certainly fail to load due to incompatibilities because some attributes in the XML code don't exist anymore or have changed during the development of 0.9.4-alpha1. Before loading your database models in this new release, please, make sure to have a backup of them all and follow the steps presented by the model fix tool to patch the documents' structure. Not paying attention to this situation may cause irreversible data loss! If the fix procedures aren't enough to make your database models loadable again, please, ask for help at the official support channels!<br/>

<strong>Summary:</strong> after two months of intense work, we finally made it! The last alpha release for 0.9.4 is ready bringing some important fixes, changes, and interesting new features which I will briefly describe further.<br/>

The first new feature in 0.9.4-alpha1 is the ability of export the database model in split SQL scripts. In this mode, pgModeler will create a single SQL file for each database object in the model. This is useful if you need to deploy your database in a custom way differently from that made by the tool. This feature is avaliable in GUI as well as in CLI.<br/>

Due to the introduction of a new syntax highlighting configuration file (for code snippets configuration), pgModeler will, from now on, create missing configuration files at startup. This will avoid breaking the tool's execution when it detects the absence of any configuration file. <br/>

pgModeler 0.9.4-alpha1 also brings the ability to drop databases quickly from the databases listing in SQL tool. Previously, there was the need to browse the database first and then drop it. <br/>

This release also introduces a new auxiliary tool which is called pgModeler Schema Editor (pgmodeler-se). In this tool, it's possible to edit and validate *.sch files as well as tweak syntax highlighting configuration files. You can use this tool to modify the SQL generation template files, for instance, so the generated syntax fits your needs. <br/>

In data manipulation form, the sorting operation performed when clicking columns was adjusted. Now, the sorting will be triggered by holding the control key and clicking the desired column, and if the user only clicks a column without holding the control key will cause the entire column to be selected in the grid. </br>

The command-line interface was patched in such a way that now doesn't chash anymore when performing a diff operation in which a database model is used as input. Another improvement to CLI is the introduction of the option --force in the mime type handling operation.<br/>

The data type configuration widget received a simple improvement which adds a significant reduction in the time spent to configure a column data type for instance. Now, the user can just type the name of the desired data type to be used instead of selecting it by using the mouse. After typing the data type name just hit tab key to jump to the next field and the data type will be automatically configured. <br/>

In preparation for pgModeler 1.0, a lot of changes were made in the files structure. The libraries subprojects were moved to the folder "libs", all the executables subprojects move to "apps" folder, and all the assets that are deployed with libraries and executables were moved to "assets" folder. Additionally to that reorganization, some libraries were renamed in order to give a better semantics.<br/>

Finally, the whole set of changes of this release has 76 entries being 21 new features, 34 changes/improvements, and 21 bug fixes. Below, some key changes are highlighted. For the complete list of changes, please, take a look at the CHANGELOG.md. <br/>

* [New] The bulk editing form now can be confirmed by Ctrl+Return shortcut.
* [New] The CLI now is capable of fixing models created before 0.9.4-alpha1 that have columns, functions and other objects that reference extension types with simple names instead of schema-qualified names.
* [New] Added an option to GeneralConfigWidget to toggle the displaying of schema names of columns data types on design view. This option affects only the rendering of objects, the code generation will keep displaying user-defined type names a in schema-qualified way.
* [New] Added the ability to drop databases from the database listing in SQLToolWidget.
* [New] Created an auxiliary tool called pgModeler Schema Editor in order to help to edit *.sch files as well as validate their syntax.
* [New] GeneralConfigWidget now restores the sch-highlight.conf to default.
* [New] Added the enviroment variable name for schema editor app in GlobalAttributes.
* [New] Added options to create only missing config files or recreate all config file in CLI.
* [New] Added support for saving SQL definition in split files.
* [New] Added support for split SQL generation in CLI.
* [New] Added the ability to cancel the SQL file export process in GUI.
* [New] Added new metacharacter to SchemaParser: $hs for #, $ps for %, $at for @, $ds for $.
* [New] Added the option --force to the mime type handling operation on CLI.
* [Change] Changed the behaviour of column click on DataManipulationForm. Now, the sorting is triggered by holding control and clicking the desired column. Clicking a column without holding control will select all the item in that column.
* [Change] Changed the behavior of extension types registration, now they will be forcibly registered with complete name (schema.name) in order to follow the same rule of user-defined data types so the reverse engineering can work properly for both types handled by extensions and those not.
* [Change] Improved the type searching on PgSqlTypeWidget in such a way to display a popup with the data types matching the text typed by the user.
* [Change] Improved the NewObjectOverlayWidget in such a way to allow only one action to be executed at a time.
* [Change] Several clean ups in the source code root. Removed unused files.
* [Change] The subproject libpgmodeler was renamed to libcore and now moved to libs/
* [Change] The subproject libpgmodeler_ui was renamed to libgui and now moved to libs/
* [Change] The subproject libobjrenderer was renamed to libcanvas and now moved to libs/
* [Change] The main executable subproject was renamed to pgmodeler and now moved to apps/
* [Change] The main-cli executable subproject was renamed to pgmodeler-cli and moved to apps/
* [Change] The crashhandler subproject was renamed to pgmodeler-ch and moved to apps/
* [Change] Created a subfolder called "assets" to store all deployed files that aren't libraries or executables: conf, lang, schemas, samples, etc.
* [Change] Refactored all .pro/.pri files in order to reference pgModeler libraries from variables $$LIB[LIBNAME] instead of relative paths.
* [Change] Renamed all icons that was in Portuguese to English.
* [Change] In SnippetsConfigWidget replaced the SQL highlight by Schema micro-language highlighting.
* [Change] Improved the syntax error messages in SchemaParser.
* [Change] Adjusting the general toolbar width according to the screen DPI.
* [Change] Source files and ui files in libgui were reorganized by contexts (widgets, tools, settings, utils, dbobjects)
* [Change] pgModeler will now create missing config files in the startup.
* [Change] Minor adjustment in AppearanceConfigWidget to enable scrollbars in the viewport so users in small screen can see the entire model.
* [Fix] Fixed the crash handler and bug report form in such a way to load the last modified model opened before their execution.
* [Fix] Fixed a crash when the database import process fails to create an inheritance relationship somehow.
* [Fix] Fixed a bug in database import process that was not correctly setting up the data type name of columns when the types were handled by extensions. Types handled by extension don't need to have the schema name prepended.
* [Fix] Minor fix in database import process in such a way to properly update the table rendering to hide schema names from title box.
* [Fix] Fixed a malformed SQL code when configuring timestamptz in PgSqlType.
* [Fix] Fixed a bug in SyntaxHighlighter that was wrongly highlighting the entire document when a multiline group had a line break as final expression.
* [Fix] Fixed the syntax highligting files (xml, sql).
* [Fix] Minor fix in CLI in order to associate .sch files to the schema editor.
* [Fix] Fixed a bug that was causing all layers to be active even if there was some inactive (invisible) when adding a new layer.
* [Fix] Minor fix in SyntaxHighlighter in order to return unformatted word when the group can't be forcibly defined in all exception cases when the word doesn't match any group.
* [Fix] Fixed the description of the option --output in CLI.
* [Fix] Fixed a bug in CLI that could lead to segfault when using diff option and a database model as input.
* [Fix] Minor fix in PgSQLTypeWidget to capture the text changing signal on data type combo so the type format input can be properly updated when the user types the desire data type.
