# SQL definition for constraints
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.
%if {decl-in-table} %then 
 $tb 
%else
[-- object: ] {name} [ | type: ] {sql-object} [ --] $br

  %if {table} %then
   [-- ] {drop}
   [ALTER TABLE ] {table} [ ADD ]
  %end
%end

[CONSTRAINT ] {name}

%if {ck-constr} %then
  [ CHECK ] ({expression})

  %if {no-inherit} %then [ NO INHERIT]  %end

%end

%if {pk-constr} %then [ PRIMARY KEY ] ({src-columns}) %end
%if {uq-constr} %then [ UNIQUE ] ({src-columns}) %end
%if {ex-constr} %then
 [ EXCLUDE ] $br
  $tb

  %if {index-type} %then
   [USING ] {index-type}
  %end

  ( {elements} $br $tb )
%end

%if {pk-constr} %or {uq-constr} %then
  %if {factor} %then
    %if {decl-in-table} %then $br $tb %end
    [ WITH (FILLFACTOR = ] {factor} [)]
  %end
%end

%if {tablespace} %then
  $br
  %if {decl-in-table} %then $tb %end
  %if {pk-constr} %or {uq-constr} %or {ex-constr} %then [USING INDEX TABLESPACE ] {tablespace} %end
%end

%if {ex-constr} %and {expression} %then
  $sp WHERE $sp ( {expression})
%end

%if {fk-constr} %then
  [ FOREIGN KEY ] ({src-columns}) $br
  
  %if {decl-in-table} %then $tb %end
  [REFERENCES ] {ref-table} $sp ({dst-columns})
  $sp {comparison-type} $br
  
  %if {decl-in-table} %then $tb %end
  [ON DELETE ] {del-action} [ ON UPDATE ] {upd-action}
%end

%if {deferrable} %then
  [ DEFERRABLE ] {defer-type}
%end

%if {decl-in-table} %then [,] 
%else
 [;] $br
 
# This is a special token that pgModeler recognizes as end of DDL command
# when exporting models directly to DBMS. DO NOT REMOVE THIS TOKEN!
[-- ddl-end --] $br
%end

%if %not {decl-in-table} %and {comment} %then
 {comment} $br 
%end

$br
