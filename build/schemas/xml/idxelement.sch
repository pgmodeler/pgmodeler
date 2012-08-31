# pt_BR:
# Definição XML para elementos de índices
# ATENÇÃO: Não modifique este arquivo a não ser que você saiba
#          o que está fazendo.
#
# en_US:
# XML definition for indexes elements
# CAUTION: Do not modify this file unless you know what
#          you are doing.
$tb <idxelement
 [ nulls-first=] %if @{nulls-first} %then "true" %else "false" %end
 [ asc-order=] %if @{asc-order} %then "true" %else "false" %end
> $br

%if @{column} %then
  $tb $tb [<column name=] "@{column}" /> $br
%else
 %if @{expression} %then
  $tb $tb <expression> @{expression} </expression> $br
 %end
%end

%if @{opclass} %then
  $tb $tb @{opclass}
%end

$tb </idxelement> $br