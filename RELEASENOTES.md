v1.1.4
------
<em>Release date: August 21, 2024</em><br/>
<em>Changes since: <strong>v1.1.3</strong></em><br/>

This patch release for pgModeler 1.1.x brings the following improvements and fixes:

* Fixed a bug in the function object that was not resetting the SETOF flag when changing the return type to TABLE.
* Fixed a crash in the database import process when destroying detached inherited columns.
* Fixed a bug in the system catalog query class that was crashing the application while trying to retrieve user mapping comments.
* Fix a bug in the index object that was preventing the removal of included columns.
