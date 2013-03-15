# XML definition for aggregate functions
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.
[<aggregate name=] "@{name}"

%if @{initial-cond} %then
[ initial-cond=] "@{initial-cond}"
%end

 %if @{protected} %then 
  [ protected=] "true"
 %end

> $br
  @{schema}
  %if @{owner} %then @{owner} %end
  %if @{comment} %then @{comment} %end

  @{types}
  @{state-type}
  @{transition}
  %if @{final} %then @{final} %end
</aggregate>  $br $br