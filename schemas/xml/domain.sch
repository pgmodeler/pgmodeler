# pt_BR:
# Definição XML para domínios
# ATENÇÃO: Não modifique este arquivo a não ser que você saiba
#          o que está fazendo.
#
# en_US:
# XML definition for domains
# CAUTION: Do not modify this file unless you know what
#          you are doing.
[<domain name=] "@{name}"

%if @{reduced-form} %then
  /> $br
%else
  [ constraint=] "@{constraint}"

  [ not-null=] %if @{not-null} %then "true" %else "false" %end
  %if @{default-value} %then [ default=] "@{default-value}" %end

  %if @{protected} %then 
   [ protected=] "true"
  %end
  > $br

  @{schema}
  %if @{owner} %then @{owner} %end
  %if @{comment} %then @{comment} %end

  $tb @{type}

  %if @{expression} %then
   $tb <expression>@{expression}</expression>$br
  %end

  </domain>  $br $br
%end