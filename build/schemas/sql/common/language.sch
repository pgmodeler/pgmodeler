# pt_BR:
# Definição SQL para linguagens procedurais
# Versão PostgreSQL: 8.x, 9.0
# ATENÇÃO: Não modifique este arquivo a não ser que você saiba
#          o que está fazendo.
#
# en_US:
# SQL definition for procedural languages
# PostgreSQL Version: 8.x, 9.0
# CAUTION: Do not modify this file unless you know what
#          you are doing.

[-- object: ] @{name} [ | type: ] @{sql-object} [ -- ] $br

[CREATE ] 
  %if @{trusted} %then 
    %if @{handler} %then
      [TRUSTED ]
    %end
  %end
[ LANGUAGE ] @{name}

%if @{handler} %then
 $br $tb [HANDLER ] @{handler}
%end

%if @{validator} %then
 $br $tb [VALIDATOR ] @{validator}
%end
; $br

%if @{owner} %then @{owner} %end
%if @{comment} %then @{comment} %end

