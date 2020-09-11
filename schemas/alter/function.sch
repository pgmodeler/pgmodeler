# SQL definition for function's attributes change
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if {definition} %then
  {definition}
%else

{alter-cmds}

  %if {has-changes} %then
    [ALTER ] {sql-object} $sp {signature} 

    %if {function-type} %then
      $br $tb {function-type}
    %end

		%if ({pgsql-ver} >=f "9.2") %and {leakproof} %then
     $br $tb
    %if ({leakproof} == "unset") %then [ NOT ] %end 
      LEAKPROOF
    %end

    %if {behavior-type} %then
      $br $tb {behavior-type}
    %end

    %if {security-type} %then
      $br $tb {security-type}
    %end

    %if {execution-cost} %then
      $br $tb [COST ] {execution-cost}
    %end

    %if {returns-setof} %and {row-amount} %then
      $br $tb [ROWS ] {row-amount}
    %end

    ; 
    
    $br [-- ddl-end --] $br
  %end
%end
