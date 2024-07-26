# SQL definition for rules
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.

@include "ddlend"
@include "objlabel"
@include "prependedsql"

[CREATE OR REPLACE RULE ] {name} [ AS ] {event-type} $br
$tb [TO ] {table} $br

%if {condition} %then
	$tb [WHERE ] {condition} $br
%end

$tb [DO ] {exec-type} $sp

%if {commands} %then
	({commands})
%else
	NOTHING
%end

;

@include "footer"
