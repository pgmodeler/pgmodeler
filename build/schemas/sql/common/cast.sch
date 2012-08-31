# pt_BR:
# Definição SQL para conversões de tipos
# Versão PostgreSQL: 8.x, 9.0
# ATENÇÃO: Não modifique este arquivo a não ser que você saiba
#          o que está fazendo.
#
# en_US:
# SQL definition for type casts
# PostgreSQL Version: 8.x, 9.0
# CAUTION: Do not modify this file unless you know what
#          you are doing.

[-- object: cast] ( @{source-type} [,] @{destiny-type} )  [ | type: ] @{sql-object} [ -- ] $br

[CREATE CAST (] @{source-type} [ AS ] @{destiny-type} ) $br

%if @{signature} %then
 $tb [WITH FUNCTION ] @{signature}
%else
 $tb [WITHOUT FUNCTION]
%end

%if @{cast-type} %then
 $br $tb [AS ] @{cast-type}
%end
; $br

%if @{comment} %then @{comment} %end

