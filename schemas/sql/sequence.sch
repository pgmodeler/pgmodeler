# SQL definition for sequences
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

[-- object: ] {name} [ | type: ] {sql-object} [ --] $br

[-- ] {drop}

 %if {prepended-sql} %then
   {prepended-sql}
   $br [-- ddl-end --] $br $br
 %end

[CREATE SEQUENCE ] {name} $br

%if {increment} %then
 $tb [INCREMENT BY ] {increment} $br
%end

%if {min-value} %then
 $tb [MINVALUE ] {min-value} $br
%end

%if {max-value} %then
 $tb [MAXVALUE ] {max-value} $br
%end

%if {start} %then
 $tb [START WITH ] {start} $br
%end

%if {cache} %then
 $tb [CACHE ] {cache} $br
%end

%if {cycle} %then
  $tb CYCLE
%else
  $tb [NO CYCLE]
%end
$br

$tb [OWNED BY ]
%if {owner-col} %then {owner-col} %else NONE %end
; $br

%if {owner-col} %and %not {col-is-identity} %then
$br
[ALTER TABLE ] {table} [ ALTER COLUMN ] {column} $br
[      SET DEFAULT nextval('] {name} ['::regclass);] $br
%end

# This is a special token that pgModeler recognizes as end of DDL command
# when exporting models directly to DBMS. DO NOT REMOVE THIS TOKEN!
[-- ddl-end --] $br

%if {owner} %then {owner} %end
%if {comment} %then {comment} %end

%if {appended-sql} %then
 {appended-sql}
 $br [-- ddl-end --] $br
%end

$br
