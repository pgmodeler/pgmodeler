# pt_BR:
# Definição SQL para comentários
# Versão PostgreSQL: 8.x, 9.0
# ATENÇÃO: Não modifique este arquivo a não ser que você saiba
#          o que está fazendo.
#
# en_US:
# SQL definition for comments
# PostgreSQL Version: 8.x, 9.0
# CAUTION: Do not modify this file unless that you know what
#          you are doing.
[COMMENT ON ] @{sql-object} $sp

%if @{dif-sql} %then
  %if @{column} %then @{table}.@{name} %end
  %if @{aggregate} %then @{name} [ (] @{types} [)] %end
  %if @{cast} %then [ (] @{source-type} [ AS ] @{destiny-type} [)] %end
  %if @{constraint} %then @{name} [ ON ] @{table} %end
  %if @{rule} %then @{name} [ ON ] @{table} %end
  %if @{trigger} %then @{name} [ ON ] @{table} %end
  #%if @{consttrigger} %then @{name} [ ON ] @{table} %end
  %if @{function} %then @{signature} %end
  %if @{operator} %then @{signature}  %end
  %if @{opclass} %then @{name} [ USING ] @{index-type} %end
  %if @{opfamily} %then @{name} [ USING ] @{index-type} %end
%else
 @{name}
%end

[ IS ] '@{comment}'; $br