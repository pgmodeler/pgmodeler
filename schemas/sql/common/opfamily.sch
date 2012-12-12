# SQL definition for operator family
# PostgreSQL Version: 8.x, 9.0
# CAUTION: Do not modify this file unless that you know what
#          you are doing.

[-- object: ] @{name} [ | type: ] @{sql-object} [ -- ] $br

[CREATE OPERATOR FAMILY ] @{name} [ USING ] @{index-type}; $br

%if @{owner} %then @{owner} %end
%if @{comment} %then @{comment} %end
 