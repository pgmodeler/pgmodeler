# SQL definition for domains
# PostgreSQL Version: 8.x, 9.0
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

[-- object: ] @{name} [ | type: ] @{sql-object} [ -- ] $br

[CREATE DOMAIN ] @{name} [ AS ] @{type}
%if @{default-value} %then $br $tb [DEFAULT ] @{default-value} %end
%if @{constraint} %then $br $tb [CONSTRAINT ] @{constraint} %end
%if @{not-null}  %then $br $tb [NOT NULL] %end
%if @{expression} %then  $br $tb [CHECK ] (@{expression}) %end
;$br

%if @{owner} %then @{owner} %end
%if @{comment} %then @{comment} %end

# This is a special token that pgModeler recognizes as end of DDL command
# when exporting models directly to DBMS. DO NOT REMOVE THIS TOKEN!
[-- ddl-end --] $br $br