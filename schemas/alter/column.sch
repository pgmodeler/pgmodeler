# SQL definition for columns's attributes change
# PostgreSQL Version: 9.x
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if {has-changes} %then
  %set {alter-table} [ALTER TABLE ] {table} [ ALTER COLUMN ] {name}
  %set {ddl-end} [;] $br [-- ddl-end --] $br

  %if {type} %then
   {alter-table}
   [ TYPE ] {type}

   %if {collation} %and ({pgsql-ver} != "9.0") %then
    [ COLLATE ] {collation}
   %end

   {ddl-end}
  %end
  
  %if {default-value} %then
    {alter-table}

    %if ({default-value}=="unset") %then
        [ DROP DEFAULT]
    %else
        [ SET DEFAULT ] {default-value}
    %end

    {ddl-end}
  %end
  
  %if {not-null} %then
    {alter-table}

    %if ({not-null}=="unset") %then
        [ DROP ]
    %else
        [ SET ]
    %end

    [NOT NULL]

    {ddl-end}
  %end

  %if {cur-identity-type} %and {min-value} %then
    {alter-table}
    [ SET GENERATED ] {cur-identity-type} [ SET MINVALUE ] {min-value}
    {ddl-end}
  %end
  
  %if {cur-identity-type} %and {max-value} %then
    {alter-table}
    [ SET GENERATED ] {cur-identity-type} [ SET MAXVALUE ] {max-value}
    {ddl-end}
  %end
  
  %if {cur-identity-type} %and {start} %then
    {alter-table}
    [ SET GENERATED ] {cur-identity-type} [ SET START WITH ] {start}
    {ddl-end}
  %end
  
  %if {cur-identity-type} %and {cache} %then
    {alter-table}
    [ SET GENERATED ] {cur-identity-type} [ SET CACHE ] {cache}
    {ddl-end}
  %end  
  
  %if {cur-identity-type} %and {increment} %then
    {alter-table}
    [ SET GENERATED ] {cur-identity-type} [ SET INCREMENT ] {increment}
    {ddl-end}
  %end 
  
  %if {cur-identity-type} %and {cycle} %then
    
    {alter-table}
    [ SET GENERATED ] {cur-identity-type} [ SET]
    
    %if ({cycle}=="false") %then [ NO] %end
    
    [ CYCLE]
    
    {ddl-end}
  %end    
  
  %if {identity-type} %or {new-identity-type} %then
    {alter-table}

    %if ({identity-type}=="unset") %then
        [ DROP IDENTITY]
    %else
        %if {new-identity-type} %then
            [ SET GENERATED ] {new-identity-type}
        %else
            [ ADD GENERATED ] {identity-type} [ AS IDENTITY]
        %end
    %end

    {ddl-end}
  %end
%end
