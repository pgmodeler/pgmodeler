# SQL definition for operator family
# PostgreSQL Version: 9.x
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

[-- object: ] @{name} [ | type: ] @{sql-object} [ --] $br

[CREATE OPERATOR FAMILY ] @{name} [ USING ] @{index-type}; $br

%if @{owner} %then @{owner} %end
%if @{comment} %then @{comment} %end
 
# This is a special token that pgModeler recognizes as end of DDL command
# when exporting models directly to DBMS. DO NOT REMOVE THIS TOKEN!
[-- ddl-end --] $br $br
