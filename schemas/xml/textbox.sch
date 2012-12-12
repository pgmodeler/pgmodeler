# XML definition for textboxes
# CAUTION: Do not modify this file unless you know what
#          you are doing.
[<textbox name=] "@{name}" 

 %if @{protected} %then 
  [ protected=] "true"
 %end

 %if @{italic} %then 
  [ italic=] "true"
 %end

 %if @{bold} %then 
  [ bold=] "true"
 %end

 %if @{underline} %then
  [ underline=] "true"
 %end

 %if @{color} %then
  [ color=] "@{color}"
 %end

 > $br

 @{position}
 @{comment}
</textbox>  $br $br
