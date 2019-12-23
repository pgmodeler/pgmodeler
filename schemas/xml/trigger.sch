# XML definition for triggers
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.
 [<trigger name=] "{name}" 
 
 %if {alias} %then
   $sp alias="{alias}"
 %end
 
 [ firing-type=] "{firing-type}"
 [ per-line=] %if {per-line} %then "true" %else "false" %end
 [ constraint=] %if {constraint} %then "true" %else "false" %end

 $br $tb

 [ ins-event=] %if {ins-event} %then "true" %else "false" %end
 [ del-event=] %if {del-event} %then "true" %else "false" %end
 [ upd-event=] %if {upd-event} %then "true" %else "false" %end
 [ trunc-event=] %if {trunc-event} %then "true" %else "false" %end

 %if {arguments} %then
   [ arguments=] "{arguments}" 
 %end

 %if {deferrable} %then
  $br $tb 
  %if {decl-in-table} %then $tb %end
  [ deferrable=] "true"
  [ defer-type=] "{defer-type}"
 %end

 %if {ref-table} %then
  [ ref-table=] "{ref-table}"
 %end

 %if {protected} %then 
  [ protected=] "true"
 %end

   $br $tb [ table=] "{table}"


  %if {sql-disabled} %then
   [ sql-disabled=] "true"
  %end
  
 %if {old-table-name} %or {new-table-name} %then
    $br $tb 
    %if {old-table-name} %then [ old-table-name=] "{old-table-name}" %end
    %if {new-table-name} %then [ new-table-name=] "{new-table-name}" %end
 %end

 > $br
 %if {comment} %then $tb {comment} %end

 %if {appended-sql} %then {appended-sql} %end
 %if {prepended-sql} %then {prepended-sql} %end

 %if {condition} %then
   $tb <condition> <! $ob CDATA $ob {condition} $cb $cb > </condition> $br
 %end

$tb {trigger-func}

 %if {columns} %then
   %if {decl-in-table} %then $tb %end
   $tb [<columns ] names="{columns}"/> $br
 %end

</trigger> $br $br
