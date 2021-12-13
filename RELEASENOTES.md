v0.9.4
------

<em>Release date: December 17, 2021</em><br/>
<em>Changes since: <strong>v0.9.3</strong></em><br/>

<strong>Attention:</strong> the database model file structure has changed since the last stable 0.9.3. Models created in older releases will certainly fail to load due to incompatibilities because some attributes in the XML code don't exist anymore or have changed during the development of 0.9.4. Before loading your database models in this new release, please, make sure to have a backup of them all and follow the steps presented by the model fix tool to patch the documents' structure. Not paying attention to this situation may cause irreversible data loss! If the fix procedures aren't enough to make your database models loadable again, please, ask for help at the official support channels! <br/>

<strong>Summary:</strong> it was quite a challenge to develop version 0.9.4 during the year 2021 but we finally made it, the last pgModeler of the series 0.9.x is ready! This one has a vast set of improvements over 0.9.3, being 51 new features, 104 enhancements, and 73 bug fixes that will make a big difference in the overall usage of the tool. <br/>

The first improvement in 0.9.4 is the ability to put objects in multiple layers which was not possible in previous versions. The user has now the ability to set up custom colors and labels for each layer enhancing the database model visualization and semantics. Still in the database model design, one can now define a custom color for canvas, grid lines, and page delimiter lines which can help a lot those who aren't comfortable with the excessive glare produced by the default color schema.<br/>

This release also improves the support for some PostgreSQL objects by bringing the configuration parameters and transform types to functions and procedures, adding the support for PARALLEL attribute on functions, and the support for included columns also known as non-key columns on indexes. It's worth mentioning that the import and diff processes were also improved to support all the mentioned improvements in those objects. Besides, the diff process is now capable of generating GRANT and REVOKE commands to set up new role memberships. <br/>

The data type configuration widget received a simple improvement which adds a significant reduction in the time spent to configure a data type. Now, the user can just type the name of the desired data type to be used instead of selecting it by using the mouse. After typing the data type name just hit the tab key to jump to the next field and the data type will be automatically configured. Seems a silly fix but it adds speed when you need to repeatedly create columns in a table, for example. <br/>

In the SQL tool, is now possible to drop databases quickly from the databases listing. Previously, there was the need to browse the database first and then drop it. In the database browser widget, the tree items collapse state is saved and restored after any updates requested by the user, improving the overall experience on that module. In data manipulation form, the sorting operation performed when clicking columns was adjusted. Now, the sorting will be triggered by holding the control key and clicking the desired column, and if the user only clicks a column without holding the control key will cause the entire column to be selected in the grid. <br/>

Attending to some requests, pgModeler is now capable of exporting the database model in split SQL files instead of generating a single SQL file representing the entire database. In this export mode, the files are named in such a way to represent the proper order of creation, which means that running the scripts one after another will create the whole database like it was created from a single file. <br/>

Due to the introduction of a new syntax highlighting configuration file (for code snippets configuration), pgModeler will, from now on, create missing configuration files at startup. This will avoid breaking the tool's execution when it detects the absence of any configuration file. Also, during the copy of these files, pgModeler will enforce the write permission over them to avoid access problems on some systems that store template settings on read-only paths. <br/>

The command-line interface was patched in such a way that now doesn't crash anymore when performing a diff operation in which a database model is used as input. Another improvement to the CLI is the introduction of the option --force in the mime type handling operation.  Also, a patch was made in such a way to avoid the usage of option '-platform offscreen' explicitly when calling the application. <br/>

Finally, there is a bunch of other new features and bug fixes implemented in almost all parts of the tool which improves stability and reliability. I kindly invite you to read the CHANGELOG.md file of the 0.9.4 development timeline to know everything in detail. <br/>
