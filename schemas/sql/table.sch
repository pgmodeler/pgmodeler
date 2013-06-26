# SQL definition for tables
# PostgreSQL Version: 9.x
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

[-- object: ] @{name} [ | type: ] @{sql-object} [ --] $br

[CREATE TABLE ] @{name} ( $br
  %if @{copy-table} %then
    $tb LIKE $sp @{copy-table}
    %if @{columns} %then [,] %end
    $br
  %end

  %if @{columns} %then @{columns} %end
  %if @{constraints} %then , $br @{constraints} %end
$br )


%if @{oids} %then [WITH ( OIDS = TRUE )] %end

%if @{tablespace} %then
 $br [TABLESPACE ] @{tablespace}
%end
; $br

# This is a special token that pgModeler recognizes as end of DDL command
# when exporting models directly to DBMS. DO NOT REMOVE THIS TOKEN!
[-- ddl-end --] $br $br

%if @{indexes} %then @{indexes} $br %end
%if @{triggers} %then @{triggers} $br %end
%if @{rules} %then @{rules} $br %end

%if @{comment} %then @{comment} %end
%if @{cols-comment} %then @{cols-comment} %end
%if @{owner} %then @{owner} %end

%if @{appended-sql} %then @{appended-sql} %end

# This is a special token that pgModeler recognizes as end of DDL command
# when exporting models directly to DBMS. DO NOT REMOVE THIS TOKEN!
[-- ddl-end --] $br $br
