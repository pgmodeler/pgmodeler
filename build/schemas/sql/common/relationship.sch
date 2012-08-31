# pt_BR:
# Definição SQL para relacionamentos
# Versão PostgreSQL: 8.x, 9.0
# ATENÇÃO: Não modifique este arquivo a não ser que você saiba
#          o que está fazendo.
#
# en_US:
# SQL definition for relationships
# PostgreSQL Version: 8.x, 9.0
# CAUTION: Do not modify this file unless that you know what
#          you are doing.

%if @{rel1n} %then
 @{constraint}
%end

%if @{relgen} %then
 [ALTER TABLE ] @{table} [ INHERIT ] @{ancestor-table}; $br $br
%end

%if @{relnn} %then
 @{table}
 @{constraints}
%end

