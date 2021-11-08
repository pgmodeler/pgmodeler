v0.9.4-beta1
------

<em>Release date: November 08, 2021</em><br/>
<em>Changes since: <strong>v0.9.4-beta</strong></em><br/>

<strong>Attention:</strong> the database model file structure has changed since the last stable 0.9.3. Models created in older releases will certainly fail to load due to incompatibilities because some attributes in the XML code don't exist anymore or have changed during the development of 0.9.4-beta1. Before loading your database models in this new release, please, make sure to have a backup of them all and follow the steps presented by the model fix tool to patch the documents' structure. Not paying attention to this situation may cause irreversible data loss! If the fix procedures aren't enough to make your database models loadable again, please, ask for help at the official support channels!<br/>

<strong>Summary:</strong> in preparation for the next major pgModeler release, the goal of this version was to bring only improvements and fixes to what was implemented until 0.9.4-beta and thus was done. <br/>

pgModeler 0.9.4-beta1 brings a few entries in its changelog since we didn't have serious bugs reported in the past two months. So, I decided to make some polishing in several portions of the tool. <br/>

Anyway, the improvement that is worth mentioning in this release is the experimental support for timescaledb in reverse engineering. So, now pgModeler is capable of generating models from databases that make use of that PostgreSQL extension without major issues. For pgModeler 0.9.4, we expect that any problem that eventually appears regarding the importing of timescaledb-based databases can be solved. 
<br/>

Below, the changelog entries of this version:<br/>

* [New] Added extra PostGiS data types to PgSQLType.
* [New] Created the method PgSqlType::isPostGiSType() which returns true if the current type is a PostGiS one.
* [New] Created the method PgSqlType::reset() that clears some attributes of the type.
* [New] Added the built-in type pg_lsn in order to make databases using timescaledb extension to be imported correctly.
* [Change] Minor adjustment in windowsdeploy.sh to create zip packages.
* [Change] Minor adjustment in demo version warning messages.
* [Change] Adjusted the catalog query filters in ModelDatabaseDiffForm to retrieve system and extension objects according to the checkboxes "Import system objects" and "Import extension objects".
* [Change] Adjusted the reverse engineering in such a way that the table children will follow the SQL disabled state of their parent tables.
* [Change] Allowing importing pg_lsn attributes in usertype.sch.
* [Change] Allowing the use of commas in enum type labels.
* [Change] Improved the output of model fix operation in CLI.
* [Change] The constants DataSeparator, UnescValueStart, UnescValueEnd, WildcardChar, FilterSeparator, FilterWildcard, FilterRegExp were moved from CoreUtilsNs to UtilsNs in order to be used in the parsers module.
* [Change] In SchemaParser the meta char $ds (dollar sign) was renamed to $ms (money sign). Also, a new meta char $ds (data separator) was added and translates to the special data separator character UtilsNs::DataSeparator.
* [Change] Minor adjustments in table.sch and foreigntable.sch schema files.
* [Change] Improved DatabaseModel::getUserDefTypesReferences in order to get all references to postgis data types.
* [Change] Improved ModelValidationHelper in order to set postgis extension a default comment when automatically creating it.
* [Change] PgSQLTypeWidget now can be configured in such a way to disallow the configuration of type qualifiers like length, precision, interval, and some other. This is useful when configuring data types for objects that don't require such attributes in the data type like parameter, aggregates, transforms, casts, operators.
* [Change] Changed the behavior of Parameter::getCodeDefinition when generating SQL code. Now, any type qualifier (except dimension descriptor []) will be discarded.
* [Fix] Fixed the catalog query that lists policies.
* [Fix] Added a minor workaround in DatabaseImportHelper::getType in order to treat the "any" pseudo-type correctly.
* [Fix] Minor fix in DatabaseImportForm in order to disable option buttons when no connection is selected.
* [Fix] Fixed a segfault when importing domains with long expressions.
* [Fix] Fixed the broken SQL generation for tables with columns/constraints disabled.
* [Fix] Fix the name of the checkbox related to updates checking to avoid breaking the building when enabling NO_UPDATE_CHECK via qmake.
* [Fix] Fixed the shortcuts of Copy/Paste actions in DataManipulationForm.
