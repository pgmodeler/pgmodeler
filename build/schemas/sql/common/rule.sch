# pt_BR:
# Definição SQL para regras
# Versão PostgreSQL: 8.x, 9.0
# ATENÇÃO: Não modifique este arquivo a não ser que você saiba
#          o que está fazendo.
#
# en_US:
# SQL definition for rules
# PostgreSQL Version: 8.x, 9.0
# CAUTION: Do not modify this file unless you know what
#          you are doing.

[-- object: ] @{name} [ | type: ] @{sql-object} [ -- ] $br

[CREATE OR REPLACE RULE ] @{name} [ AS ] @{event-type} $br
$tb [TO ] @{table} $br

%if @{condition} %then
$tb [WHERE ] @{condition} $br
%end

$tb [DO ] @{exec-type} $sp
%if @{commands} %then
  (@{commands})
%else
  NOTHING
%end
;$br

%if @{comment} %then @{comment} %end
