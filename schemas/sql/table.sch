# SQL definition for tables
# PostgreSQL Version: 9.x
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

[-- object: ] {name} [ | type: ] {sql-object} [ --] $br

[-- ] {drop}

 %if {prepended-sql} %then
   {prepended-sql}
   $br [-- ddl-end --] $br $br
 %end

[CREATE]

%if ({pgsql-ver} != "9.0") %and {unlogged} %then
  [ UNLOGGED]
%end

[ TABLE ] {name} ( $br
  %if {copy-table} %then
    $tb LIKE $sp {copy-table}
    %if %not {gen-alter-cmds} %then
      %if {columns} %or {constraints} %then [,] %end
    %end
    $br
  %end

  %if %not {gen-alter-cmds} %then
    %if {columns} %then 
        {columns} 
        
        %if %not {constr-sql-disabled} %and {constraints} %then [,] $br %end
    %end

    %if {inh-columns} %then {inh-columns} %end

    %if {constraints} %then       
        {constraints}
    %end
  %end

$br )

%if {ancestor-table} %then [ INHERITS(] {ancestor-table} [)] $br %end
%if {oids} %then [WITH ( OIDS = TRUE )] %end
%if {tablespace} %then
 $br [TABLESPACE ] {tablespace}
%end
; $br

# This is a special token that pgModeler recognizes as end of DDL command
# when exporting models directly to DBMS. DO NOT REMOVE THIS TOKEN!
[-- ddl-end --] $br

%if {gen-alter-cmds} %then
  %if {columns} %then $br {columns} %end
  %if {constraints} %then $br {constraints} %end
%end

%if {comment} %then {comment} %end
%if {cols-comment} %then {cols-comment} %end
%if {owner} %then {owner} %end

%if ({pgsql-ver} >=f "9.5") %and {rls-enabled} %then
  [ALTER TABLE ] {name} [ ENABLE ROW LEVEL SECURITY;]
  $br [-- ddl-end --] $br
  
  %if {rls-forced} %then
    [ALTER TABLE ] {name} [ FORCE ROW LEVEL SECURITY;]
    $br [-- ddl-end --] $br
  %end
%end

%if {appended-sql} %then
 {appended-sql}
 $br [-- ddl-end --] $br
%end

%if {initial-data} %then
 $br {initial-data} $br
%end

 $br
