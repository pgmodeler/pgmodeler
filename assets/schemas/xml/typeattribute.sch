# XML definition for user defined type attributes
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.
$tb [<typeattrib name=] "&{name}"> $br
$tb {type}

%if {collation} %then
	$tb {collation}
%end

$tb </typeattrib> $br
