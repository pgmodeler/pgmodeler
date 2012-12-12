# SQL definition for aggregate functions
# PostgreSQL Version: 8.1
# CAUTION: Do not modify this file unless that you know what
#          you are doing.

[-- object: ] @{name} [ | type: ] @{sql-object} [ -- ] $br

[CREATE AGGREGATE ] @{name} [ (] $br
$tb [BASETYPE = ] @{types}, $br
$tb [SFUNC = ] @{transition}, $br
$tb [STYPE = ] @{state-type} $br
%if @{final} %then $tb [,FINALFUNC = ] @{final} $br %end
%if @{initial-cond} %then $tb [,INITCOND = ] @{initial-cond} $br %end
%if @{sort-op} %then $tb [,SORTOP = ] @{sort-op} $br %end
); $br

%if @{owner} %then @{owner} %end
%if @{comment} %then @{comment} %end
