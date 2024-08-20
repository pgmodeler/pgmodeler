# SQL definition for domains
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.

@include "ddlend"
@include "objlabel"
@include "prependedsql"

[CREATE DOMAIN ] {name} [ AS ] {type}

%if {collation} %then
	$br $tb [COLLATE ] {collation}
%end

%if {default-value} %then
	$br $tb [DEFAULT ] {default-value}
%end

%if {not-null} %then
	$br $tb [NOT NULL]
%end

%if {constraints} %then
	{constraints}
%end

;

@include "footer"
