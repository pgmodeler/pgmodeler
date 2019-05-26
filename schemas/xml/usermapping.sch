# XML definition for user mappings
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

[<usermapping name=] "{name}"

%if {role} %then
    [ role=] "{role}"
%end

[ server=] "{server}"

%if {options} %then
    [ options=] "{options}" 
%end

%if {protected} %then 
    [ protected=] "true"
%end

%if {sql-disabled} %then
    [ sql-disabled=] "true"
%end

> $br

%if {appended-sql} %then {appended-sql} %end
%if {prepended-sql} %then {prepended-sql} %end

</usermapping>  $br $br
