# SQL definition for procedural languages
# PostgreSQL Version: 8.x, 9.0
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

[-- object: ] @{name} [ | type: ] @{sql-object} [ -- ] $br

[CREATE ] 
  %if @{trusted} %then 
    %if @{handler} %then
      [TRUSTED ]
    %end
  %end
[ LANGUAGE ] @{name}

%if @{handler} %then
 $br $tb [HANDLER ] @{handler}
%end

%if @{validator} %then
 $br $tb [VALIDATOR ] @{validator}
%end
; $br

%if @{owner} %then @{owner} %end
%if @{comment} %then @{comment} %end

# This is a special token that pgModeler recognizes as end of DDL command
# when exporting models directly to DBMS. DO NOT REMOVE THIS TOKEN!
[-- ddl-end --] $br $br