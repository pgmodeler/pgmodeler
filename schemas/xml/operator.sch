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

[<operator ]

%if @{reduced-form} %then
  %if @{ref-type} %then ref-type="@{ref-type}" %end
  [ signature=] "@{signature}"
%else 
  name="@{name}"
%end

%if @{reduced-form} %then
/> $br
%else

  %if @{protected} %then 
   [ protected=] "true"
  %end
  > $br

  @{schema}
  %if @{owner} %then @{owner} %end
  %if @{comment} %then @{comment} %end

  %if @{left-type} %then @{left-type} %end
  %if @{right-type} %then @{right-type} %end


  %if @{commutator-op} %then @{commutator-op} %end
  %if @{greater-op} %then @{greater-op} %end
  %if @{less-op} %then @{less-op} %end
  %if @{negator-op} %then @{negator-op} %end
  %if @{sort-op} %then @{sort-op} %end
  %if @{sort2-op} %then @{sort2-op} %end

  %if @{operfunc} %then @{operfunc} %end
  %if @{join} %then @{join} %end
  %if @{restriction} %then @{restriction} %end

  </operator>  $br $br
%end