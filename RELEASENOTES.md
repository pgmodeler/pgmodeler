v1.1.0-beta1
------
<em>Release date: December 30, 2023</em><br/>
<em>Changes since: <strong>v1.1.0-beta</strong></em><br/>

<em><strong>Attention:</strong> Some configuration files were changed in pgModeler 1.1.0-beta1 causing a break in backward compatibility with pgModeler 1.0.x settings. This way, at the first start of the newer version, pgModeler will try to migrate the older settings to the newer ones automatically!</em><br/><br/>

<strong>Summary</strong> This version was supposed to be released earlier this month, but after discovering a critical problem on macOS that almost made pgModeler unusable, there was the need to perform lots of refactoring on all portions of the UI, which consumed more time than expected. Fortunately, most problems were solved and now the macOS version runs normally after a long routine of adjustments and tests. Of course, one or another corner case may not be covered and eventually will be quickly patched once reported.<br/>

**Changes/improvements**<br/>
* [Change] Minor adjustment in connections configuration form in such a way to reload connections where they a used only when the user applies new connection settings on the form or restores original connections by clicking "Cancel".
* [Change] Minor improvement on the object ids swapping form in such a way as to preserve the sorting parameters after swapping object ids.
* [Change] Minor change in the operation list widget in such a way as to use icons instead of labels for operation list size and current history position.
* [Change] Refactored signal/slot connections that needed exception handling all over the tool avoiding crashing the application when an exception is raised (macOS only).
* [Change] Minor adjustment in model creation to avoid marking it invalidated even if it's new and empty.
* [Change] Minor adjustment in code completion widget to include sequences when listing living database objects.

**Bug fixes**<br/>
* [Fix] Fixed the catalog query operator.sch so argument data types can have the schema name prepended.
* [Fix] Fixed a crash when trying to display function/procedure/operator source code that references a domain in the parameters.
* [Fix] Fixed the catalog queries for function and procedure to correctly format parameters' data type signatures.
* [Fix] Fixed a problem in object ids swapping widget that was not capturing arrow key press on macOS.
* [Fix] Fixed a bug in the importing of partitioned table indexes which was affecting the results produced by the diff process.
* [Fix] Fixed a bug in the code completion widget that was crashing the application when trying to list columns of an alias related to a non-schema-qualified table.
* [Fix] Fixed a bug in reverse engineering related to the importing of partition tables hierarchy.
* [Fix] Fixed a bug in diff that was generating false-positive results for columns with the same user-defined type.
* [Fix] Fixed a bug in the line numbers widget, within source code input fields, that was not aligning the line numbers with their respective lines when the font size was different from the default.
* [Fix] Fixed a broken build related to XML parser that was using an xmlError * reference instead of a const xmlError * reference.
* [Fix] Minor fix in code completion widget to avoid the displaying of the widget in a position that extrapolates the screen limits.
* [Fix] Minor fix in the icon positioning in the list and tree widget items.

Finally, for more details about this version's changelog, please, refer to CHANGELOG.md.
