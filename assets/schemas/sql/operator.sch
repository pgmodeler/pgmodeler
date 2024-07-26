# SQL definition for operators
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.

@include "ddlend"
@include "objlabel"
@include "prependedsql"

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

@include "footer"
