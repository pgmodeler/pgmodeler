v1.1.0-beta
------
<em>Release date: November 17, 2023</em><br/>
<em>Changes since: <strong>v1.1.0-alpha1</strong></em><br/>

<em><strong>Attention:</strong> Some configuration files were changed in pgModeler 1.1.0-beta causing a break in backward compatibility with pgModeler 1.0.x settings. This way, at the first start of the newer version, pgModeler will try to migrate the older settings to the newer ones automatically!</em><br/><br/>

Almost two months after launching 1.1.0-alpha1, I'm pleased to announce 1.1.0-beta! This version is the last of this development cycle to receive new features, and from now on, until the release of the stable 1.1.0 only bug fixes will be done. My main focus on this release was to work on two things that always bothered me since the early days of pgModeler: the view creation process and the extension data types handling. Since the codebase was mature enough, I thought it was time to change those two aspects of the tool, and this was done as described below:<br/>

**Views creation is now way simpler:** Instead of that clunky interface to configure views on previous versions, now the user can create this kind of object by using freely typed SQL commands with special placeholder variables enclosed by {} that we call references. Any reference in the typed SQL command that defines the view will be replaced by the referenced object, which can be columns, functions, procedures, tables, foreign tables, and views (yeah, views!). Once the view is created, pgModeler will create relationships between the referenced tables (foreign tables, and views) and the new view. Of course, not everything is roses, the feature is not backward compatible with models designed in the previous version, which means if you have models containing views you'll need to make manual repairs since the pgmodeler-cli fix process can't make the proper corrections. In that case, please, follow the instructions in the section "Fixing views' structure in pgModeler 1.1.0-beta+" of this release's blog post.<br/>

**Extensions can now have multiple child data types:** In previous releases, pgModeler had a special flag in the extension's editing form labeled "Handles data type". That flag served to inform pgModeler that the extension needed to be used as a data type, for example, creating an extension named "hstore" and checking the mentioned flag, would create the type "hstore" making it usable in the columns, function parameters, and so on. The problem with this approach is that if an extension installed more than one data type in the database, it was needed to make some workarounds to have a second type available to be used in the database modeling process. So, in pgModeler 1.1.0-beta, the "Handles data type" flag was ditched and now the user can specify a free number of data types that the extension handles. pgModeler will handle the data types when adding or removing the extension. Models that use the old extension format can be fixed by using the pgmodeler-cli model fix process.<br/>

**Improved tool's executables relocation:** pgModeler already has some mechanisms to customize the paths associated with the assets and executables once installed in the system. One of them is the environment variables, but sometimes the user doesn't want or even has no privileges to change environment variables in the system. Thinking of that, this new version introduces a special configuration file called "pgmpaths.conf" which goal is to configure the paths where the pgModeler main executable as well as the auxiliary tools can find all the needed folders, assets, and configuration files. This file must be created in the same folder as a pgModeler's executable and must be filled with lines in the format variable=path. The "variable" refers to one of the available environment variables understood by pgModeler (refer to the installation instructions, section "Environment variables" for details) and the "path" is a relative or absolute path to the resource associated with the environment variable.<br/>

**Several other improvements:** General improvements were made all over the tool and some of them are described below.<br/>
* Added support for overriding the canvas' background color when exporting the model to PNG.
* The "Display unique results" option on objects' dependencies & references dialog is now checked by default.
* Adjusted the CSV pasting in the table data editor.
* Adjusted the extension's attributes display in the database explorer to list types related to an extension.
* The code completion widget now resizes according to the displayed items' width.
* The code completion will not display a "no items found" popup if no element is found matching the word at the cursor's position.
* Adjusted the reverse engineering process so relationships can be created from the link between two views.
* Minor change in reverse engineering to avoid importing extension child types into the model since the extension itself, when imported, already creates the types.

**Bug fixes:** Also, as part of the constant search for the overall tool's stability and reliability, almost twenty bugs were fixed, and below we highlight some key ones:<br/>
* Fixed settings storing for the grid options in MainWindow.
* Fix a crash that was happening only on Windows.
* Fixed a bug in the generation of diff commands for identity columns.
* Fixed a bug in list widget items painting that was causing the rendering of artifacts sometimes.
* Fixed a bug in pgmodeler-cli that was aborting the fix process during the parsing of the model changelog.
* Fixed a crash when trying to load an invalid model from the recent model's menu.
* Fixed sample model structure to the new view's format.
* Fixed several bugs in the code completion widget when completing code using live database object names.

Finally, for more details about the version's changelog, please, take a look at the file CHANGELOG.md.
