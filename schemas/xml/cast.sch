# XML definition for type casts
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.
[<cast cast-type=] "@{cast-type}" 

 %if @{io-cast} %then 
  [ io-cast=] "true"
 %end

 %if @{protected} %then 
  [ protected=] "true"
 %end

> $br

%if @{comment} %then @{comment} %end

@{source-type}
@{destiny-type}

%if @{signature} %then @{signature} %end
#%if @{function} %then
# $tb [<function signature=] "@{signature}" /> $br
#%end

</cast>  $br $br