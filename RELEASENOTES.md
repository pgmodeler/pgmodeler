v0.8.1-alpha
------
<em>Codename: <strong>Faithful Elephant</strong></em>
<em>Release date: April 20, 2015</em>
<em>Changes since: <strong>v0.8.1-alpha</strong></em><br/>

<strong>Summary:</strong> this release brings a small series of fixes for bugs reported for 0.8.0 and 0.8.1-alpha.

One fix that deserves special attention is related to the use of threads. Those people that were facing UI hangups when validating, exporting or diff'ing large models will note that this times pgModeler will not crash. This is why threads are now correctly created and handled avoiding the excessive CPU usage.

There are more changes in this version and the key ones are listed below. Please, refer to CHANGELOG.md to a complete set of modifications.<br/>

* [New] Added the ability to import objects from an existent database to a currently working model.
* [New] Improvements on DatabaseImportHelper to dump the objects attributes in debug mode or to the log file when "ignore errors" is checked.
* [New] Added a fix step in CLI to fix functions signatures that includes OUT parameters.
* [Change] Minor adjustment on model validation progress and output.
* [Change] Minor adjust on model export progress on CLI.
* [Change] Minor improvements on objects validation process when dealing with broken relationship config as well with FKs referencing a column created before it.
* [Change] Change in user's configuration copy process. Now only configuration files are copied, folders are not copied anymore.
* [Change] The CLI now references template configuration files instead of user's when configuring file association.
* [Change] The settings related widgets now references template configuration files in order to load defaults or save settings.
* [Change] Minor adjust on tag objects positioning.
* [Change] Fixed the generation of function's signature in Function. Now OUT parameters aren't included.
* [Change] Fixed the generation of function's signature in DatabaseImportHelper. Parameters signaled with 't' in their modes aren't included.
* [Fix] Added a proper quotation to the command used to trigger the crash handler.
* [Fix] Minor fix when displaying the row amount for the selected table on database explorer.
* [Fix] Added a patch into swap id dialog when dealing with broken relationships.
* [Fix] Minor fix on relationship line configuration when redrawing them during the relationship validation process.
* [Fix] Minor fix when drawing views with extended attributes.
* [Fix] Minor fix when enabling/disabling zoom buttons when the current zoom reaches the minimum or maximum values.
* [Fix] Minor fix on thread operations on Windows system.
* [Fix] Fixed the export to SQL and PNG in thread mode.
* [Fix] Fixed the UI hang up when exporting a huge model by using BlockingQueueConnection in some slots to avoid thread consume all CPU resources.
* [Fix] Fixed a endless loop in PgModelerUiNS::formatMessage.
* [Fix] Minor fix in the generation of ALTER TABLE .. ADD COLUMN command.
* [Fix] Fixed the operator name validation.
