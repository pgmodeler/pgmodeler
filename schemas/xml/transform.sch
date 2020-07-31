# XML definition for transforms
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.
<transform> $br

%if {comment} %then {comment} %end
%if {appended-sql} %then {appended-sql} %end
%if {prepended-sql} %then {prepended-sql} %end

{type}

# One of the two function should be defined otherwise the code generation will fail
%if {fromsql} %or %not {tosql} %then
  {fromsql}
%end

%if {tosql} %or %not {fromsql} %then
  {tosql} 
%end

</transform> $br
