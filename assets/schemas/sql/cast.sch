# SQL definition for type casts
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.

@include "ddlend"
@include "objlabel"
@include "prependedsql"

[CREATE CAST (] {source-type} [ AS ] {destiny-type} ) $br

%if {io-cast} %then
	$tb [WITH INOUT ]
%else
	%if {function} %then
		$tb [WITH FUNCTION ] {function}
	%else
		$tb [WITHOUT FUNCTION]
	%end
%end

%if {cast-type} %then
	$br $tb [AS ] {cast-type}
%end

;

@include "footer"
