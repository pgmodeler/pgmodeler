# SQL definition for operators
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

[CREATE OPERATOR ] {name} [ (]
$br $tb [PROCEDURE = ] {operfunc}

%if {left-type} %then
 $br $tb [, LEFTARG = ] {left-type}
%end

%if {right-type} %then
 $br $tb [, RIGHTARG = ] {right-type}
%end

%if {commutator-op} %then
 $br $tb [, COMMUTATOR = ] OPERATOR({commutator-op})
%end

%if {negator-op} %then
 $br $tb [, NEGATOR = ] OPERATOR({negator-op})
%end

%if {restriction} %then
 $br $tb [, RESTRICT = ] {restriction}
%end

%if {join} %then
 $br $tb [, JOIN = ] {join}
%end

%if {hashes} %then
 $br $tb [, HASHES ]
%end

%if {merges} %then
 $br $tb [, MERGES ]
%end

); 

{ddl-end}

%if {owner} %then {owner} %end
%if {comment} %then {comment} %end

%if {appended-sql} %then
 {appended-sql}
 {ddl-end}
%end

$br
