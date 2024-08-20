# SQL definition for event triggers
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.

@include "ddlend"
@include "objlabel"
@include "prependedsql"

[CREATE EVENT TRIGGER ] {name}
$br $tb [ON ] {event}

%if {filter} %then
	$br $tb [WHEN ] {filter}
%end

$br $tb [EXECUTE PROCEDURE ] {function};

@include "footer"
