# SQL definition for procedure's attributes change
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if {definition} %then
    {definition}
%else
    {alter-cmds}

    %if {has-changes} %then
        [ALTER ] {sql-object} $sp {signature} 

        %if {security-type} %then
            $br $tb {security-type}
        %end

        ;
        
        $br [-- ddl-end --] $br
    %end
%end
