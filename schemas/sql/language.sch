# SQL definition for procedural languages
# PostgreSQL Version: 9.x
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

[-- object: ] @{name} [ | type: ] @{sql-object} [ --] $br

@{drop}

 %if @{prepended-sql} %then
   @{prepended-sql}
   $br [-- ddl-end --] $br $br
 %end

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

%if @{inline} %then
 $br $tb [INLINE ] @{inline}
%end

; $br

%if @{owner} %then @{owner} %end
%if @{comment} %then @{comment} %end

# This is a special token that pgModeler recognizes as end of DDL command
# when exporting models directly to DBMS. DO NOT REMOVE THIS TOKEN!
[-- ddl-end --] $br $br

%if @{appended-sql} %then
 @{appended-sql}
 $br [-- ddl-end --] $br $br
%end
