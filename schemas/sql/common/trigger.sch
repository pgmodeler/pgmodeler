# SQL definition for triggers
# PostgreSQL Version: 8.x, 9.0
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

[-- object: ] @{name} [ | type: ] @{sql-object} [ -- ] $br

[CREATE ] 
%if @{ref-table} %then [CONSTRAINT ]%end
[TRIGGER ] @{name} $br
$tb @{firing-type} $sp @{events} $br

$tb [ON ] @{table} $br

%if @{ref-table} %then 
 $tb [FROM ] @{ref-table} $br 

 %if @{deferrable} %then
  $tb [DEFERRABLE ] @{defer-type} $br
 %else
  $tb [NOT DEFERRABLE ] $br
 %end
%end

$tb [FOR EACH ] %if @{per-line} %then ROW %else STATEMENT %end $br

$tb [EXECUTE PROCEDURE ] @{trigger-func}(
%if @{arguments} %then @{arguments} %end ); $br

%if @{comment} %then @{comment} %end


# This is a special token that pgModeler recognizes as end of DDL command
# when exporting models directly to DBMS. DO NOT REMOVE THIS TOKEN!
[-- ddl-end --] $br $br