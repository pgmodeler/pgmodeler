# XML definition for procedures
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

<procedure [ name=] "{name}"

%if {protected} %then [ protected=] "true" %end
%if {sql-disabled} %then [ sql-disabled=] "true" %end
[ security-type=] "{security-type}" > $br

{schema} 

%if {owner} %then {owner} %end
%if {comment} %then {comment} %end
%if {appended-sql} %then {appended-sql} %end
%if {prepended-sql} %then {prepended-sql} %end
%if {language} %then {language} %end
%if {parameters} %then {parameters} %end

%if {library} %then
    $tb [<definition library=] "{library}"

    %if {symbol} %then
        [ symbol=] "{symbol}"
    %end

    /> $br
%else
    $tb <definition><! $ob CDATA $ob {definition} $cb $cb ></definition> $br
%end

</procedure> $br $br 
