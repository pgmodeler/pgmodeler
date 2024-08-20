# SQL definition for tablespaces
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.

@include "ddlend"
@include "objlabel"
@include "prependedsql"

[CREATE TABLESPACE ] {name} $br

%if {owner} %then
	$tb [OWNER ] {owner} $br
%end

$tb [LOCATION ] {directory}; $br

@include "footer"
