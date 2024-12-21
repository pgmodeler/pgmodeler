v1.1.6
------
<em>Release date: December 21, 2024</em><br/>
<em>Changes since: <strong>v1.1.5</strong></em><br/>

This patch release for pgModeler 1.1.x brings the following improvements and fixes:

* Fixed a bug in the FK relationships update routine that was causing more relationships to be deleted than what really needed to be.
* Fixed a crash when creating a many-to-many relationship where one or more columns of the involved primary keys use generated default values (GENERATED AS).
* Fixed a bug in the routine that updates relationships' generated objects that was causing the application to hang up when importing a simple hierarchy of tables.
