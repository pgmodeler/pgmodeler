# pt_BR:
# Definição XML para funçoes agregadas
# ATENÇÃO: Não modifique este arquivo a não ser que você saiba
#          o que está fazendo.
#
# en_US:
# XML definition for aggregate functions
# CAUTION: Do not modify this file unless you know what
#          you are doing.
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