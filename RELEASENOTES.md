v0.9.0-alpha
------

<em>Release date: October 17, 2016</em><br/>
<em>Changes since: <strong>v0.8.2</strong></em><br/>

<strong>Summary:</strong> after four months of inactivity we have the first alpha release of 0.9.0. The main purpose of this version is to enable pgModeler to communicate with the recently arrived PostgreSQL 9.6. <br/>

Also, we have some small fixes and improvements to make pgModeler more user friendly. These set of changes and fixes are:<br/>

* [New] Enabling pgModeler to connect to PostgreSQL 9.6 servers.
* [New] Added the option to ignore error codes during the export process in CLI.
* [New] Added the ability to ignore extra errors by their codes in ModelExportForm and ModelDatabaseDiffForm.
* [New] Added the ability to load data from CSV file into TableDataWidget and DataManipulationForm.
* [Change] Minor update in snippets.conf by adding a SELECT * command.
* [Change] Removed deprecated exception ERR_ASG_ZERO_LENGTH.
* [Change] Improvements done in CodeCompletionWidget so that the completion can be more accurate mainly when using the form [schema].[table].
* [Change] Methods responsible for dropping and exploring data were moved from SQLToolWidget to DatabaseExplorerWidget.
* [Change] Improved the error output in DatabaseImportForm, ModelDatabaseDiffForm, ModelExportForm and Messagebox.
* [Change] TableDataWidget widget now can have the column names changed freely not only when there are invalid ones.
* [Change] Removed codename from AboutWidget.
* [Fix] Fixed a bug in PgSQLType and PgSQLTypeWidget that was not properly setting length = 1 in character, varchar and numeric data types.
* [Fix] Fixed a bug that was leading to stack overflow when generating object's sql plus its dependencies in huge models.
* [Fix] Fix the structure of the sample model pagila.dbm.
* [Fix] Minor fix in diff proccess in order to permit the comparison between a column added by relatinship and other that is not but share the same name.
* [Fix] Fixed a bug that could cause crashes when editing connections in DatabaseImportForm or ModelDatabaseDiffForm.
* [Fix] Fixed a crash when the user modified a connection on the fly with the SQL tool activated and trying to resume his work in database management.
* [Fix] Fixed the tab order in ConnectionsConfigWidget.
* [Fix] Fixed a bug in ModelDatabaseDiffForm that was running the export thread several times.
* [Fix] Fix the generation of truncate commands in the diff when the types of columns are incompatible.
* [Fix] Fixed a bug that was generating broken sql for tables when these objects have no constraints.
* [Fix] Fixed a bug in diff that was not detecting column types length changes.

Since this is not a release that bring major changes compared to the 0.8.2 the update is optional. But if you want to manage PostgreSQL 9.6 servers the update is needed otherwise you are fine with the 0.8.2.
