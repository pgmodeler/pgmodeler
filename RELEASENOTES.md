v0.8.2
------

<em>Codename: <strong>Faithful Elephant</strong></em><br/>
<em>Release date: June 6, 2016</em><br/>
<em>Changes since: <strong>v0.8.2-beta1</strong></em><br/>

<strong>Summary:</strong> after two months of work we bring to you the pgModeler 0.8.2! With this release we officially concluded the series 0.8.x. This is by far the most important series in project's lifetime since the introduction of reverse engineering feature and port to Qt 5. This one brings additional improvements that definitively turned pgModeler into a solid application.<br/>

Several portions of the software were changed or fixed in this final release as the following:

* <strong>SQL Tool:</strong> the command history now is part of the output widget having a better appearance by registering commands and their respective affected or retrieved rows and, for broken commands, the error raised in the moment of their execution. Still in SQL tool, the data manipulation dialog was improved enabling the user to duplicate rows making the data handling even more quick.<br/>

* <strong>Connection timeouts:</strong> in previous versions in certain cases when the connection remained open for too long or the server unexpectedly closed it the application could crash. Now, pgModeler implements an internal connection timeout avoiding execution of SQL commands using an invalid connection, instead, an error is raised and the operation is aborted. This will greatly diminish these annoying failures.<br/>

* <strong>Initial data for tables:</strong> in previous versions the user was capable to create custom INSERT commands in order to populate tables after their creation. Now, pgModeler implements a dedicated dialog for this purpose. It's an special grid, very similar to the data manipulation form in SQL tool, that enables data input. The magic here is that the data inserted in the grid is saved as a CSV-like buffer in the table and translated to INSERT commands when the table code is  generated and exported to the server or file. For now, the form is quite simple and surely will be improved in future releases.<br/>

* <strong>Default connections:</strong> in response to some users' requests pgModeler now counts with default connections for each operation that involves export, import, diff and validation. In the moment a connection is being managed the user can check in which operation that connection will be used by default. This is useful when you have multiple databases for different phases of the project like in software factories (e.g. test, homolog, production and etc).<br/>

* <strong>Transactional commands:</strong> in previous versions, due to implementation details, pgModeler was not capable of run transactional commands like START TRANSTION, COMMIT and ROLLBACK. Now, after some tweaks in the core, the user can freely create a transaction session, play around with the tables and commit
or roll back changes.<br/>

* <strong>SVG support in export:</strong> this long awaited feature now lives in pgModeler. Models can be exported to SVG format opening the possibility to import them in another general purpose modeling tools or even serving as an additional option for documentation.<br/>

* <strong>Miscelaneous:</strong> the metadata handling form was improved and simplified the process of extract and apply objects' metadata in the models. Source code in SQL tool's command input field now can have the case changed or indented by calling the context menu using right-click or specific shortcuts. The annoying error related to new format of connections.conf is now solved automatically when the application detects and older format being used. Several componets in the UI were resized improving usability, missing shortcuts added and many others.<br/>

Finally, in order to give an ideia on how much pgModeler changed since 0.8.1, we have 273 changelog entries divided in: 63 new features, 97 changes/improvements, 113 bug fixes. Obviously, not all changes will be noticeable since they occurred in the core, but what is more important is that if you're running 0.8.1 it is strongly recommended that you upgrade to 0.8.2 to take advantage of all new features and fixes. Also, to get full details about all modifications in the software since 0.8.1 take a look in CHANGELOG.md.
