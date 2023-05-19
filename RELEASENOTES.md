v1.0.4
------
<em>Release date: May 19, 2023</em><br/>
<em>Changes since: <strong>v1.0.3</strong></em><br/>

This patch release for pgModeler 1.0.x brings the following changes and fixes:

* Now pgModeler selects the UI element colors based upon the system's default colors set (light/dark).
* pgModeler now restores the default settings in case of some configuration file is corrupted/incompatible and causes the initialization to fail.
* Allowing sequences to be assigned to columns with numeric type.
* Created the methods GuiUtilsNs::saveFile and GuiUtilsNs::loadFile.
* Removed the deprecated attribute partial-match from configuration files in conf/defaults.
* Minor adjustment in output messages in PgModelerCliApp::createConfigurations.
* SyntaxHighlighter when in single-line mode will strip any line break char in the input field.
* Minor adjustment in SyntaxHighlighter to force no line wrap in the parent input when single_line_mode is activated.
* Changed the behavior of TableDataWidget::populateDataGrid. Instead of failing and never opening the dialog again for the user to try to import another file, the method now ask for saving the current (corrupted data) to a file and opens an empty grid to a new CSV import.
* Removed unused constant PhysicalTable::DataLineBreak.
* Fixed a false-positive diff result when comparing numeric columns.
* Minor fix in the name pattern of the settings backup folder in PgModelerCliApp::createConfigurations.
* Fixed bug in reverse engineering that was happening during the creation of object permissions.
* Minor fixes in the catalog queries for cast, collation, and role.
* Fixed the wrong usage of cached names and signatures in DatabaseImportHelper.
* Additional fix in CsvParser::parseBuffer to append a line break character at the end of the buffer in case it is missing so the parsing can be done correctly.
