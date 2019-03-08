# SQL definition for views
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

[-- object: ] {name} [ | type: ] {sql-object} [ --] $br

[-- ] {drop}

 %if {prepended-sql} %then
   {prepended-sql}
   $br [-- ddl-end --] $br $br
 %end

[CREATE ]

%if ({pgsql-ver} >=f "9.3") %then
  %if {recursive} %then
    [RECURSIVE ]
  %else
    %if {materialized} %then
      [MATERIALIZED ]
    %end
  %end
%end

VIEW $sp {name}

%if {columns} %then
[ (] {columns} [)]
%end

$br

%if ({pgsql-ver} >=f "9.3") %then
  %if {materialized} %and {tablespace} %then
    TABLESPACE $sp {tablespace} $br
  %end
%end

[AS ] $br

#Commom table expression (CTE)
%if {cte-exp} %then
 [WITH ] {cte-exp}
%end

$br {definition}

%if ({pgsql-ver} >=f "9.3") %then
  %if {materialized} %and {with-no-data} %then
    $br [WITH NO DATA]
  %end
%end


# This is a special token that pgModeler recognizes as end of DDL command
# when exporting models directly to DBMS. DO NOT REMOVE THIS TOKEN!
$br [-- ddl-end --] $br

%if {comment} %then {comment} %end
%if {owner} %then {owner} %end

%if {appended-sql} %then
 {appended-sql}
 $br [-- ddl-end --] $br
%end

$br
