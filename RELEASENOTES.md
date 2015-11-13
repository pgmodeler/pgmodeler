v0.8.2-alpha1
------
<em>Codename: <strong>Faithful Elephant</strong></em>
<em>Release date: November 13, 2015</em>
<em>Changes since: <strong>v0.8.2-alpha</strong></em><br/>

<strong>Summary:</strong> this is the second release of the last version that will close the 0.8.x series. This one brings a few fixes for bugs reported after the release of 0.8.2-alpha and some new minor features.<br/>

Below the key features and fixes of this new release are described. Please, refer to CHANGELOG.md to see the complete list of features and fixes.<br/>

* [New] Added an additional step in import process to validate inheritance relationships to avoid incomplete tables.
* [New] Added an additional relationship validation in model loading process when there are inheritances. This will avoid incomplete columns and validation errors related to "permanent invalidation state".
* [New] Created an exclusive exception code when a parent table is not found in the imported set. This error is raised during inheritances creation.
* [New] Added the signal s_connectionsUpdateRequest in DatabaseImportForm, ModelExportForm, ModelDatabaseDiffForm, ModelValidationWidget and SQLToolWidget in order to inform the main window that user has changed connections in those forms.
* [New] Added the ability to configure connections without using the main configuration form. Now the user is able to do this task by using the "edit connections" option in any combo related to connections.
* [Change] Minor adjustments in diff process messages.
* [Change] Minor adjustments on ModelValidationWidget, ObjectFinderWidget and SQLExecutionWidget resize event to change the shape of toolbuttons in order to avoid truncate texts when the window size is too small.
* [Change] Replaced the explicit hint texts from ModelValidationWidget by HintText instances.
* [Change] Minor adjustment on widgets that are used to set connections.
* [Fix] Fixed the way objects are destroyed when a model is closed diminishing the time consumed by that operation and the chances of crashes after their destruction.
* [Fix] Additional fix in database import feature. Inheritances will be automatically created when "auto resolve dependencies" is checked.
* [Fix] Fixed a crash when importing a database that contains big tables that handles multiple inheritances.
* [Fix] Fixed a crash in Windows version. A missing initialization in OperatorClassElement was leading to segmentation fault.
* [Fix] Fixed a bug in table and view editing form that was permitting to confugure permissions to new objects before create them in the model.
* [Fix] Fixed a problem in UpdateNotifierWidget when the server returns http status code 302.

