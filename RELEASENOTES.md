v1.1.2
------
<em>Release date: April 08, 2024</em><br/>
<em>Changes since: <strong>v1.1.1</strong></em><br/>

This patch release for pgModeler 1.1.x brings the following improvements and fixes:

* Added a sample model of the famous northwind database ported to PostgreSQL.
* Updated the sample model pagila.dbm. Now it uses the table partition feature.
* Minor adjustment in the SQL execution widget to notify the OS and blink the taskbar after running a SQL command while the main window is minimized.
* The command-line interface tool had its menu texts reviewed.
* The view editing form now displays an alert regarding extra semicolons at the end of the definition command.
* Disabled the cached object code in the database explorer widget.
* Fixed a bug in the generation of SQL for roles when no option is set.
* Fixed a bug in the automatic name truncation/disambiguation for long names.
* Minor fix in the code completion widget to select the first visible item in the list.
* Fixed a bug in the generation of the command COMMENT ON CONSTRAINT.
* Minor fix in the view conversion process (in pgmodeler-cli) to correctly extract the SQL definition of views with disabled code.
* Fixed the generation of view's SQL to ignore extra semicolons at the end of the command.
