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
 
 $br
 %if @{decl-in-table} %then $tb %end
 $tb
 [ index-type=] "@{index-type}"
 [ factor=] "@{factor}"

 %if @{protected} %then 
  [ protected=] "true"
 %end

> $br

 %if @{tablespace} %then $tb @{tablespace} %end
 
 %if @{comment} %then $tb @{comment} %end

 @{elements}

 %if @{condition} %then
   $tb <condition> <! $ob CDATA $ob @{condition} $cb $cb > </condition> $br
 %end
 
%if @{decl-in-table} %then $tb %end
</index> $br