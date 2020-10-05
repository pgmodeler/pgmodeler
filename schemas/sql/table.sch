# SQL definition for tables
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

[CREATE]

%if ({pgsql-ver} != "9.0") %and {unlogged} %then
  [ UNLOGGED]
%end

[ TABLE ] {name} 

%if ({pgsql-ver} >=f "10.0") %and {partitioned-table} %then $br [PARTITION OF ] {partitioned-table} $sp %end

%if %not {partitioned-table} %or ({pgsql-ver} <f "10.0")  %then 

[ (] $br
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
    
    %if {inh-columns} %then 
        $br {inh-columns} 
    %end

    %if {constraints} %then
        {constraints}
    %end

  %end

$br )

%else 
    %if %not {gen-alter-cmds} %and {partitioned-table} %and {constraints} %then
        [ (] $br {constraints} $br [)] 
    %end
%end

%if ({pgsql-ver} >=f "10.0") %and {partitioned-table} %then 
    %if {partition-bound-expr} %then
        $br [FOR VALUES ] {partition-bound-expr}
    %else
        DEFAULT
    %end
%end

%if ({pgsql-ver} >=f "10.0") %and {partitioning} %then $br [PARTITION BY ] {partitioning} [ (] {partitionkey} [)] %end
%if {ancestor-table} %then $br [ INHERITS(] {ancestor-table} [)] %end
%if ({pgsql-ver} <=f "11.0") %and {oids} %then $br [WITH ( OIDS = TRUE )] %end
%if {tablespace} %then
 $br [TABLESPACE ] {tablespace}
%end
; 

{ddl-end}

%if {gen-alter-cmds} %then
  %if {columns} %then $br {columns} %end
  %if {constraints} %then $br {constraints} %end
%end

%if {comment} %then {comment} %end
%if {cols-comment} %then {cols-comment} %end
%if {owner} %then {owner} %end

%if ({pgsql-ver} >=f "9.5") %and {rls-enabled} %then
  [ALTER TABLE ] {name} [ ENABLE ROW LEVEL SECURITY;]
  {ddl-end}
  
  %if {rls-forced} %then
    [ALTER TABLE ] {name} [ FORCE ROW LEVEL SECURITY;]
    {ddl-end}
  %end
%end

%if {appended-sql} %then
 {appended-sql}
 {ddl-end}
%end

%if {initial-data} %then
 $br {initial-data} $br
%end

 $br
