v1.2.0
------
<em>Release date: May 9, 2025</em><br/>
<em>Changes since: <strong>v1.1.0</strong></em><br/>

<em><strong>Attention:</strong> pgModeler 1.2.0 introduces configuration changes that may affect compatibility with 1.1.x settings. On the first launch, pgModeler will automatically attempt to migrate your existing settings. Please note that this is an early development release where stability issues may occur. It's recommended to back up all models and databases before use. Please report any found bugs for prompt resolution. The mentioned migration process helps transition to improved configurations while minimizing disruption to your workflow.</em><br/><br/>

**New features** <br/>

This release introduces several enhancements to improve database modeling workflows. The SQL source code editor now offers per-instance control line wrap along with an integrated search/replace widget. Debugging capabilities have been expanded with dedicated output tabs in the database import and diff forms. The diff feature restored the force object re-creation support in both CLI and GUI, but now with a type selection popup menu for fine-tuned object re-creation. The database model now provides finer control over SQL generation by allowing the disabled SQL code to go into the generated scripts. The extension object handling has been upgraded for better child object management (currently, only children types and schemas are tracked). The system objects can now have permissions assigned. <br/>

**Changes and enhancements** <br/>

The model objects widget now accepts an Alt + click over a graphical object, highlighting it in the design view. Redundant search/replace instances all over the tool were removed due to the integrated search/replace widget in the source code editor. The extension editing form now properly supports custom schema names in data types, while the database import process and the database model itself have been refactored for more reliable handling of extension-owned objects using the new extension object structure. The comparison operations in the diff process have been optimized to filter system objects correctly, and the function behavior type was simplified by dropping the STRICT type since it has the same semantics as RETURNS NULL ON NULL INPUT. The database import form, when in debug mode, will remain open so the user can inspect the commands and objects created during the process. <br/>

**Bug fixes** <br/>

* Fixed some crashes during diff operations on extension-created tables.
* Fixed the importing of columns using arrays of user-defined types.
* Fixed the time zone persistence on the timestamp data type.
* Removed false-positive diffs for functions with comments or STRICT behavior.
* Fixed reverse engineering of uppercase type names.
* Addressed crashes in identifier relationships with FK indexes.
