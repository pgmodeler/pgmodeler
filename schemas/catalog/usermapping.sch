# Catalog queries for user mapping
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if {list} %then
    %set {usermap-name} [um.usename || '@' || um.srvname]

    [ SELECT um.umid AS oid, ] {usermap-name} [ AS name, 
      current_database() AS parent, 'database' AS parent_type
      FROM pg_user_mappings AS um ]

    %if {last-sys-oid} %or {not-ext-object}  %or {name-filter} %then  
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
        
        %if {name-filter} %then
            
            %if {last-sys-oid} %or {not-ext-object} %then
                [ AND ] 
            %end
            
            ( {usermap-name} [ ~* ] E'{name-filter}' )
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
