# pt_BR:
# Definição XML para parâmetros de função
# ATENÇÃO: Não modifique este arquivo a não ser que você saiba
#          o que está fazendo.
#
# en_US:
# XML definition for function parameters
# CAUTION: Do not modify this file unless you know what
#          you are doing.
$tb [<parameter name=] "@{name}"
 %if @{in} %then [ in=] "true" %end
 %if @{out} %then [ out=] "true" %end
 %if @{default-value} %then [ default-value=] "@{default-value}" %end
>$br

$tb @{type}

$tb </parameter>  $br