# SQL definition for type casts
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

[-- object: cast] ( {source-type} [,] {destiny-type} )  [ | type: ] {sql-object} [ --] $br
[-- ] {drop}

%if {prepended-sql} %then
 {prepended-sql}
 $br [-- ddl-end --] $br $br
%end

[CREATE CAST (] {source-type} [ AS ] {destiny-type} ) $br

%if {io-cast} %then
  $tb [WITH INOUT ]
%else
  %if {function} %then
    $tb [WITH FUNCTION ] {function}
  %else
    $tb [WITHOUT FUNCTION]
  %end
%end

%if {cast-type} %then
 $br $tb [AS ] {cast-type}
%end
; $br

# This is a special token that pgModeler recognizes as end of DDL command
# when exporting models directly to DBMS. DO NOT REMOVE THIS TOKEN!
[-- ddl-end --] $br

%if {comment} %then {comment} %end

%if {appended-sql} %then
 {appended-sql}
 $br [-- ddl-end --] $br
%end

$br
