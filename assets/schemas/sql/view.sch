# SQL definition for views
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.

@include "ddlend"
@include "objlabel"
@include "prependedsql"

%if {materialized} %then
[CREATE ]
%else
[CREATE OR REPLACE ]
%end

%if {recursive} %then
	[RECURSIVE ]
%else
	%if {materialized} %then
		[MATERIALIZED ]
	%end
%end

VIEW $sp {name}

%if {columns} %then
	[ (] {columns} [)] 
%end

%if {materialized} %and {tablespace} %then
	$br TABLESPACE $sp {tablespace}
%end

%if {options} %then
	$br [WITH (] {options} [)] 
%end

$br [AS ] $br {definition}

%if {materialized} %and {with-no-data} %then
	$br [WITH NO DATA]
%end

%if %not {materialized} %and {check-option} %then
	$br [WITH ] {check-option} [ OPTION] 
%end

[;] 

@include "footer"
