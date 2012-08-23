# pt_BR:
# Definição SQL para visões
# Versão PostgreSQL: 8.x, 9.0
# ATENÇÃO: Não modifique este arquivo a não ser que você saiba
#          o que está fazendo.
#
# en_US:
# SQL definition for views
# PostgreSQL Version: 8.x, 9.0
# CAUTION: Do not modify this file unless that you know what
#          you are doing.

[-- object: ] @{name} [ | type: ] @{sql-object} [ -- ] $br

[CREATE OR REPLACE VIEW ] @{name} $br
[AS ] @{declaration}; $br

%if @{comment} %then @{comment} %end
