# SQL definition for table inheritance configuration
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%set {alter-table} [ALTER ] {sql-object} $sp 

%if {partitioned-table} %then

    %if ({pgsql-ver} >=f "10.0") %then
    
        {alter-table} {partitioned-table}
    
        %if ({partitioning}=="unset") %then [ DETACH] %else [ ATTACH] %end
        
        [ PARTITION ] {table} 
        
        %if ({partitioning}!="unset") %then
        
            %if {partition-bound-expr} %then 
                [ FOR VALUES ] {partition-bound-expr} 
            %else 
                [ DEFAULT] 
            %end
            
        %end    
        
    %end    

%else

    {alter-table} {table} 
 
    %if ({inherit}=="unset") %then [ NO] %end
 
    [ INHERIT ] {ancestor-table} 
%end

; 

$br [-- ddl-end --] $br



