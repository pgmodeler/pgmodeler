# SQL definition for columns
# PostgreSQL Version: 9.x
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if {decl-in-table} %then
 $tb
%else
[-- object: ] {name} [ | type: ] {sql-object} [ --] $br
[-- ] {drop}
 %if {table} %then
  [ALTER TABLE ] {table} [ ADD COLUMN ]
  %end
%end

{name} $sp {type}

%if {not-null} %then
 [ NOT NULL]
%end

%if ({pgsql-ver} >=f "10.0") %and {identity-type} %then
    [ GENERATED ] {identity-type} [ AS IDENTITY ]
  
    %if {increment} %or {min-value} %or {max-value} %or  {start} %or {cache} %or {cycle} %then
        [(]
    %end
        
    %if {increment} %then
        [ INCREMENT BY ] {increment}
    %end

    %if {min-value} %then
        [ MINVALUE ] {min-value}
    %end

    %if {max-value} %then
        [ MAXVALUE ] {max-value}
    %end

    %if {start} %then
        [ START WITH ] {start}
    %end

    %if {cache} %then
        [ CACHE ] {cache}
    %end

    %if {cycle} %then
        [ CYCLE]
    %end
    
    %if {increment} %or {min-value} %or {max-value} %or {start} %or {cache} %or {cycle} %then
        [ )]
    %end
    
%else
    %if {default-value} %then
        [ DEFAULT ] {default-value}
    %end
%end

%if {decl-in-table} %then
[,]
%else
[;]
# This is a special token that pgModeler recognizes as end of DDL command
# when exporting models directly to DBMS. DO NOT REMOVE THIS TOKEN!
$br [-- ddl-end --] $br 
$br
%end

$br
