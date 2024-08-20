# SQL definition for indexes
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.

@include "ddlend"
@include "objlabel"
@include "prependedsql"

[CREATE ] %if {unique} %then [UNIQUE ] %end [INDEX ]

%if {concurrent} %then
	[ CONCURRENTLY ]
%end

{name} [ ON ] {table}

%if {index-type} %then
	$br [USING ] {index-type}
%end

$br ( {elements} $br )

%if {expression} %then
	$br ({expression}) $sp
%end

%if ({pgsql-ver} >=f "15.0") %and {nulls-not-distinct} %then
	$br [NULLS NOT DISTINCT]
%end

%if ({pgsql-ver} >=f "11.0") %and {include-cols} %then
	$br [INCLUDE ] ({include-cols})
%end

%if {stg-params} %then
	$br [WITH (]

	%if {factor} %then
		[FILLFACTOR = ] {factor}
	%end

	%if {fast-update} %then
		%if {factor} %then [, ] %end
		[FASTUPDATE = ON]
	%end

	%if {buffering} %then
		%if {factor} %then [, ] %end
		[BUFFERING = ON]
	%end

	[)]
%end

%if {tablespace} %then
	$br [TABLESPACE ] {tablespace}
%end

%if {predicate} %then
	$br [WHERE (] {predicate} [)]
%end

;

@include "footer"
