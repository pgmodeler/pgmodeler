# Catalog queries for transforms
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if ({pgsql-ver} >=f "9.5") %then
    
    %set {transf-name} [trftype::regtype::text || '_' || lg.lanname ]

    %if {list} %then
    
        [ SELECT tr.oid, ] {transf-name} [ AS name, current_database() AS parent, 'database' AS parent_type
          FROM pg_transform AS tr 
          LEFT JOIN pg_language AS lg ON lg.oid = tr.trflang ]
        
        %if {last-sys-oid} %or {not-ext-object}  %or {name-filter} %then  
            [ WHERE ]
        
            %if {last-sys-oid} %then
                [ tr.oid ] {oid-filter-op} $sp {last-sys-oid}
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
                
                ( {transf-name} [ ~* ] E'{name-filter}' )
            %end    
        %end
    
    %else
        %if {attribs} %then

            [ SELECT tr.oid, ] {transf-name} [ AS name, tr.trftype AS type, lg.oid AS language, 
                     tr.trffromsql::regproc::oid AS fromsql, tr.trftosql::regproc::oid AS tosql
              FROM pg_transform AS tr 
              LEFT JOIN pg_language AS lg ON lg.oid = tr.trflang ]

            %if {last-sys-oid} %then
                [ WHERE tr.oid ] {oid-filter-op} $sp {last-sys-oid}
            %end

            %if {filter-oids} %then
                %if {last-sys-oid} %then
                    [ AND]
                %else
                    [ WHERE ]
                %end

                [ tr.oid IN (] {filter-oids} )
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
%end
