v0.9.4-alpha1
------

<em>Release date: July 15, 2021</em><br/>
<em>Changes since: <strong>v0.9.4-alpha</strong></em><br/>

<strong>Attention:</strong> the database model file structure has changed since the last stable 0.9.3. Models created in older releases will certainly fail to load due to incompatibilities because some attributes in the XML code don't exist anymore or have changed during the development of 0.9.4-alpha. Before loading your database models in this new release, please, make sure to have a backup of all of them and follow the steps presented by the model fix tool to correct the documents' structure. Not paying attention to this situation may cause irreversible data loss! If the fix procedures aren't enough to make your database models loadable again, please, ask for help at the official support channels!<br/>

<strong>Summary:</strong> <br/>

Finally, the whole set of changes of this release has 76 entries being 21 new features, 34 changes/improvements, and 21 bug fixes. Below, some key changes are highlighted. For the complete list of changes, please, take a look at the CHANGELOG.md. <br/>

* [New] Added compatibility code that allows loading models created in versions before 0.9.4-alpha without losing layers and active layers information.
* [New] Added an option to the metadata handling operation that serves to indicate if duplicated objects (textboxes, tags, generic sql) must be merged or not.
* [New] Added support for configuration parameters and transform types on functions and procedures.
* [New] Added the ability to rename the default layer.
* [New] Added support for setup, in general settings, the custom color for grid lines, canvas area and page delimiter lines.
* [New] Added support for multiple layers on the model.
* [New] Added support for PARALLEL attribute to functions.
* [Change] Minor adjustments on Messagebox resizing during first display.
* [Change] Minor improvement on Messagebox in order to display the stack trace toggle button in a more intuitive way.
* [Change] Changed the behaviour of the method ObjectsScene::removeLayers, now it'll allow the resetting or not the layers of the objects.
* [Change] Adjusted the minimum size of the main window to 640x480 so it can fit in smaller screens.
* [Change] Change on the search path for PluginsDir to use getPathFromEnv().
* [Change] DatabaseModel will now create a default layer in the constructor.
* [Change] The database model will add the default layer to the loaded model if there isn't a single layer identified.
* [Change] DatabaseModel when loading older database model files will generate random layer colors.
* [Change] Improved the window title of DataManipulationForm in sucha a way to display the currently browsed table.
* [Change] Improved the selection of layers to be associated to object in context menu (Quick > Set layers). Now a floating widget is use to select multiple layers for the object selection in the canvas area.
* [Change] Updated the sample models to use "layers" attributes on graphical objects.
* [Change] pgModeler CLI will now rename "layer" attributes to "layers" in order to reflect the multiple layers support for older models.
* [Change] The option "Truncate before alter columns" in the diff process was removed since its use is discouraged and dangerous in some situations.
* [Fix] Minor fix in the table's catalog query in order to avoid setting up a partitioning strategy when the table is not a partitioned one.
* [Fix] Minor fix in function.sch in order to generate transform types only for PostgreSQL >= 9.5.
* [Fix] Minor fix to avoid black canvas when using settings from 0.9.3 or below on 0.9.4-alpha.
* [Fix] Minor fix in ObjectsScene in such a way to update layer rects when the grid alignment option is toggled.
* [Fix] Fixed the buttons shortcuts on LayerConfigWidget.
* [Fix] Minor fix on SchemaView to compute the last position any time the object is reconfigured.
* [Fix] Fixed the key combination to trigger the magnifier tool.
* [Fix] Minor fixes in ModelWidget, MainWindow to update scene layer settings correctly.
* [Fix] Fixed the CLI in such a way to update scene layer settings so the exporting processes can generate graphical files correctly.
* [Fix] Fixed a bug in the layers removal process that was causing objects to be placed in a incorrect layer index after the removal.
* [Fix] Fixed the "Fix" menu in main tool bar.
* [Fix] Fixed a bug in ModelWidget that was causing overlay to be displayed when a textbox object was selected.
* [Fix] Fixed wrong text in collationwidget.ui.
* [Fix] Fixed a bug during the loading of changelog entries.
* [Fix] Fixed the display of warnings during export in ModelExportForm.
* [Fix] Fixed the display of warning messages during export in ModelDatabaseDiffForm.
* [Fix] Fixed a malformed SQL code for triggers.
* [Fix] Fixed the broken build on Qt 5.9.x.
* [Fix] Fixed a bug that was preventing special PKs to be properly configured.
* [Fix] Fixed the SQL generation of functions based on internal language.
