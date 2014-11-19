# SQL definition for columns's attributes change
# PostgreSQL Version: 9.x
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%define(alter-table, [ALTER TABLE ] @{table} $sp)

%if @{has-changes} %then
 
  %if @{type} %then
   @{alter-table}
   ALTER [ COLUMN ] column_name [ SET DATA ] TYPE data_type [ COLLATE collation ] [ USING expression ]
  %end
  
  %if @{default} %then
    @{alter-table}
    ALTER [ COLUMN ] column_name SET DEFAULT expression
    ALTER [ COLUMN ] column_name DROP DEFAULT
  %end
  
  %if @{not-null} %then
    @{alter-table}
    ALTER [ COLUMN ] column_name { SET | DROP } NOT NULL
  %end
%end