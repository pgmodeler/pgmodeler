# SQL definition for foreign server
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.

@include "ddlend"
@include "objlabel"
@include "prependedsql"

[CREATE SERVER ] {name}

%if {type} %then
	[ TYPE ] '{type}'
%end

%if {version} %then
	[ VERSION ] '{version}'
%end

$br [FOREIGN DATA WRAPPER ] {fdw}

%if {options} %then
	$br [OPTIONS (] {options} )
%end

;

@include "footer"
