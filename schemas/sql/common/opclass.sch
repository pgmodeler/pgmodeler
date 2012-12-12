# SQL definition for operator classes
# PostgreSQL Version: 8.x, 9.0
# CAUTION: Do not modify this file unless that you know what
#          you are doing.

[-- object: ] @{name} [ | type: ] @{sql-object} [ -- ] $br

[CREATE OPERATOR CLASS ] @{name} 
%if @{default} %then [ DEFAULT ] %end

[FOR TYPE ] @{type} $br
[USING ] @{index-type} 

%if @{family} %then [ FAMILY ] @{family} %end
[ AS] $br

@{elements}; $br

%if @{owner} %then @{owner} %end
%if @{comment} %then @{comment} %end

