# SQL definition for indexes
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

[CREATE ] %if {unique} %then [UNIQUE ] %end [INDEX ]

%if {concurrent} %then
 [ CONCURRENTLY ]
%end

{name} [ ON ] {table}

%if {index-type} %then
  $br $tb [USING ] {index-type}
%end

$br $tb ( {elements} $br $tb )

%if {expression} %then
  $tb ({expression}) $sp 
%end

%if {stg-params} %then
 $br $tb [WITH (]

 %if {factor} %then
  [FILLFACTOR = ] {factor}
 %end

 %if {fast-update} %then
  %if {factor} %then [, ] %end
  [FASTUPDATE = ON]
 %end

 %if {buffering} %and ({pgsql-ver} >=f "9.2") %then
  %if {factor} %then [, ] %end
  [BUFFERING = ON]
 %end

[)]
%end

%if {tablespace} %then
 $br $tb [TABLESPACE ] {tablespace}
%end

%if {predicate} %then
 $br $tb [WHERE (] {predicate} [)]
%end
;

{ddl-end}

%if {comment} %then {comment} %end

%if {appended-sql} %then
 {appended-sql}
 {ddl-end}
%end

$br
