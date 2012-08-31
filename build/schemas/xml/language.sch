# pt_BR:
# Definição XML para linguagens
# ATENÇÃO: Não modifique este arquivo a não ser que você saiba
#          o que está fazendo.
#
# en_US:
# XML definition for schemas
# CAUTION: Do not modify this file unless you know what
#          you are doing.
%if @{reduced-form} %then $tb %end
[<language name=]"@{name}"

  %if @{protected} %then 
   [ protected=] "true"
  %end

%if @{reduced-form} %then
[/>] $br
%else
  [ trusted=] %if @{trusted} %then "true" %else "false" %end

  [>]$br

  %if @{owner} %then @{owner} %end
  %if @{comment} %then @{comment} %end

  %if @{handler} %then @{handler} %end
  %if @{validator} %then @{validator} %end

 </language>  $br $br
%end