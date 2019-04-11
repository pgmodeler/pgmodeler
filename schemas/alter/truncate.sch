# SQL definition for trucate tables
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.
 
[TRUNCATE ] {sql-object} $sp {signature} 

%if {restart-seq} %then
    [ RESTART IDENTITY ]
%end

%if {cascade} %then
    [ CASCADE]
%end

; 

$br [-- ddl-end --] $br
