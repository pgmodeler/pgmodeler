# XML definition for operator classes
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.
$tb [<element type=]

%if {function} %then "function" %end
%if {operator} %then "operator" %end
%if {storage} %then "storage" %end

%if {stg-number} %then
	[ stg-number=] "{stg-number}"
%end

> $br
$tb {definition}
$tb </element> $br
