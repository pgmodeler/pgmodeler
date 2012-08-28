# pt_BR:
# Definição XML para colunas
# ATENÇÃO: Não modifique este arquivo a não ser que você saiba
#          o que está fazendo.
#
# en_US:
# XML definition for columns
# CAUTION: Do not modify this file unless you know what
#          you are doing.
$tb [<column name=] "@{name}"

 %if @{not-null} %then
  [ not-null=] "true"
 %end

 %if @{default-value} %then
  [ default=] "@{default-value}"
 %end

 %if @{protected} %then 
  [ protected=] "true"
 %end

 > $br

 $tb @{type}

 %if @{comment} %then @{comment} %end

$tb </column> $br