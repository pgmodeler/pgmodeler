# XML definition for indexes elements
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.
$tb <idxelement

 [ use-sorting=] %if @{use-sorting} %then "true" %else "false" %end

%if @{use-sorting} %then
 [ nulls-first=] %if @{nulls-first} %then "true" %else "false" %end
 [ asc-order=] %if @{asc-order} %then "true" %else "false" %end
%end

> $br

%if @{column} %then
  $tb $tb [<column name=] "@{column}" /> $br
%else
 %if @{expression} %then
  $tb $tb <expression> <! $ob CDATA $ob @{expression} $cb $cb > </expression> $br
 %end
%end

%if @{opclass} %then
  $tb $tb @{opclass}
%end

$tb </idxelement> $br
