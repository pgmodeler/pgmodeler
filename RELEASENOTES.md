v1.2.0-beta
------
<em>Release date: February 10, 2025</em><br/>
<em>Changes since: <strong>v1.2.0-alpha1</strong></em><br/>

<em><strong>Attention:</strong> Some configuration files were changed in pgModeler 1.2.0-beta causing a break in the backward compatibility with some pgModeler 1.1.x settings. This way, at the first start of the newer version, pgModeler will automatically try to migrate the older settings to the newer ones! Be advised that this is a release of a version in the early stages of development. Bugs, malfunctioning, crashes, and other problems are expected and will be resolved as soon as they are reported. Make sure to make backups of your models and databases before using this version!</em><br/><br/>

After almost 4 months since the release of the last development version of 1.2.0, here we are announcing its first beta! This means that 1.2.0 has reached its final shape and from now on I will work exclusively on fixing bugs and polishing the new features that will be arriving in the stable version! Below some new features and changes are detailed.

* **Relationships' FK columns indexes:** Now, the relationships that automatically create foreign key columns, can also create indexes over that columns too. This can speed up a bit more the modeling process. The generated indexes, like relationships, are named after the specific pattern and the user has total control over the generated names in the relationship editing form or by defining a global name pattern under relationships settings.

* **Quick create constraints, indexes, and relationships:** This feature, introduced through an exclusive plugin in the paid version of the tool, allows the creation of constraints, indexes, and relationships based on the objects selected in the design view, without the need to open a single editing form or fill lots of fields. The objects created also support name patterns, and they can be configured in the plugin's settings menu in the main window.

* **Miscellaneous:**

* [New] When the export, import, and diff processes finish, the taskbar blinks when the window is not visible.
* [New] Added support for the total number of rows in the table being handled in the data handling form.
* [Change] Set new icons for tree widgets collapse and expand actions.
* [Change] Updates on icons-*.conf files.
* [Change] Minor layout adjustment in the data handling form.
* [Change] Minor adjustment in the layers configuration widget to accept Enter/Return to apply settings.
* [Fix] Fixed some shortcut conflicts in the main window.
* [Fix] Minor fix in the "Open relationship" action in the design view.
* [Fix] Fixed the index catalog queries when using pgModeler in compatibility mode (PG 9.x).
* [Fix] Minor fix in buttons' drop shadows in data handling form.
