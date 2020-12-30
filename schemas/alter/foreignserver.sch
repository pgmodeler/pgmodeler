# SQL definition for foreign sever's attributes change
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

{alter-cmds}

%if {has-changes} %then
    [ALTER ] {sql-object} $sp {signature} 
    
    %if {version} %then
        [ VERSION ] '{version}'
        
    %end
    
    %if {options} %then
        $br $tb 
        [ OPTIONS (] {options} [)]
    %end

; 

$br [-- ddl-end --] $br

%end

