# XML definition for views
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.
[<view name=] "@{name}" 

 %if @{protected} %then 
  [ protected=] "true"
 %end
 
 > $br

 @{schema}
 %if @{comment} %then @{comment} %end
 @{position}
 @{references}

 %if @{select-exp} %then
  [<expression type="select-exp">] @{select-exp} </expression> $br
 %end

 %if @{from-exp} %then
  [<expression type="from-exp">] @{from-exp} </expression> $br
 %end

 %if @{simple-exp} %then
  [<expression type="simple-exp">] @{simple-exp} </expression> $br
 %end
</view> $br $br