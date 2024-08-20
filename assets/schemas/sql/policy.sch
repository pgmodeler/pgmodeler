# SQL definition for policy
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.

@include "ddlend"
@include "objlabel"
@include "prependedsql"

[CREATE POLICY ] {name} [ ON ] {table}

%if {permissive} %then
	$br $tb [AS PERMISSIVE]
%else
	$br $tb [AS RESTRICTIVE]
%end

$br $tb [FOR ] {command}
$br $tb [TO ]

%if {roles} %then
	{roles}
%else
	PUBLIC
%end

%if {using-exp} %then
	$br $tb [USING (] {using-exp} [)]
%end

%if {check-exp} %then
	$br $tb [WITH CHECK (] {check-exp} [)]
%end

;

@include "footer"
