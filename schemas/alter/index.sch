# SQL definition for index's attributes change
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

{alter-cmds}
 
%if {has-changes} %then
  [ALTER ] {sql-object} $sp {signature} 
 
  $br [SET (]

  %if {factor} %then
    [FILLFACTOR = ] {factor}
  %end

  %if {fast-update} %then
   %if {factor} %then [, ] %end
   [FASTUPDATE = ] %if ({fast-update} == "unset") %then OFF %else ON %end
  %end

	%if {buffering} %and ({pgsql-ver} >=f "9.2") %then
    %if {factor} %then [, ] %end
    [BUFFERING = ] %if ({buffering} == "unset") %then OFF %else ON %end
  %end

  [)]  ; 
  
  $br [-- ddl-end --] $br
  
%end
 
