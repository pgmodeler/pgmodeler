# XML definition for exclude constraint elements
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.
$tb $tb <excelement

[ use-sorting=] %if {use-sorting} %then "true" %else "false" %end

%if {use-sorting} %then
	[ nulls-first=] %if {nulls-first} %then "true" %else "false" %end
	[ asc-order=] %if {asc-order} %then "true" %else "false" %end
%end

> $br

%if {column} %then
	$tb $tb $tb [<column name=] "&{column}" /> $br
%else
	#%if {expression} %then
	$tb $tb $tb <expression> $sp <! $ob CDATA $ob {expression} $cb $cb > $sp </expression> $br
	#%end
%end

%if {opclass} %then
	$tb $tb $tb {opclass}
%end

#%if {operator} %then
$tb $tb $tb {operator}
#%end

$tb $tb </excelement> $br
