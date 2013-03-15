# SQL definition for operators
# PostgreSQL Version: 8.1
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

[-- object: ] @{name} [ | type: ] @{sql-object} [ -- ] $br

[CREATE OPERATOR ] @{name} [ (]
$br $tb [PROCEDURE = ] @{operfunc}

%if @{left-type} %then
 $br $tb [, LEFTARG = ] @{left-type}
%end

%if @{right-type} %then
 $br $tb [, RIGHTARG = ] @{right-type}
%end

%if @{commutator-op} %then
 $br $tb [, COMMUTATOR = ] OPERATOR(@{commutator-op})
%end

%if @{negator-op} %then
 $br $tb [, NEGATOR = ] OPERATOR(@{negator-op})
%end

%if @{restriction} %then
 $br $tb [, RESTRICT = ] @{restriction}
%end

%if @{join} %then
 $br $tb [, JOIN = ] @{join}
%end

%if @{hashes} %then
 $br $tb [, HASHES ]
%end

%if @{merges} %then
 $br $tb [, MERGES ]
%end

%if @{sort-op} %then
 $br $tb [, SORT1 = ] @{sort-op}
%end

%if @{sort2-op} %then
 $br $tb [, SORT2 = ] @{sort2-op}
%end

%if @{less-op} %then
 $br $tb [, LTCMP = ] @{less-op}
%end

%if @{greater-op} %then
 $br $tb [, GTCMP = ] @{greater-op}
%end

); $br

%if @{owner} %then @{owner} %end
%if @{comment} %then @{comment} %end

# This is a special token that pgModeler recognizes as end of DDL command
# when exporting models directly to DBMS. DO NOT REMOVE THIS TOKEN!
[-- ddl-end --] $br $br