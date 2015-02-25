# XML definition for tables
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.
[<table name=] "{name}" 
  %if {oids} %then [ oids="true"] %end
  %if {gen-alter-cmds} %then [ gen-alter-cmds="true"] %end
  %if {unlogged} %then [ unlogged="true"] %end
 
 %if {protected} %then 
  [ protected=] "true"
 %end

  %if {sql-disabled} %then
   [ sql-disabled=] "true"
  %end

> $br

 {schema}
 %if {owner} %then {owner} %end
 %if {tablespace} %then {tablespace} %end
 %if {comment} %then {comment} %end
 %if {tag} %then {tag} %end
 %if {appended-sql} %then {appended-sql} %end
 %if {prepended-sql} %then {prepended-sql} %end
 {position}

 %if {columns} %then {columns} %end
 %if {constraints} %then {constraints} %end
 
 %if {col-indexes} %then {col-indexes} %end
 %if {constr-indexes} %then {constr-indexes} %end

</table>  $br $br
