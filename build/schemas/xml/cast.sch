# pt_BR:
# Definição XML para conversões de tipos
# ATENÇÃO: Não modifique este arquivo a não ser que você saiba
#          o que está fazendo.
#
# en_US:
# XML definition for type casts
# CAUTION: Do not modify this file unless you know what
#          you are doing.
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