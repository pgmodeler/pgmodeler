# SQL definition for permissions change
# PostgreSQL Version: 9.x
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made. 

%if {revoke} %then
 {definition}
%else
 [REVOKE ALL PRIVILEGES ON] {object} [ FROM ] {roles} 
 
  %if {cascade} %then
   [ CASCADE]
  %end
  
 ; $br [-- ddl-end --] $br
%end
