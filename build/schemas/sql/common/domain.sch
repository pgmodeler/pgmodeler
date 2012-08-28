# pt_BR:
# Definição SQL para domínios
# Versão PostgreSQL: 8.x, 9.0
# ATENÇÃO: Não modifique este arquivo a não ser que você saiba
#          o que está fazendo.
#
# en_US:
# SQL definition for domains
# PostgreSQL Version: 8.x, 9.0
# CAUTION: Do not modify this file unless you know what
#          you are doing.

[-- object: ] @{name} [ | type: ] @{sql-object} [ -- ] $br

[CREATE DOMAIN ] @{name} [ AS ] @{type}
%if @{default-value} %then $br $tb [DEFAULT ] @{default-value} %end
%if @{constraint} %then $br $tb [CONSTRAINT ] @{constraint} %end
%if @{not-null}  %then $br $tb [NOT NULL] %end
%if @{expression} %then  $br $tb [CHECK ] (@{expression}) %end
;$br

%if @{owner} %then @{owner} %end
%if @{comment} %then @{comment} %end
