# SQL command to drop an object
# PostgreSQL Version: 9.x
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if @{constraint} %or @{column} %and %not @{trigger} %then
 %if %not @{decl-in-table} %then
  [ALTER TABLE ] @{table} $sp DROP $sp @{sql-object} $sp
 %end
%else
 [DROP ] @{sql-object} $sp
%end

%if %not @{dif-sql} %then
 @{name}
%else
    #%if @{cast} %or @{extension} %or @{index} %or @{operator} %or
    #    @{rule} %or @{trigger} %or @{aggregate} %or @{opclass} %or
    #    @{opfamily} %or @{constraint} %or @{function} %or @{column} %then

        %if @{cast} %then [(] @{source-type} [ AS ] @{destiny-type} [)] %end
        %if @{extension} %then @{name} $sp CASCADE %end
        %if @{index} %then @{schema}.@{name} %end
        %if @{operator} %or @{function} %then @{signature} %end
        %if @{rule} %or @{trigger} %then @{name} [ ON ] @{table} %end

      %if @{aggregate} %or @{opclass} %or @{opfamily} %then
          @{name}
      %else
          %if @{column} %or @{constraint} %and %not @{decl-in-table} %then
             @{name}
          %end
      %end
    #%end
%end

; $br
