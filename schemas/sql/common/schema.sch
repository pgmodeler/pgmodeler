# SQL definition for schemas
# PostgreSQL Version: 8.x, 9.0
# CAUTION: Do not modify this file unless you know what
#          you are doing.

[-- object: ] @{name} [ | type: ] @{sql-object} [ -- ] $br

[CREATE SCHEMA ] @{name}; $br
%if @{owner} %then @{owner} %end
%if @{comment} %then @{comment} %end
 