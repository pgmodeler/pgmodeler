# XML definition for indexes
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

[<index name=] "{name}" [ table=] "{table}"
 
 %if {alias} %then
   $sp alias="{alias}"
 %end
  
 $br $tb
 [ concurrent=] %if {concurrent} %then "true" %else "false" %end 
 [ unique=] %if {unique} %then "true" %else "false" %end
 [ fast-update=] %if {fast-update} %then "true" %else "false" %end
 [ buffering=] %if {buffering} %then "true" %else "false" %end
 
 $br $tb
 [ index-type=] "{index-type}"
 [ factor=] "{factor}"

 %if {protected} %then 
  [ protected=] "true"
 %end

  %if {sql-disabled} %then
   [ sql-disabled=] "true"
  %end

> $br

 %if {tablespace} %then {tablespace} %end
 %if {comment} %then {comment} %end
 %if {appended-sql} %then {appended-sql} %end
 %if {prepended-sql} %then {prepended-sql} %end

 {elements}

 %if {predicate} %then
   $tb  <predicate> <! $ob CDATA $ob {predicate} $cb $cb > </predicate> $br
 %end
 
</index> $br $br
