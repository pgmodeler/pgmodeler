# SQL definition for sequences
# PostgreSQL Version: 9.x
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

[-- object: ] @{name} [ | type: ] @{sql-object} [ --] $br

[CREATE SEQUENCE ] @{name} $br

%if @{increment} %then
 $tb [INCREMENT BY ] @{increment} $br
%end

%if @{min-value} %then
 $tb [MINVALUE ] @{min-value} $br
%end

%if @{max-value} %then
 $tb [MAXVALUE ] @{max-value} $br
%end

%if @{start} %then
 $tb [START WITH ] @{start} $br
%end

%if @{cache} %then
 $tb [CACHE ] @{cache} $br
%end

%if @{cycle} %then
  $tb CYCLE
%else
  $tb [NO CYCLE]
%end
$br

$tb [OWNED BY ]
%if @{owner} %then @{owner} %else NONE %end
; $br

%if @{comment} %then @{comment} %end

# This is a special token that pgModeler recognizes as end of DDL command
# when exporting models directly to DBMS. DO NOT REMOVE THIS TOKEN!
[-- ddl-end --] $br $br
