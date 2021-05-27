# XML definition for view references
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.
$tb <reference

%if {ref-alias} %then 
    [ ref-alias=] "{ref-alias}" 
%end

%if {table} %then
  [ table=] "{table}" 
  %if {column} %then [ column=] "{column}" %end
  %if {alias} %then [ alias=] "{alias}" %end
  %if {column-alias} %then [ column-alias=] "{column-alias}" %end
%else
  %if {alias} %then [ alias=] "{alias}" %end
%end

%if {expression} %then
 > $br
 
 $tb $tb <expression> <! $ob CDATA $ob {expression} $cb $cb > </expression> $br
 
 %if {columns} %then {columns} %end
 %if {ref-tables} %then {ref-tables} %end
   
 $tb </reference> $br
%else
 /> $br
%end
