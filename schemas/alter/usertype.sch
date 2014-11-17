# SQL definition for user defined types' attributes change
# PostgreSQL Version: 9.x
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.
 
@{alter-cmds}

%if (@{pgsql-ver} != "9.0") %and @{has-changes} %then 
 [ALTER ] @{sql-object} $sp @{name}
 $br [-- ddl-end --] $br
%end