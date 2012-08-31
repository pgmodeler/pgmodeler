# pt_BR:
# Definição XML para familia de operadores
# ATENÇÃO: Não modifique este arquivo a não ser que você saiba
#          o que está fazendo.
#
# en_US:
# XML definition for operator families
# CAUTION: Do not modify this file unless you know what
#          you are doing.
%if @{reduced-form} %then $tb %end
[<opfamily name=] "@{name}"

%if @{reduced-form} %then
/> $br
%else
  [ index-type=] "@{index-type}" 

  %if @{protected} %then 
   [ protected=] "true"
  %end
  > $br

  @{schema}
  %if @{owner} %then @{owner} %end
  %if @{comment} %then @{comment} %end
  </opfamily>  $br $br
%end