# pt_BR:
# Definição SQL para elementos de classe de operadores
# Versão PostgreSQL: 8.x, 9.0
# ATENÇÃO: Não modifique este arquivo a não ser que você saiba
#          o que está fazendo.
#
# en_US:
# SQL definition for operator class elements
# PostgreSQL Version: 8.x, 9.0
# CAUTION: Do not modify this file unless that you know what
#          you are doing.
%if @{column} %then
 @{column}
%else 
  %if @{expression} %then
    [(] @{expression} [)]
  %end
%end

%if @{asc-order} %then
 [ ASC ]
%else
 [ DESC ]
%end

%if @{nulls-first} %then
 [NULLS FIRST]
%else
 [NULLS LAST]
%end
