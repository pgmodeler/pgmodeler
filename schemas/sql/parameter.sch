# SQL definition for function parameters
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if {variadic} %then
 VARIADIC
%else
  %if {in} %then IN  %end
  %if {out} %then OUT  %end
%end

%if {reduced-form} %then
 $sp {type}, $sp
%else
 $sp {name} $sp
 {type}

 %if {default-value} %then
  [ DEFAULT ] {default-value}
 %end
 , $sp
%end
