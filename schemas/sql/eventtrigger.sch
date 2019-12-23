# SQL definition for event triggers
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if ({pgsql-ver} >=f "9.3") %then

  [-- object: ] {name} [ | type: ] {sql-object} [ --] $br
  [-- ] {drop}

 %if {prepended-sql} %then
   {prepended-sql}
   $br [-- ddl-end --] $br $br
 %end

  [CREATE EVENT TRIGGER ] {name}
  $br $tb [ON ] {event}

  %if {filter} %then
    $br $tb [WHEN ] {filter}
  %end

  $br $tb [EXECUTE PROCEDURE ] {function}; $br

  # This is a special token that pgModeler recognizes as end of DDL command
  # when exporting models directly to DBMS. DO NOT REMOVE THIS TOKEN!
  [-- ddl-end --] $br

  %if {owner} %then {owner} %end
  %if {comment} %then {comment} %end

  %if {appended-sql} %then
   {appended-sql}
   $br [-- ddl-end --] $br
  %end

 $br
%end
