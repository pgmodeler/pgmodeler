# XML definition for policies
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

[<policy name=] "{name}"

 %if {alias} %then
   $sp alias="{alias}"
 %end

[ table=] "{table}" [ command=] "{command}"

[ permissive=] %if {permissive} %then  "true" %else "false" %end

%if {protected} %then [ protected=] "true" %end
%if {sql-disabled} %then [ sql-disabled=] "true" %end
> $br
    
%if {comment} %then {comment} %end
%if {appended-sql} %then {appended-sql} %end
%if {prepended-sql} %then {prepended-sql} %end    

%if {roles} %then
    $tb [<roles names=] "{roles}" [/>] 
%end

%if {using-exp} %then
    $br $tb [<expression type="using-exp">] <! $ob CDATA $ob {using-exp} $cb $cb > </expression>
%end    

%if {check-exp} %then
    $br $tb [<expression type="check-exp">] <! $ob CDATA $ob {check-exp} $cb $cb > </expression>
%end    
 
$br

</policy>
