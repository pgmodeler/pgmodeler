# SQL definition for transforms
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.

@include "ddlend"
@include "objlabel"
@include "prependedsql"

[CREATE OR REPLACE TRANSFORM FOR ] {type} [ LANGUAGE ] {language} [ (]

# One of the two function should be defined otherwise the code generation will fail

%if {fromsql} %or %not {tosql} %then
	$br $tb [FROM SQL WITH FUNCTION ] {fromsql}
%end

%if {fromsql} %and {tosql} %then
	,
%end

%if {tosql} %or %not {fromsql} %then
	$br $tb [TO SQL WITH FUNCTION ] {tosql}
%end

$br [);]

@include "footer"
