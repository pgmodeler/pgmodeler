# SQL definition for exclude constraints elements
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.
$br $tb $sp $sp
%if {column} %then
 {column}
%else 
  %if {expression} %then
    {expression}
  %end
%end

%if {opclass} %then
  $sp {opclass}
%end

%if {use-sorting} %then
  %if {asc-order} %then
   [ ASC ]
  %else
   [ DESC ]
  %end

  %if {nulls-first} %then
   [NULLS FIRST]
  %else
   [NULLS LAST]
  %end
%end

[ WITH ] {operator}
