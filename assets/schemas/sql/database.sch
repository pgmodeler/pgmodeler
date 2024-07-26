# SQL definition for databases
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.

@include "ddlend"
@include "objlabel"
@include "prependedsql"

[CREATE DATABASE ] {name}

%if {template} %then
	$br $tb [TEMPLATE = ] {template}
%end

%if {encoding} %then
	$br $tb [ENCODING = ] {encoding}
%end

%if {lc-collate} %then
	$br $tb [LC_COLLATE = ] {lc-collate}
%end

%if {lc-ctype} %then
	$br $tb [LC_CTYPE = ] {lc-ctype}
%end

%if {tablespace} %then
	$br $tb [TABLESPACE = ] {tablespace}
%end

%if {owner} %then
	$br $tb [OWNER = ] {owner}
%end

%if {connlimit} %then
	$br $tb [CONNECTION LIMIT = ] {connlimit}
%end

%if ({is-template} == "true") %then
	$br $tb [IS_TEMPLATE = ] {is-template}
%end

%if ({allow-conns} == "false") %then
	$br $tb [ALLOW_CONNECTIONS = ] {allow-conns}
%end
;

@include "footer"
