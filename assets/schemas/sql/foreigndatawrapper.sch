# SQL definition for foreign data wrappers
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.

@include "ddlend"
@include "objlabel"
@include "prependedsql"

[CREATE ] {sql-object} $sp {name} $br

%if %not {handler} %then
	[NO HANDLER]
%else
	[HANDLER ] {handler}
%end

$br

%if %not {validator} %then
	[NO VALIDATOR]
%else
	[VALIDATOR ] {validator}
%end

%if {options} %then
	$br [OPTIONS (] {options} )
%end

;

@include "footer"
