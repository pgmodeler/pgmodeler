v0.8.1
------
<em>Codename: <strong>Faithful Elephant</strong></em>
<em>Release date: July 30, 2015</em>
<em>Changes since: <strong>v0.8.0</strong></em><br/>

<strong>Summary:</strong> after five months working to enhance the last major release we finally have the 0.8.1! This version brings a lot of improvements and fixes in a total of 30 new features, 68 changes, 72 bug fixes.<br/>
The previous release (0.8.1) introduced important new feature and this one brings more improvements to them.<br/>
Obviously, not all new features or changes will be noted by the user because they were created to help in bug fixes but what matters is that pgModeler is continously being enhanced.<br/>
Below the key features of this new release are described. Please, refer to CHANGELOG.md to see the complete list of features and fixes.<br/>

* <strong>Faster design:</strong> now it is possible to create dependency objects from within their related selectors in editing forms. For instance, inside the table's editing form you can create a new schema and assing to the table on the fly just by poping up the schema's selector dialog and right click the item "Schemas (n)" and then selecting "New schema". This action is valid in almost all selectors in editing forms. Surely this will shorten up the time spent to create objects in the model.<br/>

* <strong>Automatic keyword quotation:</strong> introducing an automatic name quotation mechanism. This feature will add quotes to objects' names if they refer to PostgreSQL's reserved keywords. The automatic quotation will happen whenever the SQL code needs to be generated, this will avoid export or validation errors related to objects' names. <strong>NOTE:</strong> this feature is experimental and may fail in some cases.</br>

* <strong>Improved object finder:</strong> the object finder is a handful tool when working with big models. In previous releases the only action over results was to open the editing form of the selected object in the results grid. Now the user can access the complete menu (the same one in the canvas area) by right click any item in the result grid. <strong>NOTE:</strong> the items available in the menu may vary depending upon the selected object.<br/>

* <strong>Advanced code preview:</strong> the source code visualization now is able to generate code related to the object and its dependecies or children for test purposes. Also, there is the possibility to save the code in that dialog to a file for later usage.<br/>

* <strong>Multiple database management:</strong> in previous versions the user was able to manage only a database at a time. Now it's possible to manage several databases at once using different connections without the need to close the ones currently being managed.<br/>

* <strong>Patched diff process:</strong> the experimental diff feature was quite appreciated by the users and attending to some requests this feature received several fixes and a few new options like the one to preserve database names. This option will avoid attempts to rename a database when the model's name differs from the database's name.<br/>

* <strong>Improved import process:</strong> the reverse engineering process is now able to import a set of objects to an existing model. This is useful when the user needs to reuse objects from another database in the model currently being designed.<br/>

* <strong>Miscelaneous:</strong> fixed some crashes related to thread hang ups when validating or diff'ing large models; the CLI received important patches and the model fix process generates more reliable models; pgModeler received a numbered source code editor in its most relevant portions to facilitate the source code reading and editing; Added a "Contributors" section in the about dialog to list the persons who contributed with patches and features and much more.<br/>
