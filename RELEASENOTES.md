v0.8.2
------

<em>Codename: <strong>Faithful Elephant</strong></em><br/>
<em>Release date: June 3, 2016</em><br/>
<em>Changes since: <strong>v0.8.2-beta1</strong></em><br/>

<strong>Summary:</strong> after two months of work we bring to you the 0.8.2! With this release we officially concluded the series 0.8.x. This is by far the most important series in project's lifetime since the times of reverse engineering introduction and port to Qt 5. This version brings additional improvements that definitively turned pgModeler into a solid application. <br/>

Several portions of the software were improved or fixed as the following:

1) SQL Tool: the command history now is part of the output grid (at bottom of the widget) and now have a better appearance by registering commands and their respective affected or retrieved rows and, for broken commands, the error raised in the moment of their execution. Still in SQL tool, the data manipulation dialog was improved enabling the user to duplicate rows making the data handling even more quick.<br/>

2) Connection timeouts: in previous versions in certain cases when the connection remained open for too long or the server unexpectedly closed it the application could crash. Now, pgModeler implements an internal connection timeout avoiding execution of SQL commands using an invalid connection, instead an error is raised and the operation aborted. This will greatly diminish these annoying crashes.<br/>

3) Initial table data: in previous versions the user was capable to create custom INSERT commands to populate tables after their creation. Now, pgModeler implements a dedicated form for this purpose. It's an special grid, very similar to the data manipulation form in SQL tool, that enables data input. The magic here is that the data inserted in the grid is saved as a CSV-like buffer in the table and translated to INSERT commands when the table code is exported to the server or file.<br/>

4) Connection configuration:

5) Transactional commands:

6) Model export:

7) Miscelaneous:

Finally, key bug fixes and changes are described below. The full list of news, changes and fixes is available in the CHANGELOG.md. <br/>

* []


