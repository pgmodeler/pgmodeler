# SQL definition for extensions
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if ({pgsql-ver} != "9.0") %then
  
  [-- object: ] {name} [ | type: ] {sql-object} [ --] $br

  [-- ] {drop}

 %if {prepended-sql} %then
   {prepended-sql}
   $br [-- ddl-end --] $br $br
 %end

  [CREATE EXTENSION ] {name} $br
  
  %if {schema} %then
   [WITH SCHEMA ] {schema}
  %end
    
  %if {cur-version} %then
    $br [VERSION ] '{cur-version}'
  %end
  
  %if {old-version} %then
    $br [FROM ] '{old-version}'
  %end

  ; $br

  # This is a special token that pgModeler recognizes as end of DDL command
  # when exporting models directly to DBMS. DO NOT REMOVE THIS TOKEN!
  [-- ddl-end --] $br
   
  %if {comment} %then  {comment} %end

  %if {appended-sql} %then
   {appended-sql}
   $br [-- ddl-end --] $br
  %end

  $br
%end
