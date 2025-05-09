# SQL definition for extensions
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.

@include "ddlend"
@include "objlabel"
@include "prependedsql"

[CREATE EXTENSION ] {name} $br

%if {schema} %then
	[WITH SCHEMA ] {schema}
%end

%if {cur-version} %then
	$br [VERSION ] '{cur-version}'
%end

%if {old-version} %then
	$br [FROM ] '{old-version}'
%end

;

@include "footer"