# Catalog queries for user mapping
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if {list} %then
    [ SELECT um.umid AS oid, um.usename || '@' || um.srvname AS name, 
      current_database() AS parent, 'database' AS parent_type
      FROM pg_user_mappings AS um ]

    %if {last-sys-oid} %or {not-ext-object} %then  
        [ WHERE ]
        
        %if {last-sys-oid} %then
            [ um.umid ] {oid-filter-op} $sp {last-sys-oid}
        %end

        %if {not-ext-object} %then
            
            %if {last-sys-oid} %then
                [ AND ] 
            %end
            
            ( {not-ext-object} ) 
        %end
    %end
%else 
    %if {attribs} %then
        [SELECT umid AS oid, usename || '@' || srvname AS name, um.srvid AS server, 
                umuser AS owner, umoptions AS options 
         FROM pg_user_mappings AS um ]
        
        %if {last-sys-oid} %then
            [ WHERE umid ] {oid-filter-op} $sp {last-sys-oid}
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
            
            [ umid IN (] {filter-oids} )
        %end
    %end
%end
