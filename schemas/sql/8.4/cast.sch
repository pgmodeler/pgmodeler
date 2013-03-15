# SQL definition for type casts
# PostgreSQL Version: 8.4
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

[-- object: cast] ( @{source-type} [,] @{destiny-type} )  [ | type: ] @{sql-object} [ -- ] $br

[CREATE CAST (] @{source-type} [ AS ] @{destiny-type} ) $br

%if @{io-cast} %then
  $tb [WITH INOUT]
%else

 %if @{signature} %then
  $tb [WITH FUNCTION ] @{signature}
 %else
  $tb [WITHOUT FUNCTION]
 %end

%end

%if @{cast-type} %then
 $br $tb [AS ] @{cast-type}
%end
; $br

%if @{comment} %then @{comment} %end

# This is a special token that pgModeler recognizes as end of DDL command
# when exporting models directly to DBMS. DO NOT REMOVE THIS TOKEN!
[-- ddl-end --] $br $br