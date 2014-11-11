# SQL definition for role's attributes change
# PostgreSQL Version: 9.x
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

@{alter-cmds}

%if @{has-changes} %then
  [ALTER ] @{sql-object} $sp @{signature} 
  
  %if @{superuser} %then
    $br $sp SUPERUSER
  %else
    %if @{createdb} %then $br $sp CREATEDB %end
    %if @{createrole} %then $br $sp CREATEROLE %end
    %if @{inherit} %then $br $sp INHERIT %end
    %if @{login} %then $br $sp LOGIN %end
    %if @{replication} %and (@{pgsql-ver} != "9.0") %then $br $sp REPLICATION %end
  %end

  %if @{password} %then
   $br $sp
   %if @{encrypted} %then
     ENCRYPTED
   %else
     UNENCRYPTED
   %end
   [ PASSWORD ] '@{password}'
  %end

  %if @{connlimit} %then $br $sp [CONNECTION LIMIT ] @{connlimit} %end
  %if @{validity} %then $br $sp [VALID UNTIL ] '@{validity}' %end
  
  $br [-- ddl-end --] $br
%end
