v1.1.0-alpha1
------
<em>Release date: September 29, 2023</em><br/>
<em>Changes since: <strong>v1.1.0-alpha</strong></em><br/>

<em><strong>Attention:</strong> Some configuration files were changed in pgModeler 1.1.0-alpha1 causing a break in backward compatibility with pgModeler 1.0.x settings. This way, at the first start of the newer version, pgModeler will try to migrate the older settings to the newer ones automatically!</em><br/><br/>

After five months of development, the first alpha release for pgModeler 1.1.0 is finally ready and brings some important improvements compared to 1.0.x. Below, the key changes are briefly detailed:

**Improved code completion:** A long-awaited feature is finally arriving pgModeler, the code completion based on living database object names. From now on, in the SQL execution widget, will be possible to list column/table names in the middle of the INSERT/DELETE/TRUNCATE/UPDATE commands. This feature also considers table aliases and lists the correct columns. It is worth mentioning that this feature is still experimental despite the good results on different kinds of SQL commands. <br/>

**Improved UI theme management:** In 1.0.x, pgModeler wasn't able to correctly follow the system's color set (dark/light). Now, in 1.1.0-alpha, selecting "System default" UI theme in Appearance settings makes pgModeler properly configure the UI color theme as well as the source code highlight settings to follow the system's color schema. <br/>

**Several UI improvements:** As always, general UI improvements are made attending to the users' requests. This time pgModeler brought a long list of enhancements, but the following ones are worth mentioning: <br/>
* pgModeler now supports the drag & drop of .dbm files selected in the file manager directly into the tool's main window to load models.<br/>
* Database object shadows can be now deactivated in the general settings. This feature improves the rendering speed since fewer objects per scene need to be handled.<br/>
* The SQL command results in the SQL execution widget and data manipulation form can be now exported to CSV format besides the classic plain text format.<br/>
* When pasting SQL code coming from external IDEs in SQL execution using Ctrl+Shift+V, pgModeler will try to remove unneeded string concatenation characters (e.g. " ' + .) in the clipboard text before inserting it in the input field. This is useful for testing SQL code written in other languages' source code using concatenated strings.<br/>
* Enclosing characters (),[],{} are now highlighted in any field where it's possible to type SQL code, being helpful to keep the balance of those characters avoiding syntax errors when running the typed commands.<br/>
* In order to display large data (e.g. bytea data) in the results grid without degrading the performance an option was added to general settings that toggle the truncation of column data that exceeds a certain limit. The truncated data can be fully visualized by double click the desired cell in the result grid.<br/>
* Now the SQL result grid header will display an icon according to the data type of each column.<br/>
* In data manipulation form when editing a single element with column data edit dialog, the form will display the current column's value.<br/>

**Bug fixes:** Also, as part of the constant search for the overall tool's stability and reliability, almost twenty bugs were fixed, and below we highlight some key ones:<br/>
* Fixed a critical bug in pgmodeler-cli that was causing the generation of empty models when the input file had no roles configured.<br/>
* Fixed a bug in the database model that could lead to an "unknown exception caught" error.<br/>
* Fixed a bug in the CSV generation that was causing the creation of malformed CSV in some circumstances causing the initial table data corruption.<br/>
* Fixed some problems with comments when importing a database having the same OIDs for different types of objects.<br/>
* Fixed the diff process performed on legacy database versions.<br/>

**Backup utility plugin:** This version introduces the backup utility plugin, available in the paid version of the tool, which implements a user-friendly interface for the commands _pg_dump_, _pg_dumpall_, _pg_restore_ and _psql_ commands. This extra feature was developed mainly focused on attending to those users less comfortable with command-line tools, being possible to dump and restore databases without leaving pgModeler's GUI. Of course, advanced users are welcome to use the plugin and help to improve it! In a nutshell, besides configuring the backup tools parameters with a simple form, it allows the creation of presets per backup tool for different needs, it also has some facilities that automate the backup file name generation by using a default backup folder and name patterns.<br/>

Finally, for more detailed information about this release's changelog, please, refer to the CHANGELOG.md file.
