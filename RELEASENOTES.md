v0.8.0
------
<em>Codename: <strong>Faithful Elephant</strong></em>
<em>Release date: February 28, 2015</em>
<em>Changes since: <strong>v0.7.2</strong></em><br/>

<strong>Summary:</strong> after eight months working on create an even more improved product the final 0.8.0 is done. This version is the best one ever created because the software is more stable, reliable, useful and beautiful.<br/>
Compared to the previous release (0.7.2) this one has a lot more features that will surely improve the database design. Also, there are new features to help user in manage live databases.<br/>
To give an idea on how pgModeler has changed since 0.7.2, the full change log for the new release has 349 entries, being 85 new features, 147 changes and 117 bug fixes.<br/>
Obviously, not all new features or changes will be noted since they were created to help in bug fixes but what matters is that pgModeler is now a better product.<br/>
Below the key features of this new version are described. Please, refer to CHANGELOG.md to see the complete list of features and fixes.<br/>

* <strong>Optmized layout:</strong> the entire layout was improved, separating portions of the software in form of views (welcome, design and manage) giving users a better usage experience. Also, as part of layout improvements, the database model objects are now more clean and smooth being more pleasing to the eyes.<br/>

* <strong>Faster code generation:</strong> the SQL and XML code generation speed was increased by introducing the cached code feature. With this feature the loading, saving and export processes are more faster speeding up the productivity.<br/>

* <strong>Per-user configurations:</strong> pgModeler now is capable to store its settings separated for each user in the same system. This is quite handy when several users need to run the software with their own preferred settings without mess with others settings.<br/>

* <strong>Model database diff:</strong> create SQL scripts containing all commands needed to keep synchronized both model and database. With the diff feature you can make changes on a living database without drop it and recreate from the model that originate it.<br/>
Just do the needed changes on the model and compare it to the database, pgModeler will handle the rest creating DDL commands and storing them on a file or applying directly to a server. <strong>NOTE:</strong> this is an experimental feature so it may fail in some cases.<br/>

* <strong>Improved reverse engineering:</strong> the import process is even better. Free from a lot of bug of previous releases the import permits user to create partial models by importing only a few set of objects or the entire database as he wishes.<br/>
This process now is capable to solve automatically the majority of problems related to object's dependencies that previous versions could not handle.<br/>

* <strong>Improved SQL tool:</strong> being now a dedicated section of the software the SQL tool (manage view) is even better. Now its possible to handle several databases, manage table data through the data manipulation dialog and run SQL commands directly on the server.<br/>
In this release, SQL code snippets were introduced in order to help users to create and execute SQL commands when working on a database.<br/>

* <strong>Improved model validation:</strong> the main source of crashes in previous releases the validation process was carefully improved being more stable and reliable by fixing the majority of model inconsistencies that may appear during the design time.<br/>

* <strong>Improved model export:</strong> several bugs were fixed on the model export process. Additionally, user now have the option to run DROP commands before create an object directly on the database. This is useful when you do not want to drop the entire database in order to recreate the objects.<br/>

* <strong>Miscelaneous:</strong> several bug fixes throughout the subprojects, ability to drop objects in cascade way during design, several improvements on quick actions menu when two or more objects are selected, several improvements on the configuration sections and much more. <br/>
