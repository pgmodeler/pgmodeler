# SQL definition for user defined type attributes
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.
[ ] {name} $sp {type}

%if {collation} %then
	[ COLLATE ] {collation}
%end

, $br
