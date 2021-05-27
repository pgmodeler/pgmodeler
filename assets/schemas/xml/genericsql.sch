# XML definition for generic sql objects
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.
[<genericsql name=] "{name}" 

 %if {protected} %then 
  [ protected=] "true"
 %end

 %if {sql-disabled} %then
  [ sql-disabled=] "true"
 %end

 > $br

 $tb <definition><! $ob CDATA $ob {definition} $cb $cb ></definition> $br
 
 %if {objects} %then {objects} %end
 
</genericsql>  $br $br
