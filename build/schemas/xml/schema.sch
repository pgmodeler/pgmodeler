# pt_BR:
# Definição XML para esquemas
# ATENÇÃO: Não modifique este arquivo a não ser que você saiba
#          o que está fazendo.
#
# en_US:
# XML definition for schemas
# CAUTION: Do not modify this file unless you know what
#          you are doing.
%if @{reduced-form} %then $tb %end
[<schema name=] "@{name}"

%if @{reduced-form} %then
/> $br
%else

  %if @{protected} %then 
   [ protected=] "true"
  %end

  > $br

  %if @{owner} %then @{owner} %end
  %if @{comment} %then @{comment} %end

  </schema>  $br $br
%end