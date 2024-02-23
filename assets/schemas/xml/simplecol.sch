# XML definition for view simple columns
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.
$tb [<simplecol name=] "&{name}"

%if {alias} %then
	$sp alias="&{alias}"
%end

%if {type} %then
	$sp type="&{type}"
%end

/> $br