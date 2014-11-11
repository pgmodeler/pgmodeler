# SQL command to drop an object
# PostgreSQL Version: 9.x
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if @{collation} %and (@{pgsql-ver} != "9.0") %or %not @{collation} %then

 %if @{constraint} %or @{column} %then
    %if %not @{decl-in-table} %then
      [ALTER TABLE ] @{table} $sp DROP $sp @{sql-object} $sp
    %end
 %else
    [DROP ] @{sql-object} $sp
 %end

%if @{column} %or @{constraint} %and %not @{decl-in-table} %then
  @{name}
%else
  @{signature}
%end

 %if @{cascade} %and %not @{database} %and %not @{tablespace} %and %not @{role} %then
   [ CASCADE]
 %end
 ; 

 $br [-- ddl-end --] $br
 
%end
