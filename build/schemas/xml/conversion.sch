# pt_BR:
# Definição XML para conversões de codificação
# ATENÇÃO: Não modifique este arquivo a não ser que você saiba
#          o que está fazendo.
#
# en_US:
# XML definition for type encoding conversion
# CAUTION: Do not modify this file unless you know what
#          you are doing.
[<conversion name=] "@{name}"
 [ src-encoding=] "@{src-encoding}"
 [ dst-encoding=] "@{dst-encoding}"

 %if @{protected} %then 
  [ protected=] "true"
 %end
 > $br

 @{schema}
 %if @{owner} %then @{owner} %end
 %if @{comment} %then @{comment} %end

 @{function}

</conversion>  $br $br