# SQL definition for user mapping
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.

@include "ddlend"
@include "objlabel"
@include "prependedsql"

[CREATE USER MAPPING FOR ]

%if {owner} %then {owner} %else PUBLIC %end $br

[SERVER ] {server}

%if {options} %then
	$br [OPTIONS (] {options} )
%end

;

@include "footer"
