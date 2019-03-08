# SQL definition for partition key elements
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if {column} %then
 {column}
%else 
  %if {expression} %then
    ({expression})
  %end
%end

%if {collation} %then
  [ COLLATE ] {collation}
%end

%if {opclass} %then
  $sp {opclass}
%end
