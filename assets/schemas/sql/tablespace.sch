# SQL definition for tablespaces
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

[-- object: ] {name} [ | type: ] {sql-object} [ --] $br
[-- ] {drop}

# This is a special token that pgModeler recognizes as end of DDL command
# when exporting models directly to DBMS. DO NOT REMOVE THIS TOKEN!
%set {ddl-end} $br [-- ddl-end --] $br

%if {prepended-sql} %then
   {prepended-sql}
   {ddl-end} $br
%end

[CREATE TABLESPACE ] {name} $br

%if {owner} %then
$tb [OWNER ] {owner} $br
%end

$tb [LOCATION ] {directory}; $br

{ddl-end}

%if {comment} %then {comment} %end

%if {appended-sql} %then
 {appended-sql}
 {ddl-end}
%end

$br
