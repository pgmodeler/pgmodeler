# pt_BR:
# Definição SQL para conversões de codificação
# Versão PostgreSQL: 8.x, 9.0
# ATENÇÃO: Não modifique este arquivo a não ser que você saiba
#          o que está fazendo.
#
# en_US:
# SQL definition for encoding conversions
# PostgreSQL Version: 8.x, 9.0
# CAUTION: Do not modify this file unless you know what
#          you are doing.
[-- object: ] @{name} [ | type: ] @{sql-object} [ -- ] $br

CREATE %if @{default} %then [ DEFAULT] %end

[ CONVERSION ] @{name} $br
$tb [FOR ] '@{src-encoding}' [ TO ] '@{dst-encoding}' $br
$tb [FROM ] @{function}; $br

%if @{owner} %then @{owner} %end
%if @{comment} %then @{comment} %end


