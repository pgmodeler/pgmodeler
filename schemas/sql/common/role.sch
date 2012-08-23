# pt_BR:
# Definição SQL para papéis
# Versão PostgreSQL: 8.x, 9.0
# ATENÇÃO: Não modifique este arquivo a não ser que você saiba
#          o que está fazendo.
#
# en_US:
# SQL definition for type roles
# PostgreSQL Version: 8.x, 9.0
# CAUTION: Do not modify this file unless you know what
#          you are doing.

[-- object: ] @{name} [ | type: ] @{sql-object} [ -- ] $br

[CREATE ROLE ] @{name} [ WITH ] $br

# CREATEUSER | NOCREATEUSER --> Mesmo que SUPERUSER | NOSUPERUSER
$tb %if @{superuser} %then SUPERUSER %else NOSUPERUSER %end $br
$tb %if @{createdb} %then CREATEDB %else CREATEDB %end $br
$tb %if @{createrole} %then CREATEROLE %else NOCREATEROLE %end $br
$tb %if @{inherit} %then INHERIT %else NOINHERIT %end $br
$tb %if @{login} %then LOGIN %else NOLOGIN %end $br
$tb %if @{connlimit} %then [CONNECTION LIMIT ] @{connlimit} %end $br

%if @{password} %then
 $tb
 %if @{encrypted} %then
   ENCRYPTED
 %else
   UNENCRYPTED
 %end
 [ PASSWORD ] '@{password}'
%end

%if @{validity} %then $br $tb [VALID UNTIL ] '@{validity}' %end

# IN GROUP rolename [, ...] --> Mesmo que IN ROLE
%if @{ref-roles} %then $br $tb [IN ROLE ] @{ref-roles} %end

# USER rolename [, ...] --> Mesmo que ROLE
%if @{member-roles} %then $br $tb [ROLE ] @{member-roles} %end
%if @{admin-roles} %then $br $tb [ADMIN ] @{admin-roles} %end
%if @{sysid} %then $br $tb [SYSID ] @{sysid} %end
; $br

%if @{comment} %then @{comment} %end
