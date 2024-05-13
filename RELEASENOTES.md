v1.1.3
------
<em>Release date: May 13, 2024</em><br/>
<em>Changes since: <strong>v1.1.2</strong></em><br/>

This patch release for pgModeler 1.1.x brings the following improvements and fixes:

* Added an option to toggle the display of the mouse position, zoom, and object selection information.
* Minor adjustment in UI stylesheets of the scene info widget.
* Adjust the scene info widget layout to diminish the components wobbling when moving objects.
* Minor adjustment in changelog entry validation in the database model.
* Fixed a shortcut conflict in the text search/replace widget.
* Minor fix in the database model objects recreation process (during model fix operation) to warn about invalid changelog entries avoiding aborting the entire model fix process.
* Fixed a bug in the catalog class that was not retrieving comments of sequences causing false-positive to be generated in diff process.
