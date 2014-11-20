# SQL definition for table's attributes change
# PostgreSQL Version: 9.x
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

@{alter-cmds}

%if @{has-changes} %then
  ALTER $sp @{sql-object} $sp @{signature}
  
  %if @{oids} %then
    [ SET ]
  
    %if (@{oids}=="unset") %then
      WITHOUT
    %else
      WITH
    %end
  
    [ OIDS]
  %end

  ; $br
  [-- ddl-end --] $br
%end
