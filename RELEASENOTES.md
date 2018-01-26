v0.9.1-alpha1
------

<em>Release date: January 26, 2018</em><br/>
<em>Changes since: <strong>v0.9.1-alpha1</strong></em><br/>

<strong>Summary:</strong> this is the first release of 2018 and the penultimate until we get the stable 0.9.1. We can consider it more a patch release than a fully featured version since it does not brings substantial changes to pgModeler but fixing some minor bugs detected in prior versions. <br/>

In this version we've started to adopt Qt 5.9.x LTS and PostgreSQL 10.x. The code still builds in prior versions of both Qt and PostgreSQL but we strongly recommend that the users always use the same version as the development environment to avoid any headache related to build problems. <br/>

We've added the support to GROUP BY/HAVING clauses in views. Actually, we've added the ability to append any kind of expression at the end of view's definition using expressions but the main usage for it is for GROUP/HAVING statements. Prior to that, there was the need to make some hacks in the view's references like adding a WHERE TRUE expression to enable the usage of GROUP/HAVING at the end of the code which is pretty ugly. <br/>

In the design module, we have fixed the copy/paste operation and now it copies almost any object in the canvas without much problem. The name conflict resolution was adjusted in such way that a desambiguation suffix will be appended to the pasted object's name when there really is the need and not always the paste operation is executed. The export to SVG and PNG was fixed too and now no unecessary blank areas are exported to the output file making the resulting graphics more compact.<br/>

Finally, some crashes related to attributes adding in many-to-many relationships as well to import operation were fixed.<br/>

Below the log entries for this version. Consider to read the complete CHANGELOG.md to take note of all changes.<br/>

* [New] Added support to GROUP BY/HAVING clauses in Views by adding a new kind of reference. Proper changes done in ViewWidget to allow configuring those clauses.
* [New] Added the method Catalog::isSystemObject(oid) which indicates if the provided OID is related to a system object.
* [Change] Minor adjustment in the copy/paste operation to generate suffix in the pasted objects only when there're conflics.
* [Change] Removed the port range limitation in connection configuration dialog.
* [Change] Updated the default version of Qt and PostgreSQL to, respectively, 5.9.3 and 10.1 in deployment scripts.
* [Change] Changed the method PgSQLType::getTypeName by adding a bool parameter so the name can be returned with dimension descriptor (when dimension is > 0). Useful for configuring operator's signatures.
* [Fix] Fixed the drop action for materialized views in database explorer.
* [Fix] Fixed a crash when importing extension objects.
* [Fix] Fixed the generation of operator's signature that must consider dimensions of the arguments' types.
* [Fix] Fixed the bounding rect calculation for relationship instances when one or more labels are hidden.
* [Fix] Fixed the SVG & PNG export to properly determine the area to be drawn in the destination graphics file.
* [Fix] Fixed a crash when adding attributes into many-to-many relationships.
