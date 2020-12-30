# SQL definition for extension's attributes change
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if ({pgsql-ver} != "9.0") %then
  {alter-cmds}
 
  %if {new-version} %then
   [ALTER ] {sql-object} $sp {signature} [ UPDATE TO ] '{new-version}' 
   $br [-- ddl-end --] $br
  %end
%end
