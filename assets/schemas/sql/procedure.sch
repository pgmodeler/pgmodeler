# SQL definition for procedures
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.

%if ({pgsql-ver} >=f "11.0") %then
	@include "ddlend"
	@include "objlabel"
	@include "prependedsql"

	[CREATE OR REPLACE PROCEDURE ] {name} $sp ( %if {parameters} %then {parameters} %end ) $br
	$tb [LANGUAGE ] {language} $br

	%if {transform-types} %then
		$tb TRANSFORM {transform-types} $br
	%end

	$tb {security-type} $br

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
		$br [$function$]

		$br {definition} 

		$br [$function$]
	%end

	;

	@include "footer"
%end
