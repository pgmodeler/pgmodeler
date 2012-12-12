# XML definition for indexes
# CAUTION: Do not modify this file unless you know what
#          you are doing.
$tb [<rule name=] "@{name}" [ event-type=] "@{event-type}" $br
 $tb $tb [ exec-type=] "@{exec-type}"

 %if @{protected} %then 
  [ protected=] "true"
 %end

 > $br

 %if @{comment} %then $tb @{comment} %end

 %if @{condition} %then
  $tb $tb <condition> @{condition} </condition> $br
 %end
 %if @{commands} %then
  $tb $tb <commands> @{commands} </commands> $br
 %end

$tb </rule> $br