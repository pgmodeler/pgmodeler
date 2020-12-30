# SQL definition for type casts
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

[-- object: cast] ( {source-type} [,] {destiny-type} )  [ | type: ] {sql-object} [ --] $br
[-- ] {drop}

# This is a special token that pgModeler recognizes as end of DDL command
# when exporting models directly to DBMS. DO NOT REMOVE THIS TOKEN!
%set {ddl-end} $br [-- ddl-end --] $br

%if {prepended-sql} %then
 {prepended-sql}
 {ddl-end} $br
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
; 

{ddl-end}

%if {comment} %then {comment} %end

%if {appended-sql} %then
 {appended-sql}
 {ddl-end}
%end

$br
