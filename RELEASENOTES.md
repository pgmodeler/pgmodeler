v0.9.1-beta1
------

<em>Release date: April 6, 2018</em><br/>
<em>Changes since: <strong>v0.9.1-beta</strong></em><br/>

<strong>Summary:</strong> <br/>

* [New] Added the ability to create multiples one-to-many and many-to-many relatationships between the same pair of tables.
* [New] Added the ability to use more special ascii chars in the middle of object names.
* [New] Added support to multi line comments in UI.
* [New] Added full support to row level security (RLS), including export, import and diff of this kind of object.
* [New] Added support to bulk data editing in DataManipulationForm.
* [New] Added an option to diff process to force the generation of DROP commands for columns and constraints even if the missing objects need to be preserved. This is useful to work with partial models and the user need to remove columns/constraints and preserve the rest of objects.
* [New] Added support to identity columns (PostgreSQL 10).
* [New] Added the support to BYPASSRLS option on roles.
* [New] Added support to IS_TEMPLATE and ALLOW_CONNECTIONS options in database object.
* [New] Added the procedures to fix old style domains in CLI.
* [New] Added support to multiple check constraint in domains.
* [New] Added support to sort items alphabetically (ascending) or by oid in DatabaseExplorerWidget.
* [Change] Changed the input mode of the password field in ConnectionsConfigWidget in order to hide the passwords in the form. NOTE: the passwords are still in plain text in the config file.
* [Change] Moved extensions from schema level to database level in order to reproduce better the PostgreSQL's behavior.
* [Change] In GeneralConfigWidget when restoring default settings the default settings for syntax highlight are restored as well.
* [Change] pgModeler will not try to create the plugins path anymore. This will avoid constant error messages during startup. Now, it'll silently ignore the absence of that folder and skip the plugin loading.
* [Change] Improved the source editing in external application. The use is informed about the app running state and the contents for the source editor field are locked until the user closes the external app.
* [Fix] Fixed the extension creation, allowing only one instance of the named extension per database no matter the schema used to allocate its children objects.
* [Fix] Fixed a bug when dropping Functions in DatabaseExplorerWidget.
* [Fix] Fixed a bug that was causing the disabling of connections for database models created prior to 0.9.1-beta1.
* [Fix] Fixed a bug on import process that was wrongly creating types derivated from tables/sequence/views causing duplication problems during validation.
* [Fix] Fixed a crash on macOs when opening a second model.
* [Fix] Fixed an issue in diff process that was generating a malformed DROP command for extensions.
* [Fix] Fixed the diff for domains which contain multiple check constraints.
* [Fix] Fixed a bug that was not selecting the correct spatial type in the widget.
* [Fix] Fixed a conflict of shortcuts in DatabaseExplorerWidget. Now F5 updates a leaf/subtree and Alt+F5 performs quick refresh of the tree.
* [Fix] Fixed a problem with sqlexecutionwidget.ui that is not building properly in Qt 5.10.
