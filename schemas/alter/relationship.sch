# SQL definition for table inheritance configuration
# PostgreSQL Version: 9.x
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

[ALTER TABLE ] {table} 
 
%if ({inherit}=="unset") %then [ NO] %end
 
[ INHERIT ] {ancestor-table} ; $br
 
[-- ddl-end --] $br



