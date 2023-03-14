v1.0.2
------
<em>Release date: March 14, 2023</em><br/>
<em>Changes since: <strong>v1.0.1</strong></em><br/>

This patch release for pgModeler 1.0.x brings the following changes and fixes:

* Fixed a bug related to importing referenced tables of FKs in partial import mode.
* Fixed the attributes toggler item border style in partition tables.
* Fixed a regression that was preventing the canvas color to be changed.
* Fixed a bug in BaseObjectWidget::finishConfiguration that was preventing the creation of a role and a table with the same names.
* Fixed the generation of DROP command for policy, trigger, and rule.
* Fixed a bug that was causing objects to be selected in the sample model at appearance settings.
* Forcing the usage of Qt 6.x due to problems on Windows when compiling with Qt 5.
* Minor adjustment in CodeCompletionWidget stylesheet.
* Saving/restoring the painter settings after drawing the background in ObjectsScene.
* Fixed a malformed diff code for policies.
