# pt_BR:
# Definição XML para sequencias
# ATENÇÃO: Não modifique este arquivo a não ser que você saiba
#          o que está fazendo.
#
# en_US:
# XML definition for sequences
# CAUTION: Do not modify this file unless you know what
#          you are doing.
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
