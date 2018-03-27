# XML definition for type casts
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.
[<cast ]

 %if {cast-type} %then
  [ cast-type=] "{cast-type}"
 %end 

 %if {io-cast} %then 
  [ io-cast=] "true"
 %end

 %if {protected} %then 
  [ protected=] "true"
 %end

  %if {sql-disabled} %then
   [ sql-disabled=] "true"
  %end

> $br

%if {comment} %then {comment} %end
%if {appended-sql} %then {appended-sql} %end
%if {prepended-sql} %then {prepended-sql} %end

{source-type}
{destiny-type}

%if {function} %then {function} %end

</cast>  $br $br
