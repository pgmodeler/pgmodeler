# SQL definition for table's attributes change
# PostgreSQL Version: 9.x
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

@{alter-cmds}

%if @{has-changes} %then
  ALTER $sp @{sql-object} $sp @{signature}
  
  %if @{oids} %then
    $br $tb [SET ]
  
    %if (@{oids}=="unset") %then
      WITHOUT
    %else
      WITH
    %end
  
    [ OIDS]
  %end

  %if @{inherit} %or @{no-inherit} %then

    %if @{no-inherit} %then
     $br $tb [NO INHERIT ] @{no-inherit}
    %end
    
    %if @{inherit} %then
     $br $tb  [INHERIT ] @{no-inherit}
    %end
    
  %end
  
  ; $br
  [-- ddl-end --] $br
%end