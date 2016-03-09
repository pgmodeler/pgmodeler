# XML definition for object positioning
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.
$tb [<object name=] "{name}" [ type=] "{type}" 

%if {src-table} %then
 $br $tb $tb  [src-table=] "{src-table}" [ src-type=] "{src-type}"
%end

%if {dst-table} %then
 $br $tb $tb  [dst-table=] "{dst-table}" [ dst-type=] "{dst-type}"
%end

> $br

$tb {position}
$tb </object> $br
