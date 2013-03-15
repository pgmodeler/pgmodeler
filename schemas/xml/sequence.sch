# XML definition for sequences
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.
[<sequence name=] "@{name}"

$br $tb
[ start=] "@{start}"
[ increment=] "@{increment}"

$br $tb
[ min-value=] "@{min-value}"
[ max-value=] "@{max-value}"

$br $tb
[ cache=] "@{cache}"
[ cycle=] %if @{cycle} %then "true" %else "false" %end
%if @{owner} %then [ owner=] "@{owner}" %end

 %if @{protected} %then 
  [ protected=] "true"
 %end

> $br

  @{schema}
  %if @{comment} %then @{comment} %end
</sequence>  $br $br
