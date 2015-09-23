v0.8.2-alpha
------
<em>Codename: <strong>Faithful Elephant</strong></em>
<em>Release date: September 26, 2015</em>
<em>Changes since: <strong>v0.8.1</strong></em><br/>

<strong>Summary:</strong> this is the first release of the last version that will close the 0.8.x series. This one brings a few new features and important bug fixes mainly related to diff and import processes.<br/>

Below the key features and fixes of this new release are described. Please, refer to CHANGELOG.md to see the complete list of features and fixes.<br/>

<strong>Reverse engineering:</strong> <br/>
* [New] Created a mechanism to make default values of columns in the form nextval(sequence) be transformed in a link between the sequence and the column in the import process. This will diminish the divergences raised by the diff process.
* [Fix] Fixed a bug in import process that was wrongly prepending schema's name in types related to tables.

<strong>Diff process:</strong> </br>
* [Fix] Minor fix in ModelsDiffHelper to avoid diff generation errors related to the missing 'fk-defs' attribute.
* [Fix] Fixed a bug that was duplicating some foreign key creation code in diff process.
* [Fix] Fixed a bug in the diff process that was dropping columns linked to sequences when these ones were dropped.

<strong>Validation process:</strong> </br>
* [Fix] Minor fixes in the validation process to force graphical objects updates and object's tree updates to reflect the new ids.
* [Fix] Fixed a problem with validation that was trying to validate foreign keys without need.

<strong>Database explorer:</strong> <br/>
* [New] Columns that compose primary key and unique key are exposed as children of the constraint in the object tree at DatabaseExplorerWidget.
* [New] Foreign key objects selected in DatabaseExplorerWidget now expose, in two children items, the source and referenced tables/columns.

<strong>Solved leaks and crashes:</strong> <br/>
* [Fix] Added a workaround to avoid crashes and leaks related to relationship disconnection and validation.
* [Fix] Fixed a crash when trying to create a new foreign key after connect two tables using a 1:1 or 1:n relationship.

<strong>Miscelaneous:</strong> <br/>
* [New] NumberedTextEditor now is able to set a custom tab width.
* [Change] When using snippets in the SQL execution field the current code will not be cleaned up, instead the snippet will be appended to the current code.
* [Change] Removed the automatic view switching when saving the model.
* [Change] pgModeler now indicates the name of unsaved models before quit.
* [Fix] Minor fixes in the object naming. Now pgModeler will accept dollar signs in any portion of the string or even numbers as object's name but this will automatically quote the name to avoid errors.
* [Fix] Fixed the generation of DROP commands for triggers and rules.
* [Fix] Fixed a bug that was not setting up the object's schema correctly when creating new table or view inside a selected schema.
* [Fix] Minor adjustment on ui-style.conf to minimize the problems with dark themes.
* [Fix] Fix a bug that was preventing "deferrable" attribute for constraint triggers to be used in SQL definition.