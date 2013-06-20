# SQL definition for operator classes
# PostgreSQL Version: 9.x
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

[-- object: ] @{name} [ | type: ] @{sql-object} [ --] $br

[CREATE OPERATOR CLASS ] @{name} 
%if @{default} %then [ DEFAULT ] %end

[ FOR TYPE ] @{type} $br
[ USING ] @{index-type}

%if @{family} %then [ FAMILY ] @{family} %end
[ AS] $br

@{elements}; $br

%if @{owner} %then @{owner} %end
%if @{comment} %then @{comment} %end
%if @{appended-sql} %then @{appended-sql} %end

# This is a special token that pgModeler recognizes as end of DDL command
# when exporting models directly to DBMS. DO NOT REMOVE THIS TOKEN!
[-- ddl-end --] $br $br
