# XML definition for indexes
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.
$tb [<rule name=] "@{name}" [ event-type=] "@{event-type}" $br
 $tb $tb [ exec-type=] "@{exec-type}"

 %if @{protected} %then 
  [ protected=] "true"
 %end

 > $br

 %if @{comment} %then $tb @{comment} %end

 %if @{condition} %then
  $tb $tb <condition> <! $ob CDATA $ob @{condition} $cb $cb > </condition> $br
 %end
 %if @{commands} %then
  $tb $tb <commands> <! $ob CDATA $ob @{commands} $cb $cb > </commands> $br
 %end

$tb </rule> $br