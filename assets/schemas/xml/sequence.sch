# XML definition for sequences
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.
[<sequence name=] "{name}"

[ cycle=] %if {cycle} %then "true" %else "false" %end

%if {start} %then
    [ start=] "{start}"
%end

%if {increment} %then
    [ increment=] "{increment}"
%end

%if {min-value} %then
    [ min-value=] "{min-value}"
%end

%if {max-value} %then
    [ max-value=] "{max-value}"
%end

%if {cache} %then
    [ cache=] "{cache}"
%end

%if {owner-col} %then 
    [ owner-col=] "{owner-col}" 
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
</sequence>  $br $br
