# SQL definition for extensions
# PostgreSQL Version: 9.x
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if %not @{pgsql90} %then
  
  [-- object: ] @{name} [ | type: ] @{sql-object} [ --] $br

  [CREATE EXTENSION ] @{name} $br
  [      WITH SCHEMA ] @{schema}
    
  %if @{cur-version} %then
    $br [      VERSION ] @{cur-version}
  %end
  
  %if @{old-version} %then
    $br [      FROM ] @{old-version}
  %end

  ; $br
   
  %if @{comment} %then  @{comment} %end
  %if @{appended-sql} %then @{appended-sql} %end

  # This is a special token that pgModeler recognizes as end of DDL command
  # when exporting models directly to DBMS. DO NOT REMOVE THIS TOKEN!
  [-- ddl-end --] $br $br
%end
