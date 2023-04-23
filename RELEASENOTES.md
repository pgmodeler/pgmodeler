v1.0.3
------
<em>Release date: March 24, 2023</em><br/>
<em>Changes since: <strong>v1.0.2</strong></em><br/>

This patch release for pgModeler 1.0.x brings the following changes and fixes:

* pgmodeler-cli now logs objects that fail to be recreated in fix process into a log file stored in pgModeler's temp directory.
* Added a progress bar to model fix form and a cancel button which allows aborting the fix operation without close that form.
* Added a specific icon for CSV load button in CsvLoadWidget.
* Added the methods Trigger::getColumns and Trigger::addColumns.
* Adjusted the behavior of hide and close events of model fix form.
* Minor adjustment in pgmodeler-cli model fix messages.
* Refactored TriggerWidget to use an instance of ColumnPickerWidget.
* Fixed the database model file header validation for huge models in pgmodeler-cli.
* Fixed a bug in TableDataWidget::generateDataBuffer that was causing the generation of malformed CSV in some circumstances.
* Fixed the PluginsConfigWidget::initPlugins in such a way to remove the plugins that failed to load from the plugins grid.
* Fixed a bug in BaseRelationship::canSimulateRelationship11 that was wrongly returning true.
