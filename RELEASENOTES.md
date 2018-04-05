v0.9.1-beta1
------

<em>Release date: April 6, 2018</em><br/>
<em>Changes since: <strong>v0.9.1-beta</strong></em><br/>

<strong>Summary:</strong> finally we've reached the last beta release of the 0.9.1! This time pgModeler brings important fixes and new features requested long ago which are finally implemented in experimental stage. <br/>

The first new feature we have in this release is the support to multiples relationships for the same pair of tables. In previous versions, pgModeler would refuse to create a second relationship between the tables A and B. From now on, pgModeler will accept the creation of one-to-many and many-to-many relationships linking A and B more then once. For the other kinds of relationships (generalization, copy, one-to-one) the old rule is still valid: only one relationship per table pair. <br/>

Some missing features of PostgreSQL are present in this version too being them: row level security (RLS) and identity columns. For RLS, introduced in PostgreSQL 9.5, we have added the related options to toggle it in tables (ENABLE|FORCE RLS) as well the support to policy objects which are the main part of this new security modality. The identity columns introduced in PostgreSQL 10 are now fully supported by pgModeler and you can make use of them from the column editing form.<br/>

Other new feature is the support to multiple check constraints by domains. Actually, this is more a patch than a new feature because pgModeler was unaware that domains could support more than one check constraint, so we have fixed that now. We are considering a new feature because this was completely absent in previous releases. <br/>

Now, talking about changes, we fixed an old missinterpretation of the PostgreSQL's documentations by changing the way extensions are stored in the database model. Previously, this kind of objects were stored at schemas level but the right way to store them is at database level. So now instead of seeing extensions in the schema's subtree you'll see them in the database subtree. All validation rules, export, import and diff processes were fixed in such way to reflect the right way to treat these objects. <br/>

Another important change, and that one is related to an annoying behaviour of the tool, is that pgModeler will silently ignore the absence of the plugins folder and proceed with the normal startup avoiding the display of a message box regarding the missing folder and automatically disabling the plugins search mechanism. <br/>

In the fixes section, we've included a patch for a bug that was causing recurrent crashes on macOS when the user tried to load a second model file making it impossible the working on two or more database models at once. Also, we've patched the reverse engineering in such way to avoid the duplication of data types related to tables, sequences and views which was causing problems in the validation process. Another fix was done in the diff process that was generating a malformed DROP command for extensions. <br/>

The set of changes of this release have 47 entries between new features, changes, improvements and bug fixes. Below we highlight some of them, for the complete list please consider reading the CHANGELOG.md. <br/>

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
* [Fix] Fixed the query catalog for built-in types to include the types related to domains.
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
