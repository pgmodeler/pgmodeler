v0.8.0-beta
------
<em>Codename: <strong>Faithful Elephant</strong></em>
<em>Release date: January 10, 2015</em>
<em>Changes since: <strong>v0.8.0-beta</strong></em><br/>
<strong>Summary:</strong> after six weeks of work this release brings a great set of improvements. A new feature brought is the SQL snippet support on SQL tool as a little facility for the lack of forms to handle database objects. The snippets are quite customizable (see settings dialog) and can be constructed from static SQL commands or from dynamic codes by using the pgModeler's <em>schema micro language</em>.<br/>
Still in snippets, they are accessible by the code completion on SQL input field or Snippets button both located on SQL Tool. Another shortcut for snippets is on database explorer by right-click some object and selecting the Snippets menu item.<br/>
The database explorer widget also brings a special control that lists object's properties, the main goal of this control is to help the user to identify some key attributes that may be useful when writing SQL commands or manage the database.<br/>

* [New] Created the widget SnippetConfigWidget in order to handle SQL snippets on SQLTool and DatabaseExplorerWidget.
* [New] Introduced a new expression %unset on SchemaParser to clear the named attributes.
* [New] Added support for permission on objects Type and Domain.
* [New] Added methods to handle snippet selection in DatabaseExplorerWidget and SQLToolWidget.
* [New] Added an option to enable/disable code completion widget on sql code input fields (linked to CodeCompletionWidget instances).
* [New] Introduced a new step on model validation: relationship configuration checking.
* [New] Added a panel related to object's properties on database explorer widget.
* [Change] Minor change on ColorPickerWidget in order to change color of pickers using stylesheet instead of QPallete.
* [Change] Minor improvements on ModelNavigationWidget to show the filename associated to a hovered item on combobox.
* [Change] Simplified the syntax for attributes on .sch files from @{attrb} to {attrib}.
* [Change] Updated all .sch files with the new attribute syntax.
* [Change] Minor improvements on configuration widgets in order to detect if the settings was changed, this will avoid the unecessary load/save of the configuration files in ConfigurationForm.
* [Change] Minor updates on sql-highlight.conf
* [Change] Updated the version info on platform specific installer files.
* [Fix] Minor fix on windeploy.sh to copy the correct libs.
* [Fix] Minor fix on macdeploy.sh in order to use Qt 5.4.
* [Fix] Fixes done in order to build pgModeler on Windows using Qt 5.4.
* [Fix] Fixed a bug on SchemaParser that was wrongly reading %define instructions inside if's and creating attributes when it was not need to create them.
* [Fix] Modifications done on attributes initialization at RelationshipWidget in order to fix some crashes. Now pgModeler runs without crashing when compiled in release mode using g++.
* [Fix] Minor fix on aggregates listing on Catalog class. Now the handled types are attached to aggregates' names.
* [Fix] Minor fix on config pages indexes on ConfigurationForm.
* [Fix] Minor fix on MainWindow when there are errors during configuration files loading.
* [Fix] Minor fix on object name formatting at DatabaseExplorerWidget.
* [Fix] Minor fix on SQLToolWidget to set the current database explorer instance properly.
* [Fix] Minor fix on rule catalog query.
* [Fix] Minor fix on table.sch schema file.
* [Fix] Fixed the object filter on database explorer.
* [Fix] Typo correction on ModelExportHelper.
* [Fix] Additional fix to remove g++ warnings.
