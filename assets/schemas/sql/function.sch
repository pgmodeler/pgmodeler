# SQL definition for functions
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.

@include "ddlend"
@include "objlabel"
@include "prependedsql"

[CREATE OR REPLACE FUNCTION ] {name} $sp ( %if {parameters} %then {parameters} %end ) $br
$tb [RETURNS ]

%if {return-table} %then
	[TABLE ] ( {return-table} )
%else
	%if {returns-setof} %then [SETOF ] %end
	{return-type}
%end

$br


$tb [LANGUAGE ] {language} $br

%if {transform-types} %then
	$tb TRANSFORM {transform-types} $br
%end

%if {window-func} %then
	$tb WINDOW $br
%end

$tb {function-type} $sp %if {leakproof} %then LEAKPROOF %end $br

$tb {behavior-type} $br
$tb {security-type} $br
$tb {parallel-type} $br

$tb [COST ] {execution-cost} $br

%if {returns-setof} %then
	$tb [ROWS ] {row-amount} $br
%end

%if {config-params} %then
	{config-params}
%end

$tb [AS ]

%if {library} %then
	'{library}'

	%if {symbol} %then
		[, ] '{symbol}'
	%end
%else
	%if ({language} == "internal") %then
		'{definition}'
	%else
		$br [$function$]

		$br {definition} 

		$br [$function$]
	%end
%end

;

@include "footer"
