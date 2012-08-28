# pt_BR:
# Definição SQL para papéis
# Versão PostgreSQL: 8.0
# ATENÇÃO: Não modifique este arquivo a não ser que você saiba
#          o que está fazendo.
#
# en_US:
# SQL definition for type roles
# PostgreSQL Version: 8.0
# CAUTION: Do not modify this file unless you know what
#          you are doing.

 [-- object: ] @{name} [ | type: ] @{sql-object} [ -- ] $br

%if @{group} %then

 [CREATE GROUP ] @{name} [ WITH ] $br
 %if @{sysid} %then $tb [SYSID ] @{sysid} $br %end
 %if @{member-roles} %then $tb [ USER ] @{member-roles} $br  %end

%else

 [CREATE USER ] @{name} [ WITH ] $br
 %if @{sysid} %then $tb [SYSID ] @{sysid} $br %end
 $tb %if @{superuser} %then CREATEUSER %else NOCREATEUSER %end $br
 $tb %if @{createdb} %then CREATEDB %else CREATEDB %end $br

 %if @{ref-roles} %then  $tb [IN GROUP ] @{ref-roles} $br %end

 %if @{password} %then
  $tb
  %if @{encrypted} %then
    ENCRYPTED
  %else
    UNENCRYPTED
  %end
  [ PASSWORD ] '@{password}' $br
 %end

%end

%if @{validity} %then $tb [VALID UNTIL ] '@{validity}' %end
; $br

@{comment}
