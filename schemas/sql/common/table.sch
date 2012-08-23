# pt_BR:
# Definição SQL para tabelas
# Versão PostgreSQL: 8.x, 9.0
# ATENÇÃO: Não modifique este arquivo a não ser que você saiba
#          o que está fazendo.
#
# en_US:
# SQL definition for tables
# PostgreSQL Version: 8.x, 9.0
# CAUTION: Do not modify this file unless you know what
#          you are doing.

[-- object: ] @{name} [ | type: ] @{sql-object} [ -- ] $br

[CREATE TABLE ] @{name} ( $br
%if @{columns} %then @{columns} %end
%if @{constraints} %then , $br @{constraints} %end
$br ) $br

%if @{tablespace} %then
 [TABLESPACE ] @{tablespace} $br
%end

[WITH (OIDS=] %if @{oids} %then TRUE %else FALSE %end ); $br $br

%if @{indexes} %then @{indexes} $br %end
%if @{triggers} %then @{triggers} $br %end
%if @{rules} %then @{rules} $br %end

%if @{comment} %then @{comment} %end
%if @{owner} %then @{owner} %end
