# SQL command to drop an object
# PostgreSQL Version: 9.x
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if @{constraint} %and %not @{trigger} %then
 %if %not @{decl-in-table} %then
  [-- ALTER TABLE ] @{table} $sp DROP $sp @{sql-object} $sp
 %end
%else
 [-- DROP] $sp @{sql-object} $sp
%end

%if @{dif-sql} %then
  %if @{cast} %then [(] @{source-type} [ AS ] @{destiny-type} [)] %end
  %if @{extension} %then @{name} $sp CASCADE %end
  %if @{index} %then @{schema}.@{name} %end
  %if @{operator} %or @{function} %then @{signature} %end
  %if @{rule} %or @{trigger} %then @{name} [ ON ] @{table} %end

  %if @{aggregate} %or @{opclass} %or @{opfamily} %then
    @{name}
  %else
    %if @{constraint} %and %not @{decl-in-table} %then
     @{name}
    %end
  %end
%else
  @{name}
%end

; $br



