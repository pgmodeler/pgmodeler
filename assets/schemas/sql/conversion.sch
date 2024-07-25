# SQL definition for encoding conversions
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.

@include "header"

CREATE %if {default} %then [ DEFAULT] %end

[ CONVERSION ] {name} $br
$tb [FOR ] '{src-encoding}' [ TO ] '{dst-encoding}' $br
$tb [FROM ] {function};

@include "footer"