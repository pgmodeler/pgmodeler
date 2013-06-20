# SQL definition for type roles
# PostgreSQL Version: 9.x
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

[-- object: ] @{name} [ | type: ] @{sql-object} [ --] $br

[CREATE ROLE ] @{name} [ WITH ]

%if @{superuser} %then
  $br $tb SUPERUSER
%else
  %if @{createdb} %then $br $tb CREATEDB %end
  %if @{createrole} %then $br $tb CREATEROLE %end
  %if @{inherit} %then $br $tb INHERIT %end
  %if @{login} %then $br $tb LOGIN %end
  %if @{replication} %and %not @{pgsql90} %then $br $tb REPLICATION %end
%end

%if @{password} %then
 $br $tb
 %if @{encrypted} %then
   ENCRYPTED
 %else
   UNENCRYPTED
 %end
 [ PASSWORD ] '@{password}'
%end

%if @{connlimit} %then $br $tb [CONNECTION LIMIT ] @{connlimit} %end
%if @{validity} %then $br $tb [VALID UNTIL ] '@{validity}' %end

%if @{ref-roles} %then $br $tb [IN ROLE ] @{ref-roles} %end
%if @{member-roles} %then $br $tb [ROLE ] @{member-roles} %end
%if @{admin-roles} %then $br $tb [ADMIN ] @{admin-roles} %end
; $br

%if @{comment} %then @{comment} %end
%if @{appended-sql} %then @{appended-sql} %end

# This is a special token that pgModeler recognizes as end of DDL command
# when exporting models directly to DBMS. DO NOT REMOVE THIS TOKEN!
[-- ddl-end --] $br $br
