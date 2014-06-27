# XML definition for indexes
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if @{decl-in-table} %then $tb %end
[<index name=] "@{name}"
 
 %if @{table} %then 
  [ table=] "@{table}"
 %end

 $br
 %if @{decl-in-table} %then $tb %end
 $tb
 [ concurrent=] %if @{concurrent} %then "true" %else "false" %end 
 [ unique=] %if @{unique} %then "true" %else "false" %end
 [ fast-update=] %if @{fast-update} %then "true" %else "false" %end
 [ buffering=] %if @{buffering} %then "true" %else "false" %end
 
 $br
 %if @{decl-in-table} %then $tb %end
 $tb
 [ index-type=] "@{index-type}"
 [ factor=] "@{factor}"

 %if @{protected} %then 
  [ protected=] "true"
 %end

  %if @{sql-disabled} %then
   [ sql-disabled=] "true"
  %end

> $br

 %if @{tablespace} %then $tb @{tablespace} %end
 
 %if @{comment} %then $tb @{comment} %end

 @{elements}

 %if @{predicate} %then
   $tb $tb <predicate> <! $ob CDATA $ob @{predicate} $cb $cb > </predicate> $br
 %end
 
%if @{decl-in-table} %then $tb %end
</index> $br
