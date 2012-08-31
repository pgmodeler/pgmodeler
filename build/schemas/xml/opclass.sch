# pt_BR:
# Definição XML para classe de operadores
# ATENÇÃO: Não modifique este arquivo a não ser que você saiba
#          o que está fazendo.
#
# en_US:
# XML definition for operator classes
# CAUTION: Do not modify this file unless you know what
#          you are doing.
[<opclass name=] "@{name}"

%if @{reduced-form} %then
/> $br
%else

[ index-type=] "@{index-type}"
[ default=] %if @{default} %then "true" %else "false" %end

 %if @{protected} %then 
  [ protected=] "true"
 %end

 > $br

@{schema}
%if @{owner} %then @{owner} %end
%if @{comment} %then @{comment} %end

%if @{family} %then $tb <opfamily $sp name="@{family}" /> $br %end
@{type}
@{elements}

</opclass>  $br $br
%end