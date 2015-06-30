v0.8.1-beta1
------
<em>Codename: <strong>Faithful Elephant</strong></em>
<em>Release date: June 30, 2015</em>
<em>Changes since: <strong>v0.8.1-beta</strong></em><br/>

<strong>Summary:</strong> this release brings a small set of fixes and just two new features meaning that we are close to the final 0.8.1.

A new feature introduced is the ability to rename objects in database explorer by using F2 or 'Rename' action in object's popup menu avoiding the usage of DDL commands for this task.

Another feature is a basic Spanish UI translation that still demands some review but is quite complete and for those who speak that language can take more advantage of pgModeler's features.

Good news for Mac OS X users: the 'startapp' script was dropped and all executables are able to find their dependencies and can be run from terminal without the use of the script.

Finally, small fixes were done in different portions of the software as seen below:

* [New] Objects can be now renamed in database explorer except for databases and casts.
* [New] Added an initial Spanish (es_ES) UI translation (review needed).
* [Change] Dropped the startapp script in Mac OSX. All executables now are able to run without explicitly use that script.
* [Fix] Fixed the snippet related to object renaming in snippets.conf.
* [Fix] Fixed a small bug in schema parser that was ignoring the usage of '$oc' metacharacter in some cases.
* [Fix] Fixed shortcut for delete command in data manipulation form.
* [Fix] Fixed the deployment script on Mac OSX to make the CLI and crash handler find the core libraries properly.
* [Fix] Fixed a bug that was generating ALTER ROLE commands without the semicolon.
