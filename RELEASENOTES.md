v0.9.0-beta1
------

<em>Release date: May 13, 2017</em><br/>
<em>Changes since: <strong>v0.9.0-beta</strong></em><br/>

<strong>Summary:</strong> this is the last release before we can finally launch the stable 0.9.0. This version was focused on improving the user's experience on high resulotion screens by introducing an automatic machanism that is capable to resize dialogs, objects and fonts according to the current screen settings. This support is quite experimental and will be polished in the next releases. <br/>

The only downside of the support to bigger screens is that the icons still can't be resized properly due to their low resolution. The solution for this issue is the complete redesign of the whole icon set, task that is planned to be done in any moment in pgModeler 1.0 development timeline. But I'll try to antecipate this job if there's some room for it.<br/>

Another feature introduced by this version is the support to browse the referenced rows as well the referrer rows in data manipulation dialog which will improve the the whole data handling experience. Basically, for a certain relationship between tables A -> B, pgModeler will indicate which row(s) in B are referenced by the selected row in A. The inverse indication, from B to A, is done as well when B references A through a foreign key, of course.<br/>

Below the change log for this release. Refer to the file CHANGELOG.md for the complete changes history:<br/>

* [New] Added the ability to standalone dialogs like import, diff, export and others to be resized according to the screen dpi and resolution.
* [New] Added an experimental routine that will resize windows according to the current screen resolution and font dpi.
* [New] Added support to browse referrer and referenced in DataManipulationForm.
* [New] Added an item under table items that stores the referrer tables in the DatabaseExplorerWidget.
* [New] Added the method BaseObjectView::getScreenDpiFactor to help resize scene objects according to the screen dpi/resolution.
* [Change] Minor adjustment on readonly items regarding to referenced and referrer tables in DatabaseExplorerWidget.
* [Change] Improved the tabs handling in SQLToolWidget in order to avoid confusion related to which database is being managed or queried currently.
* [Change] Improvements done in the context menu at DataManipulationForm to include the key actions related to the control buttons at the top right portion of the dialog.
* [Change] Improved the external script handling in SQLExecutionWidget.
* [Change] Applied automatic resize for TaskProgressWidget.
* [Change] Improvement done in model restoration dialog that is now displayed after the main windows is exposed.
* [Fix] Fixed a problem in UpdateNotifierWidget that was receiving error 403 from the site.
* [Fix] Fix a bug in DataManipulationForm that was causing order by clause to be nullified by comments added in the filter field.
* [Fix] Fixed a regression in permission code generation.
* [Fix] Fixed a bug in the generation of grant/revoke commands for columns.
* [Fix] Fixed a bug that was causing the sorting options of index elements to be wrongly hidden.
* [Fix] Minor fix in the site url.
* [Fix] Minor fix in the filter toggling action in ObjectFinderWidget.
