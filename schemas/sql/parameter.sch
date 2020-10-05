# SQL definition for function parameters
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if {variadic} %then
 VARIADIC $sp 
%else
  %if {in} %then IN $sp %end
  %if {out} %then OUT $sp %end
%end

%if {reduced-form} %then
 {type}, $sp
%else
 {name} $sp {type}

 %if {default-value} %then
  [ DEFAULT ] {default-value}
 %end
 , $sp
%end
