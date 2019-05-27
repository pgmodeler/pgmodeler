# XML definition for views
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.
[<view name=] "{name}" 

 %if {alias} %then
   $sp alias="{alias}"
 %end

 %if {protected} %then [ protected=] "true" %end
 %if {materialized} %then [ materialized=] "true" %end
 %if {recursive} %then [ recursive=] "true" %end
 %if {with-no-data} %then [ with-no-data=] "true" %end
 %if {sql-disabled} %then [ sql-disabled=] "true" %end
 %if {hide-ext-attribs} %then [ hide-ext-attribs=] "true" %end
 %if {faded-out} %then [ faded-out=] "true" %end

 > $br

 {schema}
 %if {owner} %then {owner} %end
 %if {tablespace} %and {materialized} %then {tablespace} %end
 %if {comment} %then {comment} %end
 %if {tag} %then {tag} %end
 %if {appended-sql} %then {appended-sql} %end
 %if {prepended-sql} %then {prepended-sql} %end
 {position}
 {references}

 %if {select-exp} %then
  $tb [<expression type="select-exp">] {select-exp} </expression> $br
 %end

 %if {from-exp} %then
  $tb [<expression type="from-exp">] {from-exp} </expression> $br
 %end

 %if {simple-exp} %then
  $tb [<expression type="simple-exp">] {simple-exp} </expression> $br
 %end
 
 %if {end-exp} %then
  $tb [<expression type="end-exp">] {end-exp} </expression> $br
 %end

 %if {cte-exp} %then
  $tb [<expression type="cte-exp">] <! $ob CDATA $ob {cte-exp} $cb $cb ></expression> $br
 %end

</view> $br $br
