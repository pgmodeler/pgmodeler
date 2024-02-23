# XML definition for function parameters
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.
$tb [<parameter name=] "&{name}"
%if {in} %then [ in=] "true" %end
%if {out} %then [ out=] "true" %end
%if {variadic} %then [ variadic=] "true" %end
%if {default-value} %then [ default-value=] "&{default-value}" %end
>$br

$tb {type}

$tb </parameter> $br
