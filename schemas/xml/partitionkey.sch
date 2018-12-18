# XML definition for indexes elements
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.
$tb $tb <partitionkey> $br

%if {column} %then
  $tb $tb $tb [<column name=] "{column}" /> $br
%else
  $tb $tb $tb <expression> <! $ob CDATA $ob {expression} $cb $cb > </expression> $br
%end

%if {collation} %then
  $tb $tb {collation}
%end

%if {opclass} %then
  $tb $tb $tb {opclass}
%end

$tb $tb </partitionkey> $br
