# XML definition for view / generic SQL references
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.
$tb <reference

[ object=] "&{object}" 
[ type=] "{type}"
[ ref-name=] "&{ref-name}"

%if {ref-alias} %then [ ref-alias=] "&{ref-alias}" %end
%if {use-signature} %then [ use-signature=] "true" %end
%if {format-name} %then [ format-name=] "true" %end
%if {use-columns} %then [ use-columns=] "true" %end

/> $br
