# SQL definition for views
# PostgreSQL Version: 9.x
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

[-- object: ] @{name} [ | type: ] @{sql-object} [ --] $br

[CREATE ]

%if %not @{pgsql90} %and %not @{pgsql91} %and %not @{pgsql92} %then
  %if @{recursive} %then
    [RECURSIVE ]
  %else
    %if @{materialized} %then
      [MATERIALIZED ]
    %end
  %end
%end

VIEW $sp @{name}

%if @{columns} %then
[ (] @{columns} [)]
%end

$br

%if %not @{pgsql90} %and %not @{pgsql91} %and %not @{pgsql92} %then
  %if @{materialized} %and @{tablespace} %then
    TABLESPACE $sp @{tablespace} $br
  %end
%end

[AS ]

#Commom table expression (CTE)
%if @{cte-exp} %then
 [WITH ] @{cte-exp}
%end

@{declaration}

%if %not @{pgsql90} %and %not @{pgsql91} %and %not @{pgsql92} %then
  %if @{materialized} %and @{with-no-data} %then
    $br [WITH NO DATA]
  %end
%end

 ; $br

# This is a special token that pgModeler recognizes as end of DDL command
# when exporting models directly to DBMS. DO NOT REMOVE THIS TOKEN!
[-- ddl-end --] $br $br

%if @{triggers} %then @{triggers} %end
%if @{rules} %then @{rules} %end
%if @{comment} %then @{comment} %end
%if @{owner} %then @{owner} %end
%if @{appended-sql} %then @{appended-sql} %end

%if @{comment} %or @{owner} %or @{appended-sql} %then
# This is a special token that pgModeler recognizes as end of DDL command
# when exporting models directly to DBMS. DO NOT REMOVE THIS TOKEN!
[-- ddl-end --] $br $br
%end

