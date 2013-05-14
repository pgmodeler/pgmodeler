# SQL definition for indexes
# PostgreSQL Version: 9.x
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

[-- object: ] @{name} [ | type: ] @{sql-object} [ --] $br

[CREATE ] %if @{unique} %then [UNIQUE ] %end [INDEX ]

%if @{concurrent} %then
 [ CONCURRENTLY ]
%end

@{name} [ ON ] @{table}

%if @{index-type} %then
  $br $tb [USING ] @{index-type}
%end

$br $tb ( @{elements} $br $tb )

%if @{expression} %then
  $tb (@{expression}) $sp 
%end

%if @{stg-params} %then
 $tb [WITH (]

 %if @{factor} %then
  [FILLFACTOR = ] @{factor}
 %end

 %if @{fast-update} %then
  [FASTUPDATE = ON]
 %end

[)]
%end

%if @{tablespace} %then
 $br $tb [TABLESPACE ] @{tablespace}
%end

%if @{condition} %then
 $br $tb [WHERE (] @{condition} [)]
%end
; $br

%if @{comment} %then  @{comment} %end

# This is a special token that pgModeler recognizes as end of DDL command
# when exporting models directly to DBMS. DO NOT REMOVE THIS TOKEN!
[-- ddl-end --] $br $br
