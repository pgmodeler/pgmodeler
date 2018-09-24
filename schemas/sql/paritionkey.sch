# SQL definition for partition key elements
# PostgreSQL Version: 10.x
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

$br $tb $sp $tb

%if {column} %then
 {column}
%else 
  %if {expression} %then
    ({expression})
  %end
%end

%if {opclass} %then
  $tb $tb $tb {opclass}
%end
