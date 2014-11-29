# SQL definition for database's attributes change
# PostgreSQL Version: 9.x
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if @{connlimit} %then
  [ALTER ] @{sql-object} $sp @{signature} [ WITH CONNECTION LIMIT ] @{connlimit} ; $br
  [-- ddl-end --] $br
%end