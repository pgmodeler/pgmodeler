# SQL definition for encoding conversions
# PostgreSQL Version: 9.x
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.
[-- object: ] @{name} [ | type: ] @{sql-object} [ --] $br

CREATE %if @{default} %then [ DEFAULT] %end

[ CONVERSION ] @{name} $br
$tb [FOR ] '@{src-encoding}' [ TO ] '@{dst-encoding}' $br
$tb [FROM ] @{function}; $br

%if @{owner} %then @{owner} %end
%if @{comment} %then @{comment} %end

# This is a special token that pgModeler recognizes as end of DDL command
# when exporting models directly to DBMS. DO NOT REMOVE THIS TOKEN!
[-- ddl-end --] $br $br
