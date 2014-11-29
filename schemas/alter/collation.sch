# SQL definition for collation's attributes change
# PostgreSQL Version: 9.x
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if (@{pgsql-ver} != "9.0") %then
 @{alter-cmds}
%end
