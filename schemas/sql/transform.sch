# SQL definition for transforms
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

[-- object: ] {name} [ | type: ] {sql-object} [ --] $br
[-- ] {drop}

# This is a special token that pgModeler recognizes as end of DDL command
# when exporting models directly to DBMS. DO NOT REMOVE THIS TOKEN!
%set ${ddl-end} [-- ddl-end --]

%if {prepended-sql} %then
 {prepended-sql}
 $br ${ddl-end} $br $br
%end

[CREATE TRANSFORM FOR ] {type} [ LANGUAGE ] {language} [ (] $br

%if {fromsql} %then
  $tb [FROM SQL WITH FUNCTION ] ${fromsql}
%end

%if {tosql} %then
  $tb [TO SQL WITH FUNCTION ] ${tosql}
%end

; $br

${ddl-end} $br

%if {comment} %then {comment} %end

%if {appended-sql} %then
 {appended-sql}
 $br ${ddl-end} $br
%end

$br
