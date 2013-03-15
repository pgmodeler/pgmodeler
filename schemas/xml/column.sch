# XML definition for columns
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.
$tb [<column name=] "@{name}"

 %if @{not-null} %then
  [ not-null=] "true"
 %end

 %if @{default-value} %then
  [ default-value=] "@{default-value}"
 %end

 %if @{protected} %then 
  [ protected=] "true"
 %end

 > $br

 $tb @{type}

 %if @{comment} %then $tb @{comment} %end

$tb </column> $br
