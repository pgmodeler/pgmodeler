# pt_BR:
# Definição SQL para gatilhos
# Versão PostgreSQL: 8.x, 9.0
# ATENÇÃO: Não modifique este arquivo a não ser que você saiba
#          o que está fazendo.
#
# en_US:
# SQL definition for triggers
# PostgreSQL Version: 8.x, 9.0
# CAUTION: Do not modify this file unless you know what
#          you are doing.

[-- object: ] @{name} [ | type: ] @{sql-object} [ -- ] $br

[CREATE ] 
%if @{ref-table} %then [CONSTRAINT ]%end
[TRIGGER ] @{name} $br
$tb @{firing-type} $sp @{events} $br

$tb [ON ] @{table} $br

%if @{ref-table} %then 
 $tb [FROM ] @{ref-table} $br 

 %if @{deferrable} %then
  $tb [DEFERRABLE ] @{defer-type} $br
 %else
  $tb [NOT DEFERRABLE ] $br
 %end
%end

$tb [FOR EACH ] %if @{per-line} %then ROW %else STATEMENT %end $br

$tb [EXECUTE PROCEDURE ] @{trigger-func}(
%if @{arguments} %then @{arguments} %end ); $br

%if @{comment} %then @{comment} %end

