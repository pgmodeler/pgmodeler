# SQL definition for object renaming
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if %not {constraint} %and %not {column} %then
  [ALTER ] {sql-object} $sp {signature} [ RENAME TO ] {new-name};
%else
  [ALTER TABLE ] {table} [ RENAME ] {sql-object} $sp {name} [ TO ] {new-name};
%end

$br [-- ddl-end --] $br
 
