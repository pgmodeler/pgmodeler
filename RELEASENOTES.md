v0.9.3-alpha
------

<em>Release date: March 26, 2020</em><br/>
<em>Changes since: <strong>v0.9.2</strong></em><br/>

<strong>Summary:</strong> after bringing some important improvements on 0.9.2 it's time to start another round of polishing all features available in pgModeler. This way, 0.9.3 development will be more focused on improvements of what already exists than the creation of new features. Of course, this will not prevent that some new functionality on the tool can be implemented, they will be just less prioritized. <br/>

This release brings some requests related to database model design, which includes, bulk objects' renaming, multiselection on object finder and model objects and the introduction of the objects' Z-stack handling, a.k.a, "send to back" and "bring to front".<br/>

Several bugs were fixed which were mainly related to reverse engineering and diff processes, but some other small bug fixes were provided over all parts of the tool. <br/>

There were some important changes too, many of them related to code refactoring that the users will not notice but will improve code maintainance since it reflects on fast response on bug fixes and new features.<br/>

The deployment processes were improved, the code now compiles without problems in Qt 5.14.x as well previous Qt versions. Additionally, the installer were almost all rewritten in order to provide a better experience for users who depend on binary packages. <br/>

Finally, in total, there were 77 change log entries in this version and some of them are detailed below. For the complete list of changes/fixes, please, read the CHANGELOG.md. <br/>

* [New] Added support to multiselection in ModelObjectsWidget.
* [New] Added support to objects' bulk renaming.
* [New] Added support to multiselection in ObjectFinderWidget.
* [New] Added support to middle button click on source code panel in order to paste the selected code in the current open SQL execution widget.
* [New] Added a "Stacking" action in popup menu that is used to control the z value of graphical objects.
* [New] Adding support to move objects on the Z stack up and down.
* [New] Added support to CLI to write system wide mime database on Linux and Windows.
* [New] Added the ability to remove owners from objects from Quick Actions > Change owner.
* [New] Added an fix step that removes IN keyword from functions signatures.
* [Change] Minor adjustments in order to make the objects listing a bit faster for large databases.
* [Change] In DatabaseModel the invalidated special objects are now stored in a list which is destroyed only when the model is destroyed too. This will avoid segfaults in some cases when the objects in that list are still being referenced in the operation list (Experimental).
* [Change] Refactored ObjectRenameWidget in order to support multiple objects renaming.
* [Change] Improved the DataManipulationForm in such way that the user can sort results by clicking the column names in the result grid. This will cause a new query to be performed by using the clicked column as sorting criteria.
* [Change] The current model's popup menu is now used as the "More" actions menu at MainWindow > general toolbar.
* [Change] Moved the 'Fix model' and 'Objects metadata' actions to the general toolbar (left) at main window under the menu "Fix".
* [Change] The class HintTextWidget was removed and replaced by simple html QToolTips.
* [Change] Small change in the icon related to new tab opening in SQLToolWidget.
* [Change] Minor adjustment on updatenotifierwidget.ui to allow link opening.
* [Change] Improved the installer script to work better on Windows.
* [Change] Improving the installer on Linux and Windows to select the installation for all users or current user.
* [Change] Improved the linuxdeploy.sh in order to remove the need to use the startup scripts by the binaries in the installer.
* [Change] Fixed the installscript.qs in order to correctly update the mime database on Linux.
* [Change] Minor improvements on the usability of swap objects ids form.
* [Change] Adjusting the linker options to make the executables search for libraries in PRIVATELIBDIR without forcing the usage of LD_LIBRARY_PATH.
* [Change] Updated deployment scripts to use Qt 5.14.x
* [Change] Changing the exposed main window from QMainWindow to MainWindow in PgModelerPlugin interface.
* [Change] Minor buttons position adjustments on WelcomeWidget.
* [Change] Minor refactoring on OperationListWidget in order to show a more compact text when displaying operations done over objects.
* [Change] Removed the workaround code that disables the ALTER SET OWNER  when the role associated to it is disabled and the object itself not. The best approach is to dissociate the role from the object so the ALTER instruction is not generated.
* [Fix] Fixed a shortcut duplication in DatabaseExplorerWidget.
* [Fix] Fixed the shortcut for partitioning relationships in new object overlay.
* [Fix] Fixed a minor bug in MainWindow that was causing the bottom widgets bar to be displayed even when object finder widget and model validation widget weren't visible.
* [Fix] Fixed a bug that was causing copy options to stay hidden for copy relationships in the relationship form.
* [Fix] Fixed the importing of extensions on PostgreSQL 9.4 and below.
* [Fix] Fixed the name uniqueness generation when the name is truncated when exceeding 63 bytes.
* [Fix] Fixed a bug that was preventing the recent menus cleaning to be persisted in the conf file.
* [Fix] Fix a bug in the validation of collation objects.
* [Fix] Fixed a crash in ObjectRenameWidget when renaming several objects in which there was the need to revalidate relationships.
* [Fix] Fix the importing/diff of columns based on PostGiS data types.
* [Fix] Fix a crash in UpdateNotifierWidget when compiling pgModeler using Qt 5.14.x.
* [Fix] Minor fix in PgSQLTypeWidget to avoid show an error message box when the user types an invalid data type name.
* [Fix] Additional fix on XmlParser::convertCharsToXMLEntities in order to replace special char to xml entities correctly.
* [Fix] Minor fix in the update notifier widget display position.
* [Fix] Minor fix in diff regarding to the comparison on column/domain default values. Now the values are compared in insensitive case.
* [Fix] Fixed the Windows installer in order to create the start menu item  correctly (system wide).
* [Fix] Fixed a bug that was allowing the applying changes of the forms on relationship added objects by hitting ENTER causing crashes.
* [Fix] Fixed the bug that was causing model restoration to be displayed when the application was started from a double click on a model file.
* [Fix] Fixed a bug in the metadata handling that was not restoring the collapse mode of tables.
* [Fix] Fix a bug that was causing wrong replacements during the reverse engineering of functions.
* [Fix] Fixed the method XmlParser::convertCharsToXMLEntities in order to avoid chars replacements within <![CDATA entities.
* [Fix] Fixed the oversized tooltips on Windows.
* [Fix] Minor warning message fix on pgmodeler.pro.
* [Fix] Fixed a bug in AppearanceConfigWidget and objects-style.sch that was causing problems while saving the objects styles.
