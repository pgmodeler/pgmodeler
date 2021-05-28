# SQL definition for function's attributes change
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

{alter-cmds}

%if {has-changes} %then
    [ALTER ] {sql-object} $sp {signature} 

    %if ({pgsql-ver} >=f "9.1") %and {handler} %then
        $br $tb
        
        %if ({handler} == "unset") %then 
            [ NO HANDLER] 
        %else
            [ HANDLER ] {handler}
        %end 
    %end
    
    %if {validator} %then
        $br $tb
        
        %if ({validator} == "unset") %then 
            [ NO VALIDATOR] 
        %else
            [ VALIDATOR ] {validator}
        %end 
    %end
    
    %if {options} %then
        $br $tb 
        [ OPTIONS (] {options} [)]
    %end

; 

$br [-- ddl-end --] $br

%end

