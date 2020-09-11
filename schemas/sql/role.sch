# SQL definition for type roles
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

[-- object: ] {name} [ | type: ] {sql-object} [ --] $br
[-- ] {drop}

# This is a special token that pgModeler recognizes as end of DDL command
# when exporting models directly to DBMS. DO NOT REMOVE THIS TOKEN!
%set {ddl-end} $br [-- ddl-end --] $br

%if {prepended-sql} %then
  {prepended-sql}
  {ddl-end} $br
%end

[CREATE ROLE ] {name} [ WITH ]

%if {superuser} %then $br $tb SUPERUSER %end
%if {createdb} %then $br $tb CREATEDB %end
%if {createrole} %then $br $tb CREATEROLE %end
%if {inherit} %then $br $tb INHERIT %end
%if {login} %then $br $tb LOGIN %end

%if {replication} %and ({pgsql-ver} != "9.0") %then $br $tb REPLICATION %end
%if {bypassrls} %and ({pgsql-ver} >=f "9.5") %then $br $tb BYPASSRLS %end

%if {password} %then
 $br $tb
 %if {encrypted} %then
   ENCRYPTED
 %else
   UNENCRYPTED
 %end
 [ PASSWORD ] '{password}'
%end

%if {connlimit} %then $br $tb [CONNECTION LIMIT ] {connlimit} %end
%if {validity} %then $br $tb [VALID UNTIL ] '{validity}' %end

%if {ref-roles} %then $br $tb [IN ROLE ] {ref-roles} %end
%if {member-roles} %then $br $tb [ROLE ] {member-roles} %end
%if {admin-roles} %then $br $tb [ADMIN ] {admin-roles} %end
; 

{ddl-end}

%if {comment} %then {comment} %end

%if {appended-sql} %then
 {appended-sql}
 {ddl-end}
%end

$br
