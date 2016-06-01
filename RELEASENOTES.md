v0.8.2
------

<em>Codename: <strong>Faithful Elephant</strong></em><br/>
<em>Release date: June 3, 2016</em><br/>
<em>Changes since: <strong>v0.8.2-beta1</strong></em><br/>

<strong>Summary:</strong> after two months of work we bring to you the pgModeler 0.8.2! With this release we officially concluded the series 0.8.x. This is by far the most important series in project's lifetime since the introduction of reverse engineering feature and port to Qt 5. This one brings additional improvements that definitively turned pgModeler into a solid application.<br/>

Several portions of the software were changed or fixed as the following:

1) SQL Tool: the command history now is part of the output widget having a better appearance by registering commands and their respective affected or retrieved rows and, for broken commands, the error raised in the moment of their execution. Still in SQL tool, the data manipulation dialog was improved enabling the user to duplicate rows making the data handling even more quick.<br/>

2) Connection timeouts: in previous versions in certain cases when the connection remained open for too long or the server unexpectedly closed it the application could crash. Now, pgModeler implements an internal connection timeout avoiding execution of SQL commands using an invalid connection, instead, an error is raised and the operation is aborted. This will greatly diminish these annoying failures.<br/>

3) Initial data for tables: in previous versions the user was capable to create custom INSERT commands in order to populate tables after their creation. Now, pgModeler implements a dedicated dialog for this purpose. It's an special grid, very similar to the data manipulation form in SQL tool, that enables data input. The magic here is that the data inserted in the grid is saved as a CSV-like buffer in the table and translated to INSERT commands when the table code is  generated and exported to the server or file. For now, the form is quite simple and surely will be improved in future releases.<br/>

4) Default connections: in response to some users' requests pgModeler now counts with default connections for each operation that involves export, import, diff and validation. In the moment a connection is being managed the user can check in which operation that connection will be used by default. This is useful when you have multiple databases for different phases of the project like in software factories (e.g. test, homolog, production and etc).<br/>

5) Transactional commands: in previous versions, due to implementation details, pgModeler was not capable of run transactional commands like START TRANSTION, COMMIT and ROLLBACK. Now, after some tweaks in the core, the user can freely create a transaction session, play around with the tables and commit
or roll back changes.<br/>

6) SVG support in export: this long awaited feature now lives in pgModeler. Models can be exported to SVG format opening the possibility to import them in another general purpose modeling tools or even serving as an additional option for documentation.<br/>

7) Miscelaneous: the metadata handling form was improved and simplified the process of extract and apply objects' metadata in the models. Source code in SQL tool's command input field now can have the case changed or indented by calling the context menu using right-click or specific shortcuts. The annoying error related to new format of connections.conf is now solved automatically when the application detects and older format being used. Several componets in the UI were resized improving usability, missing shortcuts added and many others.<br/>

Finally, the key bug fixes and changes are described below. The full list of news, changes and fixes is available in the CHANGELOG.md. <br/>

* [New] Added the ability to the table to create insert commands from the initial data buffer.
* [New] Added the ability to duplicate rows in DataManipulationForm.
* [New] Added the ability to clear and copy text from history to the sql command input field using middle mouse button in SQL tool.
* [New] Added the ability to set the default connection for operations import, export, diff and validation in ConnectionsConfigWidget.
* [New] Added the ability to save the current grid options to the pgmodeler.conf file.
* [New] Added support to export model to SVG file in UI and CLI.
* [New] Added the support to change case and identation of the selected text in NumberedTextEditor using context menu or shortcuts.
* [New] Connections now can have a timeout between command executions. When this timeout exceeds the next command is not executed. This is a workaround to avoid the crash of the program due to connections being (unexpectedly or not) closed by the server.
* [New] Added the ability to show connections notice/warning in SQL tool.
* [New] Added an step during the connections.conf loading to fix the connection timeout attribute automatically.
* [Change] Improved the metadata handling form. Now the user just need to choose from which model to extract the metadata and the form will do the rest (extract and apply) in one step.
* [Change] Minor improvement in ModelsDiffHelper in order to avoid the generation of useless SQL code (SET statments) when no effective changes were found in the process.
* [Change] Improvements on SQLExecutionWidget to enable the connection stay open in order to permit the usage of commands START TRANSATION, COMMIT and ROLLBACK.
* [Fix] Fix a bug in the Catalog class that was generating broken catalog queries for PostgreSQL releases under 9.3.
* [Fix] Fixed a bug in DataManipulationForm that was not quoting columns in the generated UPDATE commands.
* [Fix] Fixed a bug in import process related to permission creation. Now pgModeler removes extra backslash from role's name to avoid it not to be found in the model.
* [Fix] Fixed a bug that was generating broken table's SQL when the object has one or more inherited column.
* [Fix] Escaping properly columns' comments.
