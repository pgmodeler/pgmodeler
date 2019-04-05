# Catalog queries for foreign data wrappers
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if {list} %then
    [ SELECT fw.oid, fdwname AS name FROM pg_foreign_data_wrapper AS fw ]

    %if {last-sys-oid} %or {not-ext-object} %then  
        [ WHERE ]
        
        %if {last-sys-oid} %then
            [ fw.oid ] {oid-filter-op} $sp {last-sys-oid}
        %end

        %if {not-ext-object} %then
            
            %if {last-sys-oid} %then
                [ AND ] 
            %end
            
            ( {not-ext-object} ) 
        %end
    %end
%else 



%end
