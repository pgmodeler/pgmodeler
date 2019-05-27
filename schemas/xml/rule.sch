# XML definition for indexes
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.
 [<rule name=] "{name}" [ event-type=] "{event-type}" [ exec-type=] "{exec-type}" $br

 %if {alias} %then
   $sp alias="{alias}"
 %end
 
 %if {protected} %then 
  [ protected=] "true"
 %end

  %if {sql-disabled} %then
   [ sql-disabled=] "true"
  %end

 #%if {table} %then
  [ table=] "{table}"
 #%end

 > $br

 %if {comment} %then $tb {comment} %end

 %if {appended-sql} %then {appended-sql} %end
 %if {prepended-sql} %then {prepended-sql} %end

 %if {condition} %then
  $tb $tb <condition> <! $ob CDATA $ob {condition} $cb $cb > </condition> $br
 %end
 %if {commands} %then
  $tb $tb <commands> <! $ob CDATA $ob {commands} $cb $cb > </commands> $br
 %end

</rule> $br $br
