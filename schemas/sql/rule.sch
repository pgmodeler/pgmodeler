# SQL definition for rules
# PostgreSQL Version: 9.x
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

[-- object: ] @{name} [ | type: ] @{sql-object} [ -- ] $br

[CREATE OR REPLACE RULE ] @{name} [ AS ] @{event-type} $br
$tb [TO ] @{table} $br

%if @{condition} %then
$tb [WHERE ] @{condition} $br
%end

$tb [DO ] @{exec-type} $sp
%if @{commands} %then
  (@{commands})
%else
  NOTHING
%end
;$br

%if @{comment} %then @{comment} %end

# This is a special token that pgModeler recognizes as end of DDL command
# when exporting models directly to DBMS. DO NOT REMOVE THIS TOKEN!
[-- ddl-end --] $br $br
