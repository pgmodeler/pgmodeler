# SQL definition for views
# PostgreSQL Version: 9.x
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

[-- object: ] @{name} [ | type: ] @{sql-object} [ -- ] $br

[CREATE OR REPLACE VIEW ] @{name} $br
[AS ]

#Commom table expression (CTE)
%if @{cte-exp} %then
 [WITH ] @{cte-exp}
%end

@{declaration}; $br

# This is a special token that pgModeler recognizes as end of DDL command
# when exporting models directly to DBMS. DO NOT REMOVE THIS TOKEN!
[-- ddl-end --] $br $br

%if @{triggers} %then @{triggers} %end
%if @{rules} %then @{rules} %end
%if @{comment} %then @{comment} %end

