# SQL definition for function's attributes change
# PostgreSQL Version: 9.x
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

@{alter-cmds}

%if @{has-changes} %then
 [ALTER ] @{sql-object} $sp @{signature} 

  %if @{function-type} %then
    $br $sp @{function-type}
  %end
  
  %if (@{pgsql-ver} >= "9.2") %and @{leakproof} %then
    $br $sp
    %if (@{leakproof} == "unset") %then [ NOT ] %end 
    LEAKPROOF
  %end

  %if @{behavior-type} %then
    $br $sp @{behavior-type}
  %end

  %if @{security-type} %then
    $br $sp @{security-type}
  %end

  %if @{execution-cost} %then
    $br [ COST ] @{execution-cost}
  %end

  %if @{returns-setof} %and @{row-amount} %then
    $br [ ROWS ] @{row-amount}
  %end

  ; $br
  [-- ddl-end --] $br
%end
