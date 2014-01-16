# SQL definition for comments
# PostgreSQL Version: 9.x
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.
[COMMENT ON ] @{sql-object} $sp

%if @{dif-sql} %then
  %if @{column} %then @{table}.@{name} %end
  %if @{aggregate} %then @{name} [ (] @{types} [)] %end
  %if @{cast} %then [ (] @{source-type} [ AS ] @{destiny-type} [)] %end
  %if @{rule} %or @{trigger} %or @{constraint} %then @{name} [ ON ] @{table} %end
  %if @{index} %then @{schema}.@{name} %end
  %if @{function} %or @{operator} %then @{signature} %end
  %if @{opclass} %or @{opfamily} %then @{name} [ USING ] @{index-type} %end
  %if @{extension} %then @{name} %end
%else
 @{name}
%end

[ IS ] '@{comment}'; $br

# This is a special token that pgModeler recognizes as end of DDL command
# when exporting models directly to DBMS. DO NOT REMOVE THIS TOKEN!
#[-- ddl-end --] $br
