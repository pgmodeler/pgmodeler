# Catalog queries for foreign server
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if {list} %then
    [ SELECT sv.oid, srvname AS name, current_database() AS parent, 'database' AS parent_type
      FROM pg_foreign_server AS sv ]

    %if {last-sys-oid} %or {not-ext-object}  %or {name-filter} %then  
        [ WHERE ]
        
        %if {last-sys-oid} %then
            [ sv.oid ] {oid-filter-op} $sp {last-sys-oid}
        %end

        %if {not-ext-object} %then
            
            %if {last-sys-oid} %then
                [ AND ] 
            %end
            
            ( {not-ext-object} ) 
        %end
        
        %if {name-filter} %then
            %if {last-sys-oid} %or {not-ext-object} %then
                [ AND ] 
            %end
           
            ( {name-filter} )
        %end        
    %end
%else 
    %if {attribs} %then
        [SELECT oid, srvname AS name, srvversion AS version, srvtype AS type,        
         srvfdw AS fdw, srvacl AS permission, srvowner AS owner, 
         srvoptions AS options, ]

        ({comment}) [ AS comment ]

        [ FROM pg_foreign_server AS sv ]
        
        %if {last-sys-oid} %then
            [ WHERE oid ] {oid-filter-op} $sp {last-sys-oid}
        %end

        %if {not-ext-object} %then
            %if {last-sys-oid} %then
                [ AND ]
            %else
                [ WHERE ]
            %end
	 
            ( {not-ext-object} )
        %end

        %if {filter-oids} %then
            %if {last-sys-oid} %or {not-ext-object} %then
                [ AND ]
            %else
                [ WHERE ]
            %end
            
            [ oid IN (] {filter-oids} )
        %end
    %end
%end
