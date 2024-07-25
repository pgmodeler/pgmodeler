# SQL definition for aggregate functions
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.

@include "ddlend"
@include "objlabel"
@include "prependedsql"

[CREATE OR REPLACE AGGREGATE ] {name} [ (]{types}[) (] $br
$tb [SFUNC = ] {transition}, $br
$tb [STYPE = ] {state-type} $br
%if {final} %then $tb [,FINALFUNC = ] {final} $br %end
%if {initial-cond} %then $tb [,INITCOND = ] {initial-cond} $br %end
%if {sort-op} %then $tb [,SORTOP = ] {sort-op} $br %end
);

@include "footer"
