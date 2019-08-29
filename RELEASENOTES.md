v0.9.2-beta1
------

<em>Release date: September 2, 2019</em><br/>
<em>Changes since: <strong>v0.9.2-beta</strong></em><br/>

<strong>Summary:</strong> this release brings a few improvements as we're starting to deaccelarate the development of new features and concentrating on minor changes and bug fixes until the stable 0.9.2 is done. <br/>

The main focus was to improve the data manipulation form, fix some small UI problems and bugs on some features detected previously. <br/> 

The file saving was patched in such way to write a backup file prior to the saving of a model to avoid data loss in rare circumstances (mainly on macOS systems). <br/>

The diff process was also fixed due to a bug that was returning some false-positive results when dealing with timestamptz columns. <br/>

This version also brings the ability to select multiple table children (columns, contraints, triggers, etc) and operate over them by using the design popup menu. <br/>

Finally, some changes of this version are detailed below. For the complete list of changes/fixes, please, read the CHANGELOG.md. <br/>

* [New] Added a routine to write a backup file during the saving of model files in order to avoid data loss in some rare cases.
* [New] Added a new action on DataManipulationForm to clear the selected items by pressing Ctrl+R.
* [New] Adding support to duplicate several table objects at once.
* [New] Added support to table children multiselection by using Ctrl+Shift+Click on them.
* [New] Added support to load the generated diff code in the SQL tool and automatically browsing the destination server for manual diff applying.
* [New] Added a button to create a new data manipulation form from within another data manipulation form.
* [Change] Minor improvement on CSV text copying to exclude from the buffer the hidden columns.
* [Change] Improved the UI of the DataManipulationForm by rearranging the tool buttons to the left of the dialog.
* [Change] Improved the clipboard text pasting on DataManipulationForm in order to parse the text as CSV when Ctrl+Shift+V is pressed.
* [Change] Minor adjustment on SceneInfoWidget to avoid display dimensions of selected columns/constraints on the canvas.
* [Change] Improved the object protection action in ModelWidget in order to accept the multi item selection on tables.
* [Change] Improved the diff code preview allowing the user to close the dialog without going back to previous tab and click 'Cancel'.
* [Change] Changed the way the model saving timers are controlled to avoid infinity savings on the model (when the auto saving option is disabled) making the application unusable.
* [Fix] Fixed the object duplication action when only a single table object is selected.
* [Fix] Fixed a bug in ModelWidget::removeObjects that was causing the removal of relationship added columns
* [Fix] Fixed the ModelWidget::configurePopupMenu to consider the table's multi item selection.
* [Fix] Fixed a false-positive diff result when dealing with timestamptz.
* [Fix] Fixed the progress calculation of dbm files loading.
* [Fix] Fixed a bug that was causing the generation of broken view code.
* [Fix] Fixed the display of index expressions on DatabaseExplorerWidget.
* [Fix] Minor fix on SQLExecutionWidget to avoid results grid to overlap command execution history widget in certain cases.
