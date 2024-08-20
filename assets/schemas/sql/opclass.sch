# SQL definition for operator classes
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.

@include "ddlend"
@include "objlabel"
@include "prependedsql"

[CREATE OPERATOR CLASS ] {name}
%if {default} %then [ DEFAULT ] %end

[ FOR TYPE ] {type} $br
[ USING ] {index-type}

%if {family} %then [ FAMILY ] {family} %end
[ AS] $br

{elements};

@include "footer"
