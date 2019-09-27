# SQL command to drop an object
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if {collation} %and ({pgsql-ver} != "9.0") %or %not {collation} %then

 %if %not {domain} %and {constraint} %or {column} %then
    %if %not {decl-in-table} %then
      [ALTER TABLE ] {table} [ DROP ] {sql-object} [ IF EXISTS ]
    %end
 %else
    [DROP ] {sql-object} [ IF EXISTS ]
 %end

 %if {column} %or {constraint} %and %not {decl-in-table} %or {extension} %then
   {name}
 %else
   {signature}
 %end
 
 %if {cascade} %and %not {database} %and %not {tablespace} %and %not {role} %and %not {usermapping} %then
   [ CASCADE]
 %end
 ; 

 $br [-- ddl-end --] $br
 
%end
