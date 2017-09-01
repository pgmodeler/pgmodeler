v0.9.0
------

<em>Release date: September 1st, 2017</em><br/>
<em>Changes since: <strong>v0.9.0-beta2</strong></em><br/>

<strong>Summary:</strong> after one year and three months of hard work we finally have the stable release 0.9.0. This version does not bring extraordinary changes or improvements compared to the alphas and betas but it closes one more development cycle delivering an even more refined product to our users. In order to give an idea on how pgModeler have changed since last year if we place togheter all change logs from the beggining of 0.9.0 development we would have: 175 log entries being 51 new features, 58 changes/improvements, 68 bug fixes. These numbers only indicates how the project is being constantly enhanced through the years.<br/>

About some of the new features, pgModeler now supports curved relationship lines in order to improve the model's visualization. By default, for new installations, this option comes activated but it can be toggled in the general settings. Another feature requested long ago and now is available is the ability to choose the UI language instead of rely only in the OS default language. One more enhacement is the possibility of copying and pasting text from one data manipulation dialog to another. This comes to be handy mainly when the user needs to copy data between tables when they reside in different databases.<br/>

Finally, small fixes and changes have been implemented as you can see below. The complete change history is in the file CHANGELOG.md.<br/>

* [New] Added the ability to paste text from clipboard to data grid in DataManipulationForm.
* [New] Added a new sample model donated by the maintainers of 3D City DB project.
* [New] Added the language "internal" to the set of system languages available when creating a new model.
* [New] Added support to override the default language settings via GeneralConfigWidget.
* [New] Added support to toggle curved relationship lines in GeneralConfigWidget.
* [Change] Improved the MetadataHandlingForm enabling user to only extract metada to a backup file.
* [Change] Minor adjustments in the graphical points when relationships are selected.
* [Change] Changed the default action to reset label's position from middle button click to Alt + Ctrl + left click.
* [Change] Minor enhancement in auto arrange feature to avoid breaking lines when curved relationship lines are enabled.
* [Change] Minor adjustment in the methods which automatically resize dialogs depending on the resolution.
* [Change] Changed the default font for objects and source code.
* [Fix] Fixed the catalog query for event triggers.
* [Fix] Fixed the icons and labels of the "Select all" submenu in ModelWidget.
* [Fix] Fixed a rare crash when configuring self relationships.
* [Fix] Minor fix when rendering self n:n relationships.
* [Fix] Minor fix in the HintTextWidget to resize more properly according to the held text.
* [Fix] Minor fix in default confs for source code font style.
* [Fix] Fixed the generation of objects style configuration file that was missing constraints settings.
* [Fix] Fixed a bug in the index / exclude constraint import which was not creating expressions of these objects correctly and sometimes trucating them.
