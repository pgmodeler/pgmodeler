# pt_BR:
# Definição SQL para parâmetro de funções
# Versão PostgreSQL: 8.4
# ATENÇÃO: Não modifique este arquivo a não ser que você saiba
#          o que está fazendo.
#
# en_US:
# SQL definition for function parameters
# PostgreSQL Version: 8.4
# CAUTION: Do not modify this file unless you know what
#          you are doing.
%if @{in} %then IN  %end
%if @{out} %then OUT  %end
$sp @{name} $sp
$sp @{type}

%if @{default-value} %then [ DEFAULT ] @{default-value} %end

, $sp