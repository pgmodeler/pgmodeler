# SQL definition for views
# PostgreSQL Version: 8.x, 9.0
# CAUTION: Do not modify this file unless that you know what
#          you are doing.

[-- object: ] @{name} [ | type: ] @{sql-object} [ -- ] $br

[CREATE OR REPLACE VIEW ] @{name} $br
[AS ] @{declaration}; $br

%if @{comment} %then @{comment} %end
