v0.8.2-beta
------

<em>Codename: <strong>Faithful Elephant</strong></em><br/>
<em>Release date: January 12, 2016</em><br/>
<em>Changes since: <strong>v0.8.2-alpha1</strong></em><br/>

<strong>Summary:</strong> this is the first beta release of the 0.8.2. This one brings a few fixes for some bugs discovered during the final days of 2015.<br/>

An initial support to the fresh PostgreSQL 9.5 was included. Currently it'll be possible to connect to this new server version as well doing all supported operation so far: export, import and diff. <br/>

There is a basic support to BRIN indexes and new events of Event Trigger object, nothing more. The real support to PostgreSQL 9.5 including new object and important changes in another ones is scheduled to be implemented in pgModeler 0.9.x. <br/>

Below all changes of this release are described. <br/>

* [New] Added version descriptor for PostgreSQL 9.5 enabling pgModeler to connect to it.
* [New] Added access method BRIN for indexes, operator classes and operator families as an initial support to PostgreSQL 9.5.
* [New] Added event "table_rewrite" for event triggers as an initial support to PostgreSQL 9.5.
* [New] Added "Diff" action to File menu.
* [Change] Minor improvement in DataManipulationForm adding the shortcut of "Copy selection" button to its tooltip.
* [Change] Improvements on DataManipulationForm on how pk columns are handled and used in the generated DML commands for UPDATE and DELETE.
* [Change] Minor improvement on ModelRestorationForm when listing temp models.
* [Change] Changed the hint text for "Disable render smoothness" option.
* [Change] Changed the hint text for "Validate before save, export and diff".
* [Change] Minor improvements on SQLExecutionWidget and DataManipulationForm to scroll items in the results grid by pixel not per item.
* [Fix] Fixed a bug in operations that convert integer to serial and vice-versa.
* [Fix] Fixed some header items text alignment.
* [Fix] Fixed a crash when loading a broken model. Instead of show the error message related to corrupted file pgModeler was being aborted.
* [Fix] Minor fix in hint text in SourceCodeWidget and ModelDatabaseDiffForm.
* [Fix] Fixed shortcut conflicts in MainWindow.
