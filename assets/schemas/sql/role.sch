# SQL definition for type roles
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.

@include "ddlend"
@include "objlabel"
@include "prependedsql"

[CREATE ROLE ] {name}

%if {has-options} %then
	[ WITH ]
	%if {superuser} %then $br $tb SUPERUSER %end
	%if {createdb} %then $br $tb CREATEDB %end
	%if {createrole} %then $br $tb CREATEROLE %end
	%if {inherit} %then $br $tb INHERIT %end
	%if {login} %then $br $tb LOGIN %end
	%if {replication} %then $br $tb REPLICATION %end
	%if {bypassrls} %then $br $tb BYPASSRLS %end

	%if {password} %then	$br $tb	[ PASSWORD ] '{password}' %end
	%if {connlimit} %then $br $tb [CONNECTION LIMIT ] {connlimit} %end
	%if {validity} %then $br $tb [VALID UNTIL ] '{validity}' %end

	%if {member-roles} %then $br $tb [ROLE ] {member-roles} %end
	%if {admin-roles} %then $br $tb [ADMIN ] {admin-roles} %end
%end
;

@include "footer"
