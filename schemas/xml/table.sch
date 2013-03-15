# XML definition for tables
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.
[<table name=] "@{name}" 
 [ oids=] %if @{oids} %then "true" %else "false" %end
 
 %if @{protected} %then 
  [ protected=] "true"
 %end

> $br

 @{schema}
 %if @{tablespace} %then @{tablespace} %end
 %if @{comment} %then @{comment} %end
 @{position}

 %if @{columns} %then @{columns} %end
 %if @{constraints} %then @{constraints} %end
 %if @{indexes} %then @{indexes} %end
 %if @{rules} %then @{rules} %end
 %if @{triggers} %then @{triggers} %end
</table>  $br $br