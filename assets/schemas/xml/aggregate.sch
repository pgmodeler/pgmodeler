# XML definition for aggregate functions
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.
[<aggregate name=] "{name}"

%if {initial-cond} %then
[ initial-cond=] "{initial-cond}"
%end

 %if {protected} %then 
  [ protected=] "true"
 %end

  %if {sql-disabled} %then
   [ sql-disabled=] "true"
  %end

> $br
  {schema}
  %if {owner} %then {owner} %end
  %if {comment} %then {comment} %end
  %if {appended-sql} %then {appended-sql} %end
  %if {prepended-sql} %then {prepended-sql} %end

  %if {types} %then {types} %end
  
  {state-type}
  {transition}
  %if {final} %then {final} %end
  %if {sort-op} %then {sort-op} %end
</aggregate>  $br $br
