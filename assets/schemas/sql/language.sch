# SQL definition for procedural languages
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.

@include "ddlend"
@include "objlabel"
@include "prependedsql"

[CREATE OR REPLACE ]

%if {trusted} %then

	%if {handler} %then
		[TRUSTED ]
	%end
%end

[ LANGUAGE ] {name}

%if {handler} %then
	$br $tb [HANDLER ] {handler}
%end

%if {validator} %then
	$br [VALIDATOR ] {validator}
%end

%if {inline} %then
	$br [INLINE ] {inline}
%end

;

@include "footer"
