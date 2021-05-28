# SQL definition for event trigger's attributes change
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if ({pgsql-ver} >=f "9.3") %then
 {alter-cmds}
%end
