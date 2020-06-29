# Catalog queries for policies
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if ({pgsql-ver} >=f "9.5") %then

    %if {list} %then
    
        %set {parent-name} [ ns.nspname || '.' || tb.relname ]

        %if {use-signature} %then
          %set {signature} {parent-name} [ || '.' || ]
        %end
    
        [ SELECT pl.oid, polname AS name, ] {parent-name} [ AS parent, 'table' AS parent_type 
         FROM pg_policy AS pl 
         LEFT JOIN pg_class AS tb ON pl.polrelid = tb.oid
         LEFT JOIN pg_namespace AS ns ON ns.oid = tb.relnamespace ]

        %if {schema} %then
        [ WHERE nspname= ] '{schema}'

            %if {table} %then
                [ AND tb.relkind='r' AND tb.relname=] '{table}'
            %end

            %if {last-sys-oid} %or {not-ext-object} %then 
                [ AND ] 
            %end
            
        %else
         [ WHERE ]
        %end
        
        %if {last-sys-oid} %then
            [ pl.oid ] {oid-filter-op} $sp {last-sys-oid}
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
        
            ( {signature} [ pl.polname ~* ] E'{name-filter}' )
        %end
        
        %if {extra-condition} %then

            %if {last-sys-oid} %or {not-ext-object} %or {name-filter} %then
                [ AND ]
            %end
            
            ( {extra-condition} )
        %end
        
    %else
        [SELECT pl.oid, pl.polname AS name, pl.polrelid AS table, ]

        %if ({pgsql-ver} >=f "10.0") %then
            [ pl.polpermissive AS permissive_bool, ]
        %else
            [ TRUE AS permissive_bool, ]
        %end

        [ pg_get_expr(pl.polqual, pl.polrelid, true) AS using_exp, 
          pg_get_expr(pl.polwithcheck, pl.polrelid, true) AS check_exp,

	 CASE pl.polcmd
	    WHEN  '*' THEN 'ALL'
	    WHEN  'r' THEN 'SELECT'
	    WHEN  'a' THEN 'INSERT'
	    WHEN  'w' THEN 'UPDATE'
	    ELSE 'DELETE'
	 END AS command, 
	 
	 pl.polroles AS roles, ]
	 
	 ({comment}) [ AS comment ]

        [ FROM pg_policy AS pl ]

        %if {last-sys-oid} %then
            [ WHERE pl.oid ] {oid-filter-op} $sp {last-sys-oid}
        %end

        %if {filter-oids} %then
            %if {last-sys-oid} %then
                [ AND]
            %else
                [ WHERE ]
            %end

            [ pl.oid IN (] {filter-oids} )
        %end

        %if {not-ext-object} %then
            %if {last-sys-oid} %or {filter-oids} %then
                [ AND ]
            %else
                [ WHERE ]
            %end
            ( {not-ext-object} )
        %end
    %end
%end
