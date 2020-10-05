# SQL definition for policy's attributes change
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

{alter-cmds}
 
%if {has-changes} %then
    [ALTER ] {sql-object} $sp {signature} 

    %if {roles} %then
        $br $tb [TO ] 
        
        %if ({roles} == "unset") %then 
            PUBLIC
        %else
            {roles}
        %end
    %end   

    %if {using-exp} %then
        $br $tb [USING (] {using-exp} [)]
    %end

    %if {check-exp} %then
        $br $tb [WITH CHECK (] {check-exp} [)]
    %end
    
    ; 
    
    $br [-- ddl-end --] $br
%end  
