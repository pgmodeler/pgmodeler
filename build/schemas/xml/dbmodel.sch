# pt_BR:
# Definição XML para modelo de banco de dados
# ATENÇÃO: Não modifique este arquivo a não ser que você saiba
#          o que está fazendo.
#
# en_US:
# XML definition for database model
# CAUTION: Do not modify this file unless you know what
#          you are doing.
<?xml $sp version="1.0" $sp encoding="UTF-8"?> $br
[<!--] $br
[ATENÇÃO: Não modifique este arquivo a não ser que você saiba] $br
[         o que está fazendo. Resultados inesperados podem acontecer] $br
[         caso o código seja alterado deliberadamente.] $br
$br
[CAUTION: Do not modify this file unless you know what you are doing.] $br
[         Unexpected results may occur if the code is changed deliberately.] $br
[-->] $br
<dbmodel %if @{author} %then [ author=] "@{author}" %end
 %if @{protected} %then 
  [ protected=] "true"
 %end
 > $br
 %if @{objects} %then @{objects} %end
 %if @{grant} %then @{grant} %end
 </dbmodel> $br