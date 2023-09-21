v1.1.0-alpha1
------
<em>Release date: September 29, 2023</em><br/>
<em>Changes since: <strong>v1.1.0-alpha</strong></em><br/>

<em><strong>Attention:</strong> Some configuration files were changed in pgModeler 1.1.0-alpha1 causing a break in backward compatibility with pgModeler 1.0.x settings. This way, at the first start of the newer version, pgModeler will try to migrate the older settings to the newer ones automatically!</em><br/><br/>

Here we are, after working for 4 months, bringing you the last alpha release of pgModeler 1.1.0. This version was mainly focused on improving performance on several parts of the tool. So I put a huge effort into refactoring lots of code to reach an amazing (almost unbelievable) result. Let's see below:

**Improved model loading, objects' searching, and validation speeds:** One of the most annoying things on pgModeler for me was the speed of the operations like objects' search, model validation, and, mainly, database model file loading. During the development of this version, I decided to face the challenge of improving these three operations, so I delved into the internals of the tool looking for the major bottlenecks. After selecting the problematic ones, I took the path of rewriting some mechanics instead of trying to fix them. The two main bottlenecks that degraded the speed of the mentioned operations were the objects' name validation/formatting as well as the retrieval of objects' dependencies and references. Those seemed simple operations that I even could imagine that they were making pgModeler struggle to handle big models. For the objects' name formatting and validation, I decided to create an internal name cache to avoid calling those procedures repeatedly. A simple solution that brought a surprisingly good result. For the objects' dependencies and references handling, I completely ditched the methods written for that purpose and created something infinitely simpler. Instead of calling every time a procedure that runs countless loops and recursive calls, I just made the objects store internally which other objects are their references and dependencies. Those changes made models that were loading/validating in several minutes to be processed in a few seconds. I still have some other bottlenecks to solve, but those two already removed, gave pgModeler an amazing performance.<br/>

**Several other improvements:** general improvements were made all over the tool and some of them are described below.<br/>
* Added support for inksaver color theme which uses only black and white colors for models that are used for printing.<br/>
* Added support for using object comments as aliases in database import.<br/>
* pgModeler now asks the user about closing SQL execution tabs that are not empty (with typed commands).<br/>
* Add support for remembering decisions on the alerts regarding unsaved models/open SQL tabs.<br/>
* Added an option in GeneralConfigWidget to reset the exit alert display status.<br/>
* Added a basic form to inspect changelog XML code.<br/>
* Added missing multirange types.<br/>
* Improved the relationship point addition and selection via mouse clicks.<br/>
* The "dot" grid mode is now the default in the appearance.conf file due to better drawing performance.<br/>
* Improved the scene background (grid, delimiter, limits) drawing speed for big models.<br/>
* Improving the objects' filtering in reverse engineering by introducing an "any" filter type.<br/>
* Data manipulation form now shows a confirmation message before closing when items are pending save.<br/>

**Bug fixes:** Also, as part of the constant search for the overall tool's stability and reliability, almost twenty bugs were fixed, and below we highlight some key ones:<br/>
* Minor fix in the object finder widget to avoid disconnecting a null selection which could lead to crashes.<br/>
* Minor fix in database model widget to hide the new object overlay when moving a selection of objects in the design area.<br/>
* Minor fix in the object removal routine in model widget that was not erasing an object in case it shared the same name of other objects in the same schema.<br/>
* Minor fix in the object addtion routine to validate the layer of the object being added. If one or more layers are invalid the object will be moved to the default layer 0.<br/>
* Minor fix in pgmodeler-cli whe extracting the objects's XML code during model file structure repair in order to restore correctly the layers name/count.<br/>
* Fixed a bug in partial reverse engineering that was not correctly importing functions in some specific conditions.<br/>
* Fixed a bug in partial reverse engineering that was not importing some objects' dependencies correctly.<br/>
* Fixed a bug in appearance configuration widget that was not updating the example model colors when changing the UI theme.<br/>
* Fixed a crash when double-clicking the overview widget.<br/>
* Fixed the data dictionaries schema files for tables and views.<br/>
* Fixed a bug in the database model that was causing FK relationships of a hidden layer to be displayed after loading the model.<br/>
* Fixed a bug in the scene move action that was causing the grid to not be displayed after a panning/wheel move.<br/>

**SQL session plugin:** This version introduces the SQL session plugin, available in the paid version of the tool, which implements simple routines to save the current opened SQL command execution sessions in a specific configuration file which can be restored in the next pgModeler execution by clicking the action "Restore SQL session", close to the connections combo box, in the Manage view.<br/>

Finally, for more detailed information about this release's changelog, please, refer to the CHANGELOG.md file.
