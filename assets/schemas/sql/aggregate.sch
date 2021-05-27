# SQL definition for aggregate functions
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

[-- object: ] {name} [ | type: ] {sql-object} [ --] $br
[-- ] {drop}

# This is a special token that pgModeler recognizes as end of DDL command
# when exporting models directly to DBMS. DO NOT REMOVE THIS TOKEN!
%set {ddl-end} $br [-- ddl-end --] $br

%if {prepended-sql} %then
 {prepended-sql}
 {ddl-end} $br
%end

[CREATE AGGREGATE ] {name} [ (]{types}[) (] $br
$tb [SFUNC = ] {transition}, $br
$tb [STYPE = ] {state-type} $br
%if {final} %then $tb [,FINALFUNC = ] {final} $br %end
%if {initial-cond} %then $tb [,INITCOND = ] {initial-cond} $br %end
%if {sort-op} %then $tb [,SORTOP = ] {sort-op} $br %end
); 

{ddl-end}

%if {owner} %then {owner} %end
%if {comment} %then {comment} %end

%if {appended-sql} %then
 {appended-sql}
 {ddl-end}
%end

$br
