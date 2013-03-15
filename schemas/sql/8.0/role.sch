# SQL definition for type roles
# PostgreSQL Version: 8.0
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.
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

# This is a special token that pgModeler recognizes as end of DDL command
# when exporting models directly to DBMS. DO NOT REMOVE THIS TOKEN!
[-- ddl-end --] $br $br