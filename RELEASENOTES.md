v1.0.5
------
<em>Release date: July 26, 2023</em><br/>
<em>Changes since: <strong>v1.0.4</strong></em><br/>

This patch release for pgModeler 1.0.x brings the following changes and fixes:

* Added shortcuts to Z-stack operations in ModelWidget.
* Added missing PostgreSQL multirange types.
* Improved the import error message when it's not possible to create/import a permission which references a predefined PostgreSQL role.
* Minor code refactor in ObjectsScene and DatabaseModel.
* Fixed a bug in DatabaseModel that was causing FK relationships of a hidden layer to be displayed after loading the model.
* Fixed a bug in MainWindow that was causing the plugins config action icon to disappear when triggering validation.
* Fixed a bug in PgModelerCliApp::fixModel that was causing the generation of empty models when the input file had no tag <role>.
