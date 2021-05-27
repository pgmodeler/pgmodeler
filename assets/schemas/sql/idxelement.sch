# SQL definition for index elements
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.
$br $tb $sp $sp
%if {column} %then
 {column}
%else 
  %if {expression} %then
    ({expression})
  %end
%end

%if ({pgsql-ver} != "9.0") %and {collation} %then
  [ COLLATE ] {collation}
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
